/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 42cd23c50573fc0f32161ddf8bc6650b7f4a3657 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Topic_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Kafka_ConsumerTopic___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_ProducerTopic___construct arginfo_class_Kafka_ConsumerTopic___construct

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_ProducerTopic_produce, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, msgFlags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, key, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, opaque, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_ProducerTopic_producev, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, msgFlags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, key, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, headers, IS_ARRAY, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, timestampMs, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, opaque, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()


ZEND_METHOD(Kafka_Topic, getName);
ZEND_METHOD(Kafka_ConsumerTopic, __construct);
ZEND_METHOD(Kafka_ProducerTopic, __construct);
ZEND_METHOD(Kafka_ProducerTopic, produce);
ZEND_METHOD(Kafka_ProducerTopic, producev);


static const zend_function_entry class_Kafka_Topic_methods[] = {
	ZEND_ME(Kafka_Topic, getName, arginfo_class_Kafka_Topic_getName, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Kafka_ConsumerTopic_methods[] = {
	ZEND_ME(Kafka_ConsumerTopic, __construct, arginfo_class_Kafka_ConsumerTopic___construct, ZEND_ACC_PRIVATE)
	ZEND_FE_END
};


static const zend_function_entry class_Kafka_ProducerTopic_methods[] = {
	ZEND_ME(Kafka_ProducerTopic, __construct, arginfo_class_Kafka_ProducerTopic___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Kafka_ProducerTopic, produce, arginfo_class_Kafka_ProducerTopic_produce, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_ProducerTopic, producev, arginfo_class_Kafka_ProducerTopic_producev, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
