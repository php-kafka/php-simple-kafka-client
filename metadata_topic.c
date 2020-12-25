#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka_int.h"
#include "librdkafka/rdkafka.h"
#include "ext/spl/spl_iterators.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "metadata_topic_arginfo.h"

typedef struct _object_intern {
    zval                            zmetadata;
    const rd_kafka_metadata_topic_t *metadata_topic;
    zend_object                     std;
} object_intern;

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp);

static zend_class_entry * ce;
static zend_object_handlers handlers;

static void partitions_collection(zval *return_value, Z_KAFKA_OBJ *parent, object_intern *intern) { /* {{{ */
    kafka_metadata_collection_obj_init(return_value, parent, intern->metadata_topic->partitions, intern->metadata_topic->partition_cnt, sizeof(*intern->metadata_topic->partitions), kafka_metadata_partition_ctor);
}
/* }}} */

static void free_object(zend_object *object) /* {{{ */
{
    object_intern *intern = php_kafka_from_obj(object_intern, object);

    if (intern->metadata_topic) {
        zval_dtor(&intern->zmetadata);
    }

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *create_object(zend_class_entry *class_type) /* {{{ */
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

static object_intern * get_object(zval *zmt)
{
    object_intern *omt = Z_KAFKA_P(object_intern, zmt);

    if (!omt->metadata_topic) {
        zend_throw_exception_ex(NULL, 0, "Kafka\\Metadata\\Topic::__construct() has not been called");
        return NULL;
    }

    return omt;
}

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp) /* {{{ */
{
    zval ary;
    object_intern *intern;
    zval partitions;

    *is_temp = 1;

    array_init(&ary);

    intern = kafka_get_debug_object(object_intern, object);
    if (!intern) {
        return Z_ARRVAL(ary);
    }

    add_assoc_string(&ary, "topic", intern->metadata_topic->topic);

    ZVAL_NULL(&partitions);
    partitions_collection(&partitions, object, intern);
    add_assoc_zval(&ary, "partitions", &partitions);

    add_assoc_long(&ary, "err", intern->metadata_topic->err);

    return Z_ARRVAL(ary);
}
/* }}} */

/* {{{ proto string Kafka\MetadataTopic::getTopic()
   Topic name */
ZEND_METHOD(Kafka_Metadata_Topic, getTopic)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_STRING(intern->metadata_topic->topic);
}
/* }}} */

/* {{{ proto int Kafka\MetadataTopic::getErr()
   Error */
ZEND_METHOD(Kafka_Metadata_Topic, getErr)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(intern->metadata_topic->err);
}
/* }}} */


/* {{{ proto Kafka\Metadata\Collection Kafka\Metadata\Topic::getPartitions()
   Partitions */
ZEND_METHOD(Kafka_Metadata_Topic, getPartitions)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    partitions_collection(return_value, Z_KAFKA_PROP_OBJ(getThis()), intern);
}
/* }}} */

void kafka_metadata_topic_init(INIT_FUNC_ARGS)
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "Kafka\\Metadata", "Topic", class_Kafka_Metadata_Topic_methods);
    ce = zend_register_internal_class(&tmpce);
    ce->create_object = create_object;

    handlers = kafka_default_object_handlers;
    handlers.get_debug_info = get_debug_info;
    handlers.free_obj = free_object;
    handlers.offset = XtOffsetOf(object_intern, std);
}

void kafka_metadata_topic_ctor(zval *return_value, zval *zmetadata, const void *data)
{
    rd_kafka_metadata_topic_t *metadata_topic = (rd_kafka_metadata_topic_t*)data;
    object_intern *intern;

    if (object_init_ex(return_value, ce) != SUCCESS) {
        return;
    }

    intern = Z_KAFKA_P(object_intern, return_value);
    if (!intern) {
        return;
    }

    ZVAL_ZVAL(&intern->zmetadata, zmetadata, 1, 0);
    intern->metadata_topic = metadata_topic;
}
