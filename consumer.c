/**
 *   BSD 3-Clause License
 *
 *  Copyright (c) 2016, Arnaud Le Blanc (Author)
 *  Copyright (c) 2020, Nick Chiu
 *  All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *      list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka_int.h"
#include "Zend/zend_exceptions.h"
#include "consumer_arginfo.h"

typedef struct _object_intern {
    rd_kafka_t              *rk;
    kafka_conf_callbacks    cbs;
    zend_object             std;
} object_intern;

static zend_class_entry * ce;
static zend_object_handlers handlers;

static void kafka_consumer_free(zend_object *object) /* {{{ */
{
    object_intern *intern = php_kafka_from_obj(object_intern, object);
    rd_kafka_resp_err_t err;
    kafka_conf_callbacks_dtor(&intern->cbs);

    if (intern->rk) {
        err = rd_kafka_consumer_close(intern->rk);

        if (err) {
            php_error(E_WARNING, "rd_kafka_consumer_close failed: %s", rd_kafka_err2str(err));
        }

        rd_kafka_destroy(intern->rk);
        intern->rk = NULL;
    }

    kafka_conf_callbacks_dtor(&intern->cbs);

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *kafka_consumer_new(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    object_intern *intern;

    intern = ecalloc(1, sizeof(object_intern)+ zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &handlers;

    return retval;
}
/* }}} */

static object_intern * get_object(zval *zconsumer) /* {{{ */
{
    object_intern *oconsumer = Z_KAFKA_P(object_intern, zconsumer);

    if (!oconsumer->rk) {
        zend_throw_exception_ex(NULL, 0, "Kafka\\Consumer::__construct() has not been called");
        return NULL;
    }

    return oconsumer;
} /* }}} */

static int has_group_id(rd_kafka_conf_t *conf) { /* {{{ */

    size_t len;

    if (conf == NULL) {
        return 0;
    }

    if (rd_kafka_conf_get(conf, "group.id", NULL, &len) != RD_KAFKA_CONF_OK) {
        return 0;
    }

    if (len <= 1) {
        return 0;
    }

    return 1;
} /* }}} */

/* {{{ proto Kafka\Consumer::__construct(Kafka\Configuration $conf) */
ZEND_METHOD(Kafka_Consumer, __construct)
{
    zval *zconf;
    char errstr[512];
    rd_kafka_t *rk;
    object_intern *intern;
    kafka_conf_object *conf_intern;
    rd_kafka_conf_t *conf = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zconf, ce_kafka_conf)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_KAFKA_P(object_intern, getThis());

    conf_intern = get_kafka_conf_object(zconf);
    if (conf_intern) {
        conf = rd_kafka_conf_dup(conf_intern->conf);
        kafka_conf_callbacks_copy(&intern->cbs, &conf_intern->cbs);
        intern->cbs.zrk = *getThis();
        rd_kafka_conf_set_opaque(conf, &intern->cbs);
    }

    if (!has_group_id(conf)) {
        if (conf) {
            rd_kafka_conf_destroy(conf);
        }
        zend_throw_exception(ce_kafka_exception, "\"group.id\" must be configured", 0);
        return;
    }

    rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));

    if (rk == NULL) {
        zend_throw_exception(ce_kafka_exception, errstr, 0);
        return;
    }

    if (intern->cbs.log) {
        rd_kafka_set_log_queue(rk, NULL);
    }

    intern->rk = rk;

    rd_kafka_poll_set_consumer(rk);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::assign([array $topics])
    Atomic assignment of partitions to consume */
ZEND_METHOD(Kafka_Consumer, assign)
{
    HashTable *htopars = NULL;
    rd_kafka_topic_partition_list_t *topics;
    rd_kafka_resp_err_t err;
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "|h!", &htopars) == FAILURE) {
        return;
    }

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_HT(htopars)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (htopars) {
        topics = array_arg_to_kafka_topic_partition_list(1, htopars);
        if (!topics) {
            return;
        }
    } else {
        topics = NULL;
    }

    err = rd_kafka_assign(intern->rk, topics);

    if (topics) {
        rd_kafka_topic_partition_list_destroy(topics);
    }

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ proto array Kafka\Consumer::getAssignment()
    Returns the current partition getAssignment */
ZEND_METHOD(Kafka_Consumer, getAssignment)
{
    rd_kafka_resp_err_t err;
    rd_kafka_topic_partition_list_t *topics;
    object_intern *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    err = rd_kafka_assignment(intern->rk, &topics);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }

    kafka_topic_partition_list_to_array(return_value, topics);
    rd_kafka_topic_partition_list_destroy(topics);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::subscribe(array $topics)
    Update the subscription set to $topics */
ZEND_METHOD(Kafka_Consumer, subscribe)
{
    HashTable *htopics;
    HashPosition pos;
    object_intern *intern;
    rd_kafka_topic_partition_list_t *topics;
    rd_kafka_resp_err_t err;
    zval *zv;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY_HT(htopics)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    topics = rd_kafka_topic_partition_list_new(zend_hash_num_elements(htopics));

    for (zend_hash_internal_pointer_reset_ex(htopics, &pos);
            (zv = zend_hash_get_current_data_ex(htopics, &pos)) != NULL;
            zend_hash_move_forward_ex(htopics, &pos)) {
        convert_to_string_ex(zv);
        rd_kafka_topic_partition_list_add(topics, Z_STRVAL_P(zv), RD_KAFKA_PARTITION_UA);
    }

    err = rd_kafka_subscribe(intern->rk, topics);

    rd_kafka_topic_partition_list_destroy(topics);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ proto array Kafka\Consumer::getSubscription()
   Returns the current subscription as set by subscribe() */
ZEND_METHOD(Kafka_Consumer, getSubscription)
{
    rd_kafka_resp_err_t err;
    rd_kafka_topic_partition_list_t *topics;
    object_intern *intern;
    int i;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    err = rd_kafka_subscription(intern->rk, &topics);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }

    array_init_size(return_value, topics->cnt);

    for (i = 0; i < topics->cnt; i++) {
        add_next_index_string(return_value, topics->elems[i].topic);
    }

    rd_kafka_topic_partition_list_destroy(topics);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::unsubsribe()
    Unsubscribe from the current subscription set */
ZEND_METHOD(Kafka_Consumer, unsubscribe)
{
    object_intern *intern;
    rd_kafka_resp_err_t err;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    err = rd_kafka_unsubscribe(intern->rk);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ proto Message Kafka\Consumer::consume()
   Consume message or get error event, triggers callbacks */
ZEND_METHOD(Kafka_Consumer, consume)
{
    object_intern *intern;
    zend_long timeout_ms;
    rd_kafka_message_t *rkmessage, rkmessage_tmp = {0};

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(timeout_ms)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    rkmessage = rd_kafka_consumer_poll(intern->rk, timeout_ms);

    if (!rkmessage) {
        rkmessage_tmp.err = RD_KAFKA_RESP_ERR__TIMED_OUT;
        rkmessage = &rkmessage_tmp;
    }

    kafka_message_new(return_value, rkmessage);

    if (rkmessage != &rkmessage_tmp) {
        rd_kafka_message_destroy(rkmessage);
    }
}
/* }}} */

static void consumer_commit(int async, INTERNAL_FUNCTION_PARAMETERS) /* {{{ */
{
    zval *zarg = NULL;
    object_intern *intern;
    rd_kafka_topic_partition_list_t *offsets = NULL;
    rd_kafka_resp_err_t err;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
	    Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(zarg)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (zarg) {
        if (Z_TYPE_P(zarg) == IS_OBJECT && instanceof_function(Z_OBJCE_P(zarg), ce_kafka_message)) {
            zval *zerr;
            zval *ztopic;
            zval *zpartition;
            zval *zoffset;
            rd_kafka_topic_partition_t *rktpar;

            zerr = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(zarg), ZEND_STRL("err"), 0);
            if (zerr && Z_TYPE_P(zerr) != IS_NULL && (Z_TYPE_P(zerr) != IS_LONG || Z_LVAL_P(zerr) != RD_KAFKA_RESP_ERR_NO_ERROR)) {
                zend_throw_exception(ce_kafka_exception, "Invalid argument: Specified Message has an error", RD_KAFKA_RESP_ERR__INVALID_ARG);
                return;
            }

            ztopic = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(zarg), ZEND_STRL("topic_name"), 0);
            if (!ztopic || Z_TYPE_P(ztopic) != IS_STRING) {
                zend_throw_exception(ce_kafka_exception, "Invalid argument: Specified Message's topic_name is not a string", RD_KAFKA_RESP_ERR__INVALID_ARG);
                return;
            }

            zpartition = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(zarg), ZEND_STRL("partition"), 0);
            if (!zpartition || Z_TYPE_P(zpartition) != IS_LONG) {
                zend_throw_exception(ce_kafka_exception, "Invalid argument: Specified Message's partition is not an int", RD_KAFKA_RESP_ERR__INVALID_ARG);
                return;
            }

            zoffset = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(zarg), ZEND_STRL("offset"), 0);
            if (!zoffset || Z_TYPE_P(zoffset) != IS_LONG) {
                zend_throw_exception(ce_kafka_exception, "Invalid argument: Specified Message's offset is not an int", RD_KAFKA_RESP_ERR__INVALID_ARG);
                return;
            }

            offsets = rd_kafka_topic_partition_list_new(1);
            rktpar = rd_kafka_topic_partition_list_add(
                    offsets, Z_STRVAL_P(ztopic),
                    Z_LVAL_P(zpartition));
            rktpar->offset = Z_LVAL_P(zoffset)+1;

        } else if (Z_TYPE_P(zarg) == IS_ARRAY) {
            HashTable *ary = Z_ARRVAL_P(zarg);
            offsets = array_arg_to_kafka_topic_partition_list(1, ary);
            if (!offsets) {
                return;
            }
        } else if (Z_TYPE_P(zarg) != IS_NULL) {
            php_error(E_ERROR,
                    "Kafka\\Consumer::%s() expects parameter %d to be %s, %s given",
                    get_active_function_name(),
                    1,
                    "an instance of Kafka\\Message or an array of Kafka\\TopicPartition",
                    zend_zval_type_name(zarg));
            return;
        }
    }

    err = rd_kafka_commit(intern->rk, offsets, async);

    if (offsets) {
        rd_kafka_topic_partition_list_destroy(offsets);
    }

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ proto void Kafka\Consumer::commit([mixed $message_or_offsets])
   Commit offsets */
ZEND_METHOD(Kafka_Consumer, commit)
{
    consumer_commit(0, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::commitAsync([mixed $message_or_offsets])
   Commit offsets */
ZEND_METHOD(Kafka_Consumer, commitAsync)
{
    consumer_commit(1, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::close()
   Close connection */
ZEND_METHOD(Kafka_Consumer, close)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    rd_kafka_consumer_close(intern->rk);
    intern->rk = NULL;
}
/* }}} */

/* {{{ proto Metadata Kafka\Consumer::getMetadata(bool all_topics, int timeout_ms, Kafka\Topic only_topic = null)
   Request Metadata from broker */
ZEND_METHOD(Kafka_Consumer, getMetadata)
{
    zend_bool all_topics;
    zval *only_zrkt = NULL;
    zend_long timeout_ms;
    rd_kafka_resp_err_t err;
    object_intern *intern;
    const rd_kafka_metadata_t *metadata;
    kafka_topic_object *only_orkt = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
        Z_PARAM_BOOL(all_topics)
        Z_PARAM_LONG(timeout_ms)
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_OF_CLASS(only_zrkt, ce_kafka_topic)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (only_zrkt) {
        only_orkt = get_kafka_topic_object(only_zrkt);
        if (!only_orkt) {
            return;
        }
    }

    err = rd_kafka_metadata(intern->rk, all_topics, only_orkt ? only_orkt->rkt : NULL, &metadata, timeout_ms);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }

    kafka_metadata_obj_init(return_value, metadata);
}
/* }}} */

/* {{{ proto Kafka\ConsumerTopic Kafka\Consumer::getTopicHandle(string $topic)
   Returns a Kafka\ConsumerTopic object */
ZEND_METHOD(Kafka_Consumer, getTopicHandle)
{
    char *topic;
    size_t topic_len;
    rd_kafka_topic_t *rkt;
    object_intern *intern;
    kafka_topic_object *topic_intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(topic, topic_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    rkt = rd_kafka_topic_new(intern->rk, topic, NULL);

    if (!rkt) {
        return;
    }

    if (object_init_ex(return_value, ce_kafka_consumer_topic) != SUCCESS) {
        return;
    }

    topic_intern = Z_KAFKA_P(kafka_topic_object, return_value);
    if (!topic_intern) {
        return;
    }

    topic_intern->rkt = rkt;
}
/* }}} */

/* {{{ proto array Kafka\Consumer::getCommittedOffsets(array $topics, int timeout_ms)
   Retrieve committed offsets for topics+partitions */
ZEND_METHOD(Kafka_Consumer, getCommittedOffsets)
{
    HashTable *htopars = NULL;
    zend_long timeout_ms;
    object_intern *intern;
    rd_kafka_resp_err_t err;
    rd_kafka_topic_partition_list_t *topics;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_ARRAY_HT(htopars)
        Z_PARAM_LONG(timeout_ms)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    topics = array_arg_to_kafka_topic_partition_list(1, htopars);
    if (!topics) {
        return;
    }

    err = rd_kafka_committed(intern->rk, topics, timeout_ms);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        rd_kafka_topic_partition_list_destroy(topics);
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
    kafka_topic_partition_list_to_array(return_value, topics);
    rd_kafka_topic_partition_list_destroy(topics);
}
/* }}} */

/* }}} */

/* {{{ proto array Kafka\Consumer::getOffsetPositions(array $topics)
   Retrieve current offsets for topics+partitions */
ZEND_METHOD(Kafka_Consumer, getOffsetPositions)
{
    HashTable *htopars = NULL;
    object_intern *intern;
    rd_kafka_resp_err_t err;
    rd_kafka_topic_partition_list_t *topics;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY_HT(htopars)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    topics = array_arg_to_kafka_topic_partition_list(1, htopars);
    if (!topics) {
        return;
    }

    err = rd_kafka_position(intern->rk, topics);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        rd_kafka_topic_partition_list_destroy(topics);
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
    kafka_topic_partition_list_to_array(return_value, topics);
    rd_kafka_topic_partition_list_destroy(topics);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::offsetsForTimes(array $topicPartitions, int $timeout_ms)
   Look up the offsets for the given partitions by timestamp. */
ZEND_METHOD(Kafka_Consumer, offsetsForTimes)
{
    HashTable *htopars = NULL;
    object_intern *intern;
    rd_kafka_topic_partition_list_t *topicPartitions;
    zend_long timeout_ms;
    rd_kafka_resp_err_t err;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_ARRAY_HT(htopars)
        Z_PARAM_LONG(timeout_ms)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    topicPartitions = array_arg_to_kafka_topic_partition_list(1, htopars);
    if (!topicPartitions) {
        return;
    }

    err = rd_kafka_offsets_for_times(intern->rk, topicPartitions, timeout_ms);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        rd_kafka_topic_partition_list_destroy(topicPartitions);
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
    kafka_topic_partition_list_to_array(return_value, topicPartitions);
    rd_kafka_topic_partition_list_destroy(topicPartitions);
}
/* }}} */

/* {{{ proto void Kafka\Consumer::queryWatermarkOffsets(string $topic, int $partition, int &$low, int &$high, int $timeout_ms)
   Query broker for low (oldest/beginning) or high (newest/end) offsets for partition */
ZEND_METHOD(Kafka_Consumer, queryWatermarkOffsets)
{
    object_intern *intern;
    char *topic;
    size_t topic_length;
    long low, high;
    zend_long partition, timeout_ms;
    zval *lowResult, *highResult;
    rd_kafka_resp_err_t err;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_STRING(topic, topic_length)
        Z_PARAM_LONG(partition)
        Z_PARAM_ZVAL(lowResult)
        Z_PARAM_ZVAL(highResult)
        Z_PARAM_LONG(timeout_ms)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_DEREF(lowResult);
    ZVAL_DEREF(highResult);

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    err = rd_kafka_query_watermark_offsets(intern->rk, topic, partition, &low, &high, timeout_ms);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }

    ZVAL_LONG(lowResult, low);
    ZVAL_LONG(highResult, high);
}
/* }}} */

void kafka_consumer_init(INIT_FUNC_ARGS) /* {{{ */
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "Kafka", "Consumer", class_Kafka_Consumer_methods);
    ce = zend_register_internal_class(&tmpce);
    ce->create_object = kafka_consumer_new;

    handlers = kafka_default_object_handlers;
    handlers.free_obj = kafka_consumer_free;
    handlers.offset = XtOffsetOf(object_intern, std);
}
