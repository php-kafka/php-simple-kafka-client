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
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_simple_kafka_client_int.h"
#include "Zend/zend_exceptions.h"
#include "topic_partition_arginfo.h"

typedef kafka_topic_partition_intern object_intern;

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp);

zend_class_entry * ce_kafka_topic_partition;

static zend_object_handlers handlers;

static void free_object(zend_object *object) /* {{{ */
{
    object_intern *intern = php_kafka_from_obj(object_intern, object);

    if (intern->topic) {
        efree(intern->topic);
    }

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *create_object(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    object_intern *intern;

    intern = ecalloc(1, sizeof(*intern));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &handlers;

    return retval;
}
/* }}} */

static object_intern * get_object(zval *z) /* {{{ */
{
    object_intern *intern = Z_KAFKA_P(object_intern, z);

    if (!intern->topic) {
        zend_throw_exception_ex(NULL, 0, "SimpleKafkaClient\\TopicPartition::__construct() has not been called");
        return NULL;
    }

    return intern;
} /* }}} */

kafka_topic_partition_intern * get_topic_partition_object(zval *z) /* {{{ */
{
    return get_object(z);
} /* }}} */

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp) /* {{{ */
{
    zval arr;
    object_intern *intern;

    *is_temp = 1;

    array_init(&arr);

    intern = kafka_get_debug_object(object_intern, object);

    if (!intern) {
        return Z_ARRVAL(arr);
    }

    if (intern->topic) {
        add_assoc_string(&arr, "topic", intern->topic);
    } else {
        add_assoc_null(&arr, "topic");
    }

    add_assoc_long(&arr, "partition", intern->partition);
    add_assoc_long(&arr, "offset", intern->offset);
    Z_TRY_ADDREF_P(&intern->metadata);
    add_assoc_zval(&arr, "metadata", &intern->metadata);

    return Z_ARRVAL(arr);
}
/* }}} */

void kafka_topic_partition_init(zval *zobj, char * topic, int32_t partition, int64_t offset, zval *metadata) /* {{{ */
{
    object_intern *intern;

    intern = Z_KAFKA_P(object_intern, zobj);
    if (!intern) {
        return;
    }

    if (intern->topic) {
        efree(intern->topic);
    }
    intern->topic = estrdup(topic);

    intern->partition = partition;
    intern->offset = offset;

    if (!metadata) {
        ZVAL_NULL(&intern->metadata);
    } else {
        intern->metadata = *metadata;
    }
} /* }}} */

void kafka_topic_partition_list_to_array(zval *return_value, rd_kafka_topic_partition_list_t *list) /* {{{ */
{
    rd_kafka_topic_partition_t *topar;
    zval ztopar;
    int i;

    array_init_size(return_value, list->cnt);

    for (i = 0; i < list->cnt; i++) {
        topar = &list->elems[i];
        ZVAL_NULL(&ztopar);
        object_init_ex(&ztopar, ce_kafka_topic_partition);
        kafka_topic_partition_init(&ztopar, topar->topic, topar->partition, topar->offset, topar->metadata);
        add_next_index_zval(return_value, &ztopar);
    }
} /* }}} */

rd_kafka_topic_partition_list_t * array_arg_to_kafka_topic_partition_list(int argnum, HashTable *ary) { /* {{{ */

    HashPosition pos;
    rd_kafka_topic_partition_list_t *list;
    zval *zv;

    list = rd_kafka_topic_partition_list_new(zend_hash_num_elements(ary));

    for (zend_hash_internal_pointer_reset_ex(ary, &pos);
            (zv = zend_hash_get_current_data_ex(ary, &pos)) != NULL;
            zend_hash_move_forward_ex(ary, &pos)) {
        kafka_topic_partition_intern *topar_intern;
        rd_kafka_topic_partition_t *topar;

        if (Z_TYPE_P(zv) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(zv), ce_kafka_topic_partition)) {
            const char *space;
            const char *class_name = get_active_class_name(&space);
            rd_kafka_topic_partition_list_destroy(list);
            php_error(E_ERROR,
                    "Argument %d passed to %s%s%s() must be an array of SimpleKafkaClient\\TopicPartition, at least one element is a(n) %s",
                    argnum,
                    class_name, space,
                    get_active_function_name(),
                    zend_zval_type_name(zv));
            return NULL;
        }

        topar_intern = get_topic_partition_object(zv);
        if (!topar_intern) {
            rd_kafka_topic_partition_list_destroy(list);
            return NULL;
        }

        topar = rd_kafka_topic_partition_list_add(list, topar_intern->topic, topar_intern->partition);
        topar->offset = topar_intern->offset;
    }

    return list;
} /* }}} */


/* {{{ proto void SimpleKafkaClient\TopicPartition::__construct(string $topic, int $partition[, int $offset, mixed $metadata])
   Constructor */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, __construct)
{
    char *topic;
    size_t topic_len;
    zend_long partition;
    zend_long offset = 0;
    zval *metadata = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 4)
        Z_PARAM_STRING(topic, topic_len)
        Z_PARAM_LONG(partition)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(offset)
        Z_PARAM_ZVAL_OR_NULL(metadata)
    ZEND_PARSE_PARAMETERS_END();

    kafka_topic_partition_init(getThis(), topic, partition, offset, metadata);
}
/* }}} */

/* {{{ proto string SimpleKafkaClient\TopicPartition::getTopicName()
   Returns topic name */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, getTopicName)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (intern->topic) {
        RETURN_STRING(intern->topic);
    } else {
        RETURN_NULL();
    }
}
/* }}} */

/* {{{ proto TopicPartition SimpleKafkaClient\TopicPartition::setTopicName($topicName)
   Sets topic name */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, setTopicName)
{
    char * topic;
    size_t topic_len;
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(topic, topic_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (intern->topic) {
        efree(intern->topic);
    }

    intern->topic = estrdup(topic);

    RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\TopicPartition::getPartition()
   Returns partition */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, getPartition)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(intern->partition);
}
/* }}} */

/* {{{ proto TopicPartition SimpleKafkaClient\TopicPartition::setPartition($partition)
   Sets partition */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, setPartition)
{
    zend_long partition;
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(partition)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    intern->partition = partition;

    RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\TopicPartition::getOffset()
   Returns offset */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, getOffset)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(intern->offset);
}
/* }}} */

/* {{{ proto TopicPartition SimpleKafkaClient\TopicPartition::setOffset($offset)
   Sets offset */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, setOffset)
{
    zend_long offset;
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    intern->offset = offset;

    RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\TopicPartition::getMetadata()
   Returns offset */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, getMetadata)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_ZVAL(&intern->metadata, 1, 0);
}
/* }}} */

/* {{{ proto TopicPartition SimpleKafkaClient\TopicPartition::setMetadata($metadata)
   Sets metadata */
ZEND_METHOD(SimpleKafkaClient_TopicPartition, setMetadata)
{
    zval *metadata = NULL;
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ZVAL(metadata)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    intern->metadata = *metadata;

    RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

void kafka_metadata_topic_partition_init(INIT_FUNC_ARGS) /* {{{ */
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "SimpleKafkaClient", "TopicPartition", class_SimpleKafkaClient_TopicPartition_methods);
    ce_kafka_topic_partition = zend_register_internal_class(&tmpce);
    ce_kafka_topic_partition->create_object = create_object;

    handlers = kafka_default_object_handlers;
    handlers.get_debug_info = get_debug_info;
    handlers.free_obj = free_object;
    handlers.offset = XtOffsetOf(object_intern, std);
} /* }}} */
