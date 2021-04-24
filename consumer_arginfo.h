/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 378cc029a3673afe02572e7e17fde17e47b2aefd */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, configuration, SimpleKafkaClient\\Configuration, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_assign, 0, 0, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, topics, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_getAssignment, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_subscribe, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_Consumer_getSubscription arginfo_class_SimpleKafkaClient_Consumer_getAssignment

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_unsubscribe, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_consume, 0, 1, SimpleKafkaClient\\Message, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_commit, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, messageOrOffsets)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_Consumer_commitAsync arginfo_class_SimpleKafkaClient_Consumer_commit

#define arginfo_class_SimpleKafkaClient_Consumer_close arginfo_class_SimpleKafkaClient_Consumer_unsubscribe

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_getTopicHandle, 0, 1, SimpleKafkaClient\\ConsumerTopic, 0)
	ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_getCommittedOffsets, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Consumer_getOffsetPositions, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, topics, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(SimpleKafkaClient_Consumer, __construct);
ZEND_METHOD(SimpleKafkaClient_Consumer, assign);
ZEND_METHOD(SimpleKafkaClient_Consumer, getAssignment);
ZEND_METHOD(SimpleKafkaClient_Consumer, subscribe);
ZEND_METHOD(SimpleKafkaClient_Consumer, getSubscription);
ZEND_METHOD(SimpleKafkaClient_Consumer, unsubscribe);
ZEND_METHOD(SimpleKafkaClient_Consumer, consume);
ZEND_METHOD(SimpleKafkaClient_Consumer, commit);
ZEND_METHOD(SimpleKafkaClient_Consumer, commitAsync);
ZEND_METHOD(SimpleKafkaClient_Consumer, close);
ZEND_METHOD(SimpleKafkaClient_Consumer, getTopicHandle);
ZEND_METHOD(SimpleKafkaClient_Consumer, getCommittedOffsets);
ZEND_METHOD(SimpleKafkaClient_Consumer, getOffsetPositions);


static const zend_function_entry class_SimpleKafkaClient_Consumer_methods[] = {
	ZEND_ME(SimpleKafkaClient_Consumer, __construct, arginfo_class_SimpleKafkaClient_Consumer___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, assign, arginfo_class_SimpleKafkaClient_Consumer_assign, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, getAssignment, arginfo_class_SimpleKafkaClient_Consumer_getAssignment, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, subscribe, arginfo_class_SimpleKafkaClient_Consumer_subscribe, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, getSubscription, arginfo_class_SimpleKafkaClient_Consumer_getSubscription, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, unsubscribe, arginfo_class_SimpleKafkaClient_Consumer_unsubscribe, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, consume, arginfo_class_SimpleKafkaClient_Consumer_consume, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, commit, arginfo_class_SimpleKafkaClient_Consumer_commit, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, commitAsync, arginfo_class_SimpleKafkaClient_Consumer_commitAsync, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, close, arginfo_class_SimpleKafkaClient_Consumer_close, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, getTopicHandle, arginfo_class_SimpleKafkaClient_Consumer_getTopicHandle, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, getCommittedOffsets, arginfo_class_SimpleKafkaClient_Consumer_getCommittedOffsets, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Consumer, getOffsetPositions, arginfo_class_SimpleKafkaClient_Consumer_getOffsetPositions, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
