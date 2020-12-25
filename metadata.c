#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka_int.h"
#include "Zend/zend_exceptions.h"
#include "metadata_arginfo.h"

typedef struct _object_intern {
    const rd_kafka_metadata_t *metadata;
    zend_object               std;
} object_intern;

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp);

static zend_class_entry * ce;
static zend_object_handlers handlers;

static void brokers_collection(zval *return_value, Z_KAFKA_OBJ *parent, object_intern *intern) { /* {{{ */
    kafka_metadata_collection_obj_init(return_value, parent, intern->metadata->brokers, intern->metadata->broker_cnt, sizeof(*intern->metadata->brokers), kafka_metadata_broker_ctor);
}
/* }}} */

static void topics_collection(zval *return_value, Z_KAFKA_OBJ *parent, object_intern *intern) { /* {{{ */
    kafka_metadata_collection_obj_init(return_value, parent, intern->metadata->topics, intern->metadata->topic_cnt, sizeof(*intern->metadata->topics), kafka_metadata_topic_ctor);
}
/* }}} */

static void kafka_metadata_free(zend_object *object) /* {{{ */
{
    object_intern *intern = php_kafka_from_obj(object_intern, object);

    if (intern->metadata) {
        rd_kafka_metadata_destroy(intern->metadata);
    }

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *kafka_metadata_new(zend_class_entry *class_type) /* {{{ */
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

static object_intern * get_object(zval *zmetadata)
{
    object_intern *ometadata = Z_KAFKA_P(object_intern, zmetadata);

    if (!ometadata->metadata) {
        zend_throw_exception_ex(NULL, 0, "Kafka\\Metadata::__construct() has not been called");
        return NULL;
    }

    return ometadata;
}

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp) /* {{{ */
{
    zval ary;
    object_intern *intern;
    zval brokers;
    zval topics;

    *is_temp = 1;

    array_init(&ary);

    intern = kafka_get_debug_object(object_intern, object);
    if (!intern) {
        return Z_ARRVAL(ary);
    }

    ZVAL_NULL(&brokers);
    brokers_collection(&brokers, object, intern);
    add_assoc_zval(&ary, "brokers", &brokers);

    ZVAL_NULL(&topics);
    topics_collection(&topics, object, intern);
    add_assoc_zval(&ary, "topics", &topics);

    add_assoc_long(&ary, "orig_broker_id", intern->metadata->orig_broker_id);
    add_assoc_string(&ary, "orig_broker_name", intern->metadata->orig_broker_name);

    return Z_ARRVAL(ary);
}
/* }}} */

/* {{{ proto long Kafka\Metadata::getOrigBrokerId()
   Broker originating this metadata */
ZEND_METHOD(Kafka_Metadata, getOrigBrokerId)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(intern->metadata->orig_broker_id);
}
/* }}} */

/* {{{ proto string Kafka\Metadata::getOrigBrokerName()
   Name of originating broker */
ZEND_METHOD(Kafka_Metadata, getOrigBrokerName)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_STRING(intern->metadata->orig_broker_name);
}
/* }}} */

/* {{{ proto Kafka\Metadata\Collection Kafka\Metadata::getBrokers()
   Topics */
ZEND_METHOD(Kafka_Metadata, getBrokers)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    brokers_collection(return_value, Z_KAFKA_PROP_OBJ(getThis()), intern);
}
/* }}} */

/* {{{ proto Kafka\Metadata\Collection Kafka\Metadata::getTopics()
   Topics */
ZEND_METHOD(Kafka_Metadata, getTopics)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    topics_collection(return_value, Z_KAFKA_PROP_OBJ(getThis()), intern);
}
/* }}} */

void kafka_metadata_init(INIT_FUNC_ARGS)
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "Kafka", "Metadata", class_Kafka_Metadata_methods);
    ce = zend_register_internal_class(&tmpce);
    ce->create_object = kafka_metadata_new;

    handlers = kafka_default_object_handlers;
    handlers.get_debug_info = get_debug_info;
    handlers.free_obj = kafka_metadata_free;
    handlers.offset = XtOffsetOf(object_intern, std);

    kafka_metadata_topic_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_metadata_broker_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_metadata_partition_init(INIT_FUNC_ARGS_PASSTHRU);
    kafka_metadata_collection_init(INIT_FUNC_ARGS_PASSTHRU);
}

void kafka_metadata_obj_init(zval *return_value, const rd_kafka_metadata_t *metadata)
{
    object_intern *intern;

    if (object_init_ex(return_value, ce) != SUCCESS) {
        return;
    }

    intern = Z_KAFKA_P(object_intern, return_value);
    if (!intern) {
        return;
    }

    intern->metadata = metadata;
}
