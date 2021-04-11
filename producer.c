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
#include "php_simple_kafka_client_int.h"
#include "Zend/zend_exceptions.h"
#include "producer_arginfo.h"

zend_class_entry * ce_kafka_producer;

static void kafka_topic_object_pre_free(kafka_topic_object ** pp) {
    kafka_topic_object *intern = *pp;
    rd_kafka_topic_destroy(intern->rkt);
    intern->rkt = NULL;
    zval_ptr_dtor(&intern->zrk);
}

static void kafka_init(zval *this_ptr, rd_kafka_type_t type, zval *zconf) /* {{{ */
{
    char errstr[512];
    rd_kafka_t *rk;
    kafka_object *intern;
    kafka_conf_object *conf_intern;
    rd_kafka_conf_t *conf = NULL;

    intern = Z_KAFKA_P(kafka_object, this_ptr);
    intern->type = type;

    if (zconf) {
        conf_intern = get_kafka_conf_object(zconf);
        if (conf_intern) {
            conf = rd_kafka_conf_dup(conf_intern->conf);
            kafka_conf_callbacks_copy(&intern->cbs, &conf_intern->cbs);
            intern->cbs.zrk = *this_ptr;
            rd_kafka_conf_set_opaque(conf, &intern->cbs);
        }
    }

    rk = rd_kafka_new(type, conf, errstr, sizeof(errstr));

    if (rk == NULL) {
        zend_throw_exception(ce_kafka_exception, errstr, 0);
        return;
    }

    if (intern->cbs.log) {
        //rd_kafka_set_log_queue(rk, NULL);
    }

    intern->rk = rk;

    zend_hash_init(&intern->topics, 0, NULL, (dtor_func_t)kafka_topic_object_pre_free, 0);
}
/* }}} */

/* {{{ proto SimpleKafkaClient\Producer::__construct([SimpleKafkaClient\Configuration $configuration]) */
ZEND_METHOD(SimpleKafkaClient_Producer, __construct)
{
    zval *zconf = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zconf, ce_kafka_conf)
    ZEND_PARSE_PARAMETERS_END();

    kafka_init(getThis(), RD_KAFKA_PRODUCER, zconf);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::flush(int $timeout_ms)
   Wait until all outstanding produce requests, et.al, are completed. */
ZEND_METHOD(SimpleKafkaClient_Producer, flush)
{
    kafka_object *intern;
    zend_long timeout_ms;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(timeout_ms)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(rd_kafka_flush(intern->rk, timeout_ms));
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::purge(int $purge_flags)
   Purge messages that are in queue or in flight */
ZEND_METHOD(SimpleKafkaClient_Producer, purge)
{
    kafka_object *intern;
    zend_long purge_flags;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(purge_flags)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(rd_kafka_purge(intern->rk, purge_flags));
}
/* }}} */


/* {{{ proto SimpleKafkaClient\ProducerTopic SimpleKafkaClient\Producer::getTopicHandle(string $topic)
   Returns an SimpleKafkaClient\ProducerTopic object */
ZEND_METHOD(SimpleKafkaClient_Producer, getTopicHandle)
{
    char *topic;
    size_t topic_len;
    rd_kafka_topic_t *rkt;
    kafka_object *intern;
    kafka_topic_object *topic_intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(topic, topic_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    rkt = rd_kafka_topic_new(intern->rk, topic, NULL);

    if (!rkt) {
        return;
    }

    if (object_init_ex(return_value, ce_kafka_producer_topic) != SUCCESS) {
        return;
    }

    topic_intern = Z_KAFKA_P(kafka_topic_object, return_value);
    if (!topic_intern) {
        return;
    }

    topic_intern->rkt = rkt;
    topic_intern->zrk = *getThis();

    Z_ADDREF_P(&topic_intern->zrk);

    zend_hash_index_add_ptr(&intern->topics, (zend_ulong)topic_intern, topic_intern);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::initTransactions(int timeout_ms)
   Initializes transactions, needs to be done before producing and starting a transaction */
ZEND_METHOD(SimpleKafkaClient_Producer, initTransactions)
{
    kafka_object *intern;
    zend_long timeout_ms;
    const rd_kafka_error_t *error;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(timeout_ms)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    error = rd_kafka_init_transactions(intern->rk, timeout_ms);

    if (NULL == error) {
        return;
    }

    create_kafka_error(return_value, error);
    zend_throw_exception_object(return_value);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::beginTransaction()
   Start a transaction */
ZEND_METHOD(SimpleKafkaClient_Producer, beginTransaction)
{
    kafka_object *intern;
    const rd_kafka_error_t *error;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    error = rd_kafka_begin_transaction(intern->rk);

    if (NULL == error) {
        return;
    }

    create_kafka_error(return_value, error);
    zend_throw_exception_object(return_value);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::commitTransaction(int timeout_ms)
   Commit a transaction */
ZEND_METHOD(SimpleKafkaClient_Producer, commitTransaction)
{
    kafka_object *intern;
    zend_long timeout_ms;
    const rd_kafka_error_t *error;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(timeout_ms)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    error = rd_kafka_commit_transaction(intern->rk, timeout_ms);

    if (NULL == error) {
        return;
    }

    create_kafka_error(return_value, error);
    zend_throw_exception_object(return_value);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Producer::abortTransaction(int timeout_ms)
   Commit a transaction */
ZEND_METHOD(SimpleKafkaClient_Producer, abortTransaction)
{
    kafka_object *intern;
    zend_long timeout_ms;
    const rd_kafka_error_t *error;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(timeout_ms)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    error = rd_kafka_abort_transaction(intern->rk, timeout_ms);

    if (NULL == error) {
        return;
    }

    create_kafka_error(return_value, error);
    zend_throw_exception_object(return_value);
}
/* }}} */
