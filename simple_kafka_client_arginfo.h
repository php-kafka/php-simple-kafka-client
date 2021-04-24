/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 54165f3ef5d3833ee646b825574c959323fd612b */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_getMetadata, 0, 3, SimpleKafkaClient\\Metadata, 0)
	ZEND_ARG_TYPE_INFO(0, allTopics, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, topic, SimpleKafkaClient\\Topic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_getOutQLen, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_queryWatermarkOffsets, 0, 5, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, low, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, high, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_offsetsForTimes, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topicPartitions, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_setOAuthBearerTokenFailure, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, errorString, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_setOAuthBearerToken, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, token, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, lifetimeMs, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, principalName, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, extensions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()


ZEND_METHOD(SimpleKafkaClient_Kafka, getMetadata);
ZEND_METHOD(SimpleKafkaClient_Kafka, getOutQLen);
ZEND_METHOD(SimpleKafkaClient_Kafka, queryWatermarkOffsets);
ZEND_METHOD(SimpleKafkaClient_Kafka, offsetsForTimes);
ZEND_METHOD(SimpleKafkaClient_Kafka, setOAuthBearerTokenFailure);
ZEND_METHOD(SimpleKafkaClient_Kafka, setOAuthBearerToken);


static const zend_function_entry class_SimpleKafkaClient_Kafka_methods[] = {
	ZEND_ME(SimpleKafkaClient_Kafka, getMetadata, arginfo_class_SimpleKafkaClient_Kafka_getMetadata, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, getOutQLen, arginfo_class_SimpleKafkaClient_Kafka_getOutQLen, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, queryWatermarkOffsets, arginfo_class_SimpleKafkaClient_Kafka_queryWatermarkOffsets, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, offsetsForTimes, arginfo_class_SimpleKafkaClient_Kafka_offsetsForTimes, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, setOAuthBearerTokenFailure, arginfo_class_SimpleKafkaClient_Kafka_setOAuthBearerTokenFailure, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, setOAuthBearerToken, arginfo_class_SimpleKafkaClient_Kafka_setOAuthBearerToken, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
