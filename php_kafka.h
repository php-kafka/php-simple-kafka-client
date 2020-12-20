/* $Id$ */

#ifndef PHP_KAFKA_H
#define PHP_KAFKA_H

#include "librdkafka/rdkafka.h"
#include "php_kafka_int.h"

#ifndef ZEND_FE_END
#define ZEND_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

typedef struct _kafka_object {
    rd_kafka_type_t         type;
    rd_kafka_t              *rk;
    kafka_conf_callbacks    cbs;
    HashTable               consuming;
	HashTable				topics;
	HashTable				queues;
    zend_object             std;
} kafka_object;

ZEND_METHOD(Kafka, __construct);

extern zend_module_entry kafka_module_entry;
#define phpext_kafka_ptr &kafka_module_entry

#define PHP_KAFKA_VERSION "1.0.0"

extern zend_object_handlers kafka_default_object_handlers;

#ifdef PHP_WIN32
#	define PHP_KAFKA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_KAFKA_API __attribute__ ((visibility("default")))
#else
#	define PHP_KAFKA_API
#endif

kafka_object * get_kafka_object(zval *zrk);

#endif	/* PHP_KAFKA_H */
