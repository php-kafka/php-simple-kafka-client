/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f629d931de4ad3a347583a46a7fce628f567f8ee */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Kafka_Configuration___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Configuration_dump, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Configuration_set, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Configuration_setErrorCb, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Configuration_setDrMsgCb arginfo_class_Kafka_Configuration_setErrorCb

#define arginfo_class_Kafka_Configuration_setStatsCb arginfo_class_Kafka_Configuration_setErrorCb

#define arginfo_class_Kafka_Configuration_setRebalanceCb arginfo_class_Kafka_Configuration_setErrorCb

#define arginfo_class_Kafka_Configuration_setOffsetCommitCb arginfo_class_Kafka_Configuration_setErrorCb

#define arginfo_class_Kafka_Configuration_setLogCb arginfo_class_Kafka_Configuration_setErrorCb


ZEND_METHOD(Kafka_Configuration, __construct);
ZEND_METHOD(Kafka_Configuration, dump);
ZEND_METHOD(Kafka_Configuration, set);
ZEND_METHOD(Kafka_Configuration, setErrorCb);
ZEND_METHOD(Kafka_Configuration, setDrMsgCb);
ZEND_METHOD(Kafka_Configuration, setStatsCb);
ZEND_METHOD(Kafka_Configuration, setRebalanceCb);
ZEND_METHOD(Kafka_Configuration, setOffsetCommitCb);
ZEND_METHOD(Kafka_Configuration, setLogCb);


static const zend_function_entry class_Kafka_Configuration_methods[] = {
	ZEND_ME(Kafka_Configuration, __construct, arginfo_class_Kafka_Configuration___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, dump, arginfo_class_Kafka_Configuration_dump, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, set, arginfo_class_Kafka_Configuration_set, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setErrorCb, arginfo_class_Kafka_Configuration_setErrorCb, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setDrMsgCb, arginfo_class_Kafka_Configuration_setDrMsgCb, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setStatsCb, arginfo_class_Kafka_Configuration_setStatsCb, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setRebalanceCb, arginfo_class_Kafka_Configuration_setRebalanceCb, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setOffsetCommitCb, arginfo_class_Kafka_Configuration_setOffsetCommitCb, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Configuration, setLogCb, arginfo_class_Kafka_Configuration_setLogCb, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
