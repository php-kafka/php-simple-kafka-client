/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_kafka_int.h"
#include "Zend/zend_exceptions.h"
#include "functions_arginfo.h"
#include "producer_arginfo.h"
#include "kafka_arginfo.h"

enum {
   RD_KAFKA_LOG_PRINT = 100
   , RD_KAFKA_LOG_SYSLOG = 101
   , RD_KAFKA_LOG_SYSLOG_PRINT = 102
};

typedef struct _toppar {
    rd_kafka_topic_t    *rkt;
    int32_t             partition;
} toppar;

static zend_object_handlers kafka_object_handlers;
zend_object_handlers kafka_default_object_handlers;
static zend_class_entry * ce_kafka;

static void kafka_free(zend_object *object) /* {{{ */
{
    kafka_object *intern = php_kafka_from_obj(kafka_object, object);

    if (intern->rk) {
        zend_hash_destroy(&intern->topics);

        rd_kafka_destroy(intern->rk);
        intern->rk = NULL;
    }

    kafka_conf_callbacks_dtor(&intern->cbs);

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *kafka_new(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    kafka_object *intern;

    intern = ecalloc(1, sizeof(kafka_object)+ zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &kafka_object_handlers;

    return retval;
}

kafka_object * get_kafka_object(zval *zrk)
{
    kafka_object *ork = Z_KAFKA_P(kafka_object, zrk);

    if (!ork->rk) {
        zend_throw_exception_ex(NULL, 0, "Kafka\\Kafka::__construct() has not been called");
        return NULL;
    }

    return ork;
}

/* {{{ private constructor */
ZEND_METHOD(Kafka, __construct)
{
    zend_throw_exception(NULL, "Private constructor", 0);
    return;
}
/* }}} */

/* {{{ proto Kafka\Metadata::getMetadata(bool $all_topics, int $timeout_ms, Kafka\Topic $topic)
   Request Metadata from broker */
ZEND_METHOD(Kafka_Kafka, getMetadata)
{
    zend_bool all_topics;
    zval *only_zrkt = NULL;
    zend_long timeout_ms;
    rd_kafka_resp_err_t err;
    kafka_object *intern;
    const rd_kafka_metadata_t *metadata;
    kafka_topic_object *only_orkt = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
        Z_PARAM_BOOL(all_topics)
        Z_PARAM_LONG(timeout_ms)
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_OF_CLASS_EX(only_zrkt, ce_kafka_topic, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
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

/* {{{ proto Kafka\Topic Kafka\Kafka::getTopicHandle(string $topic)
   Returns an Kafka\Topic object */
ZEND_METHOD(Kafka_Kafka, getTopicHandle)
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

/* {{{ proto int Kafka\Kafka::getOutQLen()
   Returns the current out queue length */
ZEND_METHOD(Kafka_Kafka, getOutQLen)
{
    kafka_object *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(rd_kafka_outq_len(intern->rk));
}
/* }}} */

/* {{{ proto int Kafka\Kafka::poll(int $timeoutMs)
   Polls the provided kafka handle for events */
ZEND_METHOD(Kafka_Kafka, poll)
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

    RETURN_LONG(rd_kafka_poll(intern->rk, timeout_ms));
}
/* }}} */

/* {{{ proto void Kafka\Kafka::queryWatermarkOffsets(string $topic, int $partition, int &$low, int &$high, int $timeout_ms)
   Query broker for low (oldest/beginning) or high (newest/end) offsets for partition */
ZEND_METHOD(Kafka_Kafka, queryWatermarkOffsets)
{
    kafka_object *intern;
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

    intern = get_kafka_object(getThis());
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

/* {{{ proto void Kafka\Kafka::offsetsForTimes(array $topicPartitions, int $timeout_ms)
   Look up the offsets for the given partitions by timestamp. */
ZEND_METHOD(Kafka_Kafka, offsetsForTimes)
{
    HashTable *htopars = NULL;
    kafka_object *intern;
    rd_kafka_topic_partition_list_t *topicPartitions;
    zend_long timeout_ms;
    rd_kafka_resp_err_t err;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_ARRAY_HT(htopars)
        Z_PARAM_LONG(timeout_ms)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_object(getThis());
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

#define COPY_CONSTANT(name) \
    REGISTER_LONG_CONSTANT(#name, name, CONST_CS | CONST_PERSISTENT)

void register_err_constants(INIT_FUNC_ARGS) /* {{{ */
{
    const struct rd_kafka_err_desc *errdescs;
    size_t cnt;
    size_t i;
    char buf[128];

    rd_kafka_get_err_descs(&errdescs, &cnt);

    for (i = 0; i < cnt; i++) {
        const struct rd_kafka_err_desc *desc = &errdescs[i];
        int len;

        if (!desc->name) {
            continue;
        }

        len = snprintf(buf, sizeof(buf), "RD_KAFKA_RESP_ERR_%s", desc->name);
        if ((size_t)len >= sizeof(buf)) {
            len = sizeof(buf)-1;
        }

        zend_register_long_constant(buf, len, desc->code, CONST_CS | CONST_PERSISTENT, module_number);
    }
} /* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(kafka)
{
    COPY_CONSTANT(RD_KAFKA_OFFSET_BEGINNING);
    COPY_CONSTANT(RD_KAFKA_OFFSET_END);
    COPY_CONSTANT(RD_KAFKA_OFFSET_STORED);
    COPY_CONSTANT(RD_KAFKA_PARTITION_UA);
    COPY_CONSTANT(RD_KAFKA_MSG_F_BLOCK);
    COPY_CONSTANT(RD_KAFKA_PURGE_F_QUEUE);
    COPY_CONSTANT(RD_KAFKA_PURGE_F_INFLIGHT);
    COPY_CONSTANT(RD_KAFKA_PURGE_F_NON_BLOCKING);
    REGISTER_LONG_CONSTANT("RD_KAFKA_VERSION", rd_kafka_version(), CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("RD_KAFKA_BUILD_VERSION", RD_KAFKA_VERSION, CONST_CS | CONST_PERSISTENT);

    register_err_constants(INIT_FUNC_ARGS_PASSTHRU);

    COPY_CONSTANT(RD_KAFKA_CONF_UNKNOWN);
    COPY_CONSTANT(RD_KAFKA_CONF_INVALID);
    COPY_CONSTANT(RD_KAFKA_CONF_OK);

    REGISTER_LONG_CONSTANT("RD_KAFKA_LOG_PRINT", RD_KAFKA_LOG_PRINT, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("RD_KAFKA_LOG_SYSLOG", RD_KAFKA_LOG_SYSLOG, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("RD_KAFKA_LOG_SYSLOG_PRINT", RD_KAFKA_LOG_SYSLOG_PRINT, CONST_CS | CONST_PERSISTENT);
    zend_class_entry ce;

    memcpy(&kafka_default_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    kafka_default_object_handlers.clone_obj = NULL;

	kafka_object_handlers = kafka_default_object_handlers;
    kafka_object_handlers.free_obj = kafka_free;
    kafka_object_handlers.offset = XtOffsetOf(kafka_object, std);

    INIT_CLASS_ENTRY(ce, "Kafka", class_Kafka_Kafka_methods);
    ce_kafka = zend_register_internal_class(&ce);
    ce_kafka->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    ce_kafka->create_object = kafka_new;

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "Producer", class_Kafka_Producer_methods);
    ce_kafka_producer = zend_register_internal_class_ex(&ce, ce_kafka);

    kafka_conf_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_error_init();
    kafka_consumer_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_message_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_metadata_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_metadata_topic_partition_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_topic_init(INIT_FUNC_ARGS_PASSTHRU);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(kafka)
{
    char *rd_kafka_version;

    php_info_print_table_start();
    php_info_print_table_row(2, "kafka support", "enabled");

    php_info_print_table_row(2, "version", PHP_KAFKA_VERSION);
    php_info_print_table_row(2, "build date", __DATE__ " " __TIME__);

    spprintf(
        &rd_kafka_version,
        0,
        "%u.%u.%u.%u",
        (RD_KAFKA_VERSION & 0xFF000000) >> 24,
        (RD_KAFKA_VERSION & 0x00FF0000) >> 16,
        (RD_KAFKA_VERSION & 0x0000FF00) >> 8,
        (RD_KAFKA_VERSION & 0x000000FF)
    );

    php_info_print_table_row(2, "librdkafka version (runtime)", rd_kafka_version_str());
    php_info_print_table_row(2, "librdkafka version (build)", rd_kafka_version);


    efree(rd_kafka_version);

    php_info_print_table_end();
}
/* }}} */

/* {{{ kafka_module_entry
 */
zend_module_entry kafka_module_entry = {
    STANDARD_MODULE_HEADER,
    "kafka",
    ext_functions,
    PHP_MINIT(kafka),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(kafka),
    PHP_KAFKA_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_KAFKA
ZEND_GET_MODULE(kafka)
#endif
