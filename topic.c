#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka.h"
#include "php_kafka_int.h"
#include "librdkafka/rdkafka.h"
#include "ext/spl/spl_iterators.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
#include "topic_arginfo.h"

static zend_object_handlers object_handlers;
zend_class_entry * ce_kafka_consumer_topic;
zend_class_entry * ce_kafka_producer_topic;
zend_class_entry * ce_kafka_topic;

typedef struct _php_callback {
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
} php_callback;

static void kafka_topic_free(zend_object *object) /* {{{ */
{
    kafka_topic_object *intern = php_kafka_from_obj(kafka_topic_object, object);

    if (Z_TYPE(intern->zrk) != IS_UNDEF && intern->rkt) {
        kafka_object *kafka_intern = get_kafka_object(&intern->zrk);
        if (kafka_intern) {
            zend_hash_index_del(&kafka_intern->topics, (zend_ulong)intern);
        }
    }

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *kafka_topic_new(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    kafka_topic_object *intern;

    intern = ecalloc(1, sizeof(kafka_topic_object)+ zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &object_handlers;

    return retval;
}
/* }}} */

kafka_topic_object * get_kafka_topic_object(zval *zrkt)
{
    kafka_topic_object *orkt = Z_KAFKA_P(kafka_topic_object, zrkt);

    if (!orkt->rkt) {
        zend_throw_exception_ex(NULL, 0, "Kafka\\Topic::__construct() has not been called");
        return NULL;
    }

    return orkt;
}

/* {{{ private constructor */
ZEND_METHOD(Kafka_ProducerTopic, __construct) {}
/* }}} */

/* {{{ proto void Kafka\ProducerTopic::produce(int $partition, int $msgflags[, string $payload, string $key])
   Produce and send a single message to broker. */
ZEND_METHOD(Kafka_ProducerTopic, produce)
{
    zend_long partition;
    zend_long msgflags;
    char *payload = NULL;
    size_t payload_len = 0;
    char *key = NULL;
    size_t key_len = 0;
    int ret;
    rd_kafka_resp_err_t err;
    kafka_topic_object *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 4)
        Z_PARAM_LONG(partition)
        Z_PARAM_LONG(msgflags)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING_EX(payload, payload_len, 1, 0)
        Z_PARAM_STRING_EX(key, key_len, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (partition != RD_KAFKA_PARTITION_UA && (partition < 0 || partition > 0x7FFFFFFF)) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Out of range value '%ld' for $partition", partition);
        return;
    }

    if (msgflags != 0 && msgflags != RD_KAFKA_MSG_F_BLOCK) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Invalid value '%ld' for $msgflags", msgflags);
        return;
    }

    intern = get_kafka_topic_object(getThis());

    ret = rd_kafka_produce(intern->rkt, partition, msgflags | RD_KAFKA_MSG_F_COPY, payload, payload_len, key, key_len, NULL);

    if (ret == -1) {
        err = rd_kafka_last_error();
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ proto void Kafka\ProducerTopic::producev(int $partition, int $msgflags[, string $payload, string $key, array $headers, int $timestamp_ms])
   Produce and send a single message to broker (with headers possibility and timestamp). */
ZEND_METHOD(Kafka_ProducerTopic, producev)
{
    zend_long partition;
    zend_long msgflags;
    char *payload = NULL;
    size_t payload_len = 0;
    char *key = NULL;
    size_t key_len = 0;
    rd_kafka_resp_err_t err;
    kafka_topic_object *intern;
    kafka_object *kafka_intern;
    HashTable *headersParam = NULL;
    HashPosition headersParamPos;
    char *header_key;
    zval *header_value;
    rd_kafka_headers_t *headers;
    zend_long timestamp_ms = 0;
    zend_bool timestamp_ms_is_null = 0;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 6)
        Z_PARAM_LONG(partition)
        Z_PARAM_LONG(msgflags)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING_EX(payload, payload_len, 1, 0)
        Z_PARAM_STRING_EX(key, key_len, 1, 0)
        Z_PARAM_ARRAY_HT_EX(headersParam, 1, 0)
        Z_PARAM_LONG_EX(timestamp_ms, timestamp_ms_is_null, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (partition != RD_KAFKA_PARTITION_UA && (partition < 0 || partition > 0x7FFFFFFF)) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Out of range value '%ld' for $partition", partition);
        return;
    }

    if (msgflags != 0 && msgflags != RD_KAFKA_MSG_F_BLOCK) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Invalid value '%ld' for $msgflags", msgflags);
        return;
    }

    if (timestamp_ms_is_null == 1) {
        timestamp_ms = 0;
    }

    intern = get_kafka_topic_object(getThis());

    if (headersParam != NULL && zend_hash_num_elements(headersParam) > 0) {
        headers = rd_kafka_headers_new(zend_hash_num_elements(headersParam));
        for (zend_hash_internal_pointer_reset_ex(headersParam, &headersParamPos);
                (header_value = zend_hash_get_current_data_ex(headersParam, &headersParamPos)) != NULL &&
                (header_key = kafka_hash_get_current_key_ex(headersParam, &headersParamPos)) != NULL;
                zend_hash_move_forward_ex(headersParam, &headersParamPos)) {
            convert_to_string_ex(header_value);
            rd_kafka_header_add(
                headers,
                header_key,
                -1, // Auto detect header title length
                Z_STRVAL_P(header_value),
                Z_STRLEN_P(header_value)
            );
        }
    } else {
        headers = rd_kafka_headers_new(0);
    }

    kafka_intern = get_kafka_object(&intern->zrk);
    if (!kafka_intern) {
        return;
    }

    err = rd_kafka_producev(
            kafka_intern->rk,
            RD_KAFKA_V_RKT(intern->rkt),
            RD_KAFKA_V_PARTITION(partition),
            RD_KAFKA_V_MSGFLAGS(msgflags | RD_KAFKA_MSG_F_COPY),
            RD_KAFKA_V_VALUE(payload, payload_len),
            RD_KAFKA_V_KEY(key, key_len),
            RD_KAFKA_V_TIMESTAMP(timestamp_ms),
            RD_KAFKA_V_HEADERS(headers),
            RD_KAFKA_V_END
    );

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        rd_kafka_headers_destroy(headers);
        zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
        return;
    }
}
/* }}} */

/* {{{ private constructor */
ZEND_METHOD(Kafka_ConsumerTopic, __construct) {}
/* }}} */

/* {{{ proto string Kafka\Topic::getName() */
ZEND_METHOD(Kafka_Topic, getName)
{
    kafka_topic_object *intern;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = get_kafka_topic_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_STRING(rd_kafka_topic_name(intern->rkt));
}
/* }}} */

void kafka_topic_init(INIT_FUNC_ARGS) { /* {{{ */

    zend_class_entry ce;

    memcpy(&object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    object_handlers.clone_obj = NULL;
    object_handlers.free_obj = kafka_topic_free;
    object_handlers.offset = XtOffsetOf(kafka_topic_object, std);

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "Topic", class_Kafka_Topic_methods);
    ce_kafka_topic = zend_register_internal_class(&ce);
    ce_kafka_topic->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    ce_kafka_topic->create_object = kafka_topic_new;

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "ConsumerTopic", class_Kafka_ConsumerTopic_methods);
    ce_kafka_consumer_topic = zend_register_internal_class_ex(&ce, ce_kafka_topic);

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "ProducerTopic", class_Kafka_ProducerTopic_methods);
    ce_kafka_producer_topic = zend_register_internal_class_ex(&ce, ce_kafka_topic);
} /* }}} */
