/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 88d2ac53ad8266413f1ab448883a9b2b439120ef */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Topic_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SimpleKafkaClient_ConsumerTopic___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_ProducerTopic___construct arginfo_class_SimpleKafkaClient_ConsumerTopic___construct

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_ProducerTopic_produce, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, msgFlags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, key, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, opaque, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_ProducerTopic_producev, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, msgFlags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, key, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, headers, IS_ARRAY, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, timestampMs, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, opaque, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()


ZEND_METHOD(SimpleKafkaClient_Topic, getName);
ZEND_METHOD(SimpleKafkaClient_ConsumerTopic, __construct);
ZEND_METHOD(SimpleKafkaClient_ProducerTopic, __construct);
ZEND_METHOD(SimpleKafkaClient_ProducerTopic, produce);
ZEND_METHOD(SimpleKafkaClient_ProducerTopic, producev);


static const zend_function_entry class_SimpleKafkaClient_Topic_methods[] = {
	ZEND_ME(SimpleKafkaClient_Topic, getName, arginfo_class_SimpleKafkaClient_Topic_getName, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_SimpleKafkaClient_ConsumerTopic_methods[] = {
	ZEND_ME(SimpleKafkaClient_ConsumerTopic, __construct, arginfo_class_SimpleKafkaClient_ConsumerTopic___construct, ZEND_ACC_PRIVATE)
	ZEND_FE_END
};


static const zend_function_entry class_SimpleKafkaClient_ProducerTopic_methods[] = {
	ZEND_ME(SimpleKafkaClient_ProducerTopic, __construct, arginfo_class_SimpleKafkaClient_ProducerTopic___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(SimpleKafkaClient_ProducerTopic, produce, arginfo_class_SimpleKafkaClient_ProducerTopic_produce, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_ProducerTopic, producev, arginfo_class_SimpleKafkaClient_ProducerTopic_producev, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
