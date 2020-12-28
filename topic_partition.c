#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_kafka_int.h"
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
        zend_throw_exception_ex(NULL, 0, "Kafka\\TopicPartition::__construct() has not been called");
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
    zval ary;
    object_intern *intern;

    *is_temp = 1;

    array_init(&ary);

    intern = kafka_get_debug_object(object_intern, object);

    if (!intern) {
        return Z_ARRVAL(ary);
    }

    if (intern->topic) {
        add_assoc_string(&ary, "topic", intern->topic);
    } else {
        add_assoc_null(&ary, "topic");
    }

    add_assoc_long(&ary, "partition", intern->partition);
    add_assoc_long(&ary, "offset", intern->offset);

    return Z_ARRVAL(ary);
}
/* }}} */

void kafka_topic_partition_init(zval *zobj, char * topic, int32_t partition, int64_t offset) /* {{{ */
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
        kafka_topic_partition_init(&ztopar, topar->topic, topar->partition, topar->offset);
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
                    "Argument %d passed to %s%s%s() must be an array of Kafka\\TopicPartition, at least one element is a(n) %s",
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


/* {{{ proto void Kafka\TopicPartition::__construct(string $topic, int $partition[, int $offset])
   Constructor */
ZEND_METHOD(Kafka_TopicPartition, __construct)
{
    char *topic;
    size_t topic_len;
    zend_long partition;
    zend_long offset = 0;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
        Z_PARAM_STRING(topic, topic_len)
        Z_PARAM_LONG(partition)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    kafka_topic_partition_init(getThis(), topic, partition, offset);
}
/* }}} */

/* {{{ proto string Kafka\TopicPartition::getTopicName()
   Returns topic name */
ZEND_METHOD(Kafka_TopicPartition, getTopicName)
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

/* {{{ proto TopicPartition Kafka\TopicPartition::setTopicName($topicName)
   Sets topic name */
ZEND_METHOD(Kafka_TopicPartition, setTopicName)
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

/* {{{ proto int Kafka\TopicPartition::getPartition()
   Returns partition */
ZEND_METHOD(Kafka_TopicPartition, getPartition)
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

/* {{{ proto TopicPartition Kafka\TopicPartition::setPartition($partition)
   Sets partition */
ZEND_METHOD(Kafka_TopicPartition, setPartition)
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

/* {{{ proto int Kafka\TopicPartition::getOffset()
   Returns offset */
ZEND_METHOD(Kafka_TopicPartition, getOffset)
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

/* {{{ proto TopicPartition Kafka\TopicPartition::setOffset($offset)
   Sets offset */
ZEND_METHOD(Kafka_TopicPartition, setOffset)
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

void kafka_metadata_topic_partition_init(INIT_FUNC_ARGS) /* {{{ */
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "Kafka", "TopicPartition", class_Kafka_TopicPartition_methods);
    ce_kafka_topic_partition = zend_register_internal_class(&tmpce);
    ce_kafka_topic_partition->create_object = create_object;

    handlers = kafka_default_object_handlers;
    handlers.get_debug_info = get_debug_info;
    handlers.free_obj = free_object;
    handlers.offset = XtOffsetOf(object_intern, std);
} /* }}} */
