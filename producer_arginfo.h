/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b31742490463035fecff0a1cee01effed6e3e1d6 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Kafka_Producer___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, configuration, SimpleKafkaClient\\Configuration, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Producer_initTransactions, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Producer_beginTransaction, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Producer_commitTransaction arginfo_class_Kafka_Producer_initTransactions

#define arginfo_class_Kafka_Producer_abortTransaction arginfo_class_Kafka_Producer_initTransactions

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Producer_flush, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timeoutMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Producer_purge, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, purgeFlags, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Kafka_Producer, __construct);
ZEND_METHOD(Kafka_Producer, initTransactions);
ZEND_METHOD(Kafka_Producer, beginTransaction);
ZEND_METHOD(Kafka_Producer, commitTransaction);
ZEND_METHOD(Kafka_Producer, abortTransaction);
ZEND_METHOD(Kafka_Producer, flush);
ZEND_METHOD(Kafka_Producer, purge);


static const zend_function_entry class_Kafka_Producer_methods[] = {
	ZEND_ME(Kafka_Producer, __construct, arginfo_class_Kafka_Producer___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, initTransactions, arginfo_class_Kafka_Producer_initTransactions, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, beginTransaction, arginfo_class_Kafka_Producer_beginTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, commitTransaction, arginfo_class_Kafka_Producer_commitTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, abortTransaction, arginfo_class_Kafka_Producer_abortTransaction, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, flush, arginfo_class_Kafka_Producer_flush, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Producer, purge, arginfo_class_Kafka_Producer_purge, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
