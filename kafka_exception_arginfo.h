/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3aa0ea15c7e42356db55f09a0d7a37ab506a3a00 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SimpleKafkaClient_KafkaErrorException___construct, 0, 0, 6)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, error_string, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, isFatal, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, isRetriable, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, transactionRequiresAbort, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_KafkaErrorException_getErrorString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_KafkaErrorException_isFatal, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_KafkaErrorException_isRetriable arginfo_class_SimpleKafkaClient_KafkaErrorException_isFatal

#define arginfo_class_SimpleKafkaClient_KafkaErrorException_transactionRequiresAbort arginfo_class_SimpleKafkaClient_KafkaErrorException_isFatal


ZEND_METHOD(SimpleKafkaClient_KafkaErrorException, __construct);
ZEND_METHOD(SimpleKafkaClient_KafkaErrorException, getErrorString);
ZEND_METHOD(SimpleKafkaClient_KafkaErrorException, isFatal);
ZEND_METHOD(SimpleKafkaClient_KafkaErrorException, isRetriable);
ZEND_METHOD(SimpleKafkaClient_KafkaErrorException, transactionRequiresAbort);


static const zend_function_entry class_SimpleKafkaClient_KafkaErrorException_methods[] = {
	ZEND_ME(SimpleKafkaClient_KafkaErrorException, __construct, arginfo_class_SimpleKafkaClient_KafkaErrorException___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_KafkaErrorException, getErrorString, arginfo_class_SimpleKafkaClient_KafkaErrorException_getErrorString, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_KafkaErrorException, isFatal, arginfo_class_SimpleKafkaClient_KafkaErrorException_isFatal, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_KafkaErrorException, isRetriable, arginfo_class_SimpleKafkaClient_KafkaErrorException_isRetriable, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_KafkaErrorException, transactionRequiresAbort, arginfo_class_SimpleKafkaClient_KafkaErrorException_transactionRequiresAbort, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_SimpleKafkaClient_Exception_methods[] = {
	ZEND_FE_END
};
