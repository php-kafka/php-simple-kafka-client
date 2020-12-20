/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b42d1bb68767786f0b655714b388a3361a45551d */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Kafka_Consumer___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, configuration, Kafka\\Configuration, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_assign, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_getAssignment, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Consumer_subscribe arginfo_class_Kafka_Consumer_assign

#define arginfo_class_Kafka_Consumer_getSubscription arginfo_class_Kafka_Consumer_getAssignment

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_unsubscribe, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Consumer_consume, 0, 1, Kafka\\Message, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_commit, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, messageOrOffsets)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Consumer_commitAsync arginfo_class_Kafka_Consumer_commit

#define arginfo_class_Kafka_Consumer_close arginfo_class_Kafka_Consumer_unsubscribe

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Consumer_getMetadata, 0, 3, Kafka\\Metadata, 0)
	ZEND_ARG_TYPE_INFO(0, allTopics, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, topic, Kafka\\ConsumerTopic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Consumer_getTopicHandle, 0, 1, Kafka\\ConsumerTopic, 0)
	ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_getCommittedOffsets, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_getOffsetPositions, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_offsetsForTimes, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topicPartitions, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Consumer_queryWatermarkOffsets, 0, 5, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, partition, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, low, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, high, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Kafka_Consumer, __construct);
ZEND_METHOD(Kafka_Consumer, assign);
ZEND_METHOD(Kafka_Consumer, getAssignment);
ZEND_METHOD(Kafka_Consumer, subscribe);
ZEND_METHOD(Kafka_Consumer, getSubscription);
ZEND_METHOD(Kafka_Consumer, unsubscribe);
ZEND_METHOD(Kafka_Consumer, consume);
ZEND_METHOD(Kafka_Consumer, commit);
ZEND_METHOD(Kafka_Consumer, commitAsync);
ZEND_METHOD(Kafka_Consumer, close);
ZEND_METHOD(Kafka_Consumer, getMetadata);
ZEND_METHOD(Kafka_Consumer, getTopicHandle);
ZEND_METHOD(Kafka_Consumer, getCommittedOffsets);
ZEND_METHOD(Kafka_Consumer, getOffsetPositions);
ZEND_METHOD(Kafka_Consumer, offsetsForTimes);
ZEND_METHOD(Kafka_Consumer, queryWatermarkOffsets);


static const zend_function_entry class_Kafka_Consumer_methods[] = {
	ZEND_ME(Kafka_Consumer, __construct, arginfo_class_Kafka_Consumer___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, assign, arginfo_class_Kafka_Consumer_assign, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getAssignment, arginfo_class_Kafka_Consumer_getAssignment, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, subscribe, arginfo_class_Kafka_Consumer_subscribe, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getSubscription, arginfo_class_Kafka_Consumer_getSubscription, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, unsubscribe, arginfo_class_Kafka_Consumer_unsubscribe, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, consume, arginfo_class_Kafka_Consumer_consume, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, commit, arginfo_class_Kafka_Consumer_commit, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, commitAsync, arginfo_class_Kafka_Consumer_commitAsync, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, close, arginfo_class_Kafka_Consumer_close, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getMetadata, arginfo_class_Kafka_Consumer_getMetadata, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getTopicHandle, arginfo_class_Kafka_Consumer_getTopicHandle, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getCommittedOffsets, arginfo_class_Kafka_Consumer_getCommittedOffsets, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, getOffsetPositions, arginfo_class_Kafka_Consumer_getOffsetPositions, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, offsetsForTimes, arginfo_class_Kafka_Consumer_offsetsForTimes, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Consumer, queryWatermarkOffsets, arginfo_class_Kafka_Consumer_queryWatermarkOffsets, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
