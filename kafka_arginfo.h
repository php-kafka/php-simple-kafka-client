/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5620609ea29ca05a20736ac8412bee6e4cc39615 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_getMetadata, 0, 3, SimpleKafkaClient\\Metadata, 0)
	ZEND_ARG_TYPE_INFO(0, allTopics, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, topic, SimpleKafkaClient\\Topic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_getOutQLen, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Kafka_poll, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
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


ZEND_METHOD(SimpleKafkaClient_Kafka, getMetadata);
ZEND_METHOD(SimpleKafkaClient_Kafka, getOutQLen);
ZEND_METHOD(SimpleKafkaClient_Kafka, poll);
ZEND_METHOD(SimpleKafkaClient_Kafka, queryWatermarkOffsets);
ZEND_METHOD(SimpleKafkaClient_Kafka, offsetsForTimes);


static const zend_function_entry class_SimpleKafkaClient_Kafka_methods[] = {
	ZEND_ME(SimpleKafkaClient_Kafka, getMetadata, arginfo_class_SimpleKafkaClient_Kafka_getMetadata, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, getOutQLen, arginfo_class_SimpleKafkaClient_Kafka_getOutQLen, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, poll, arginfo_class_SimpleKafkaClient_Kafka_poll, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, queryWatermarkOffsets, arginfo_class_SimpleKafkaClient_Kafka_queryWatermarkOffsets, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Kafka, offsetsForTimes, arginfo_class_SimpleKafkaClient_Kafka_offsetsForTimes, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
