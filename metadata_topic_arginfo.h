/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: db8552307bc3c0d4d6035ff10c00b7e2a39a152a */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_Topic_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_Topic_getErrorCode, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_Topic_getPartitions, 0, 0, SimpleKafkaClient\\Metadata\\Collection, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(SimpleKafkaClient_Metadata_Topic, getName);
ZEND_METHOD(SimpleKafkaClient_Metadata_Topic, getErrorCode);
ZEND_METHOD(SimpleKafkaClient_Metadata_Topic, getPartitions);


static const zend_function_entry class_SimpleKafkaClient_Metadata_Topic_methods[] = {
	ZEND_ME(SimpleKafkaClient_Metadata_Topic, getName, arginfo_class_SimpleKafkaClient_Metadata_Topic_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata_Topic, getErrorCode, arginfo_class_SimpleKafkaClient_Metadata_Topic_getErrorCode, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata_Topic, getPartitions, arginfo_class_SimpleKafkaClient_Metadata_Topic_getPartitions, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
