/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ae03dd8127a9e4799e241bc490de200ff18a4178 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SimpleKafkaClient_Producer___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, configuration, SimpleKafkaClient\\Configuration, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Producer_initTransactions, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Producer_beginTransaction, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_Producer_commitTransaction arginfo_class_SimpleKafkaClient_Producer_initTransactions

#define arginfo_class_SimpleKafkaClient_Producer_abortTransaction arginfo_class_SimpleKafkaClient_Producer_initTransactions

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Producer_flush, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Producer_purge, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, purgeFlags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Producer_getTopicHandle, 0, 1, SimpleKafkaClient\\ProducerTopic, 0)
	ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(SimpleKafkaClient_Producer, __construct);
ZEND_METHOD(SimpleKafkaClient_Producer, initTransactions);
ZEND_METHOD(SimpleKafkaClient_Producer, beginTransaction);
ZEND_METHOD(SimpleKafkaClient_Producer, commitTransaction);
ZEND_METHOD(SimpleKafkaClient_Producer, abortTransaction);
ZEND_METHOD(SimpleKafkaClient_Producer, flush);
ZEND_METHOD(SimpleKafkaClient_Producer, purge);
ZEND_METHOD(SimpleKafkaClient_Producer, getTopicHandle);


static const zend_function_entry class_SimpleKafkaClient_Producer_methods[] = {
	ZEND_ME(SimpleKafkaClient_Producer, __construct, arginfo_class_SimpleKafkaClient_Producer___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, initTransactions, arginfo_class_SimpleKafkaClient_Producer_initTransactions, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, beginTransaction, arginfo_class_SimpleKafkaClient_Producer_beginTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, commitTransaction, arginfo_class_SimpleKafkaClient_Producer_commitTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, abortTransaction, arginfo_class_SimpleKafkaClient_Producer_abortTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, flush, arginfo_class_SimpleKafkaClient_Producer_flush, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, purge, arginfo_class_SimpleKafkaClient_Producer_purge, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Producer, getTopicHandle, arginfo_class_SimpleKafkaClient_Producer_getTopicHandle, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
