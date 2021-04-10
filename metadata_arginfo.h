/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: da83d0319c899361606dfa0ccf0fd439aeeabfbb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_getOrigBrokerId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_getOrigBrokerName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_getBrokers, 0, 0, SimpleKafkaClient\\Metadata\\Collection, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_Metadata_getTopics arginfo_class_SimpleKafkaClient_Metadata_getBrokers


ZEND_METHOD(SimpleKafkaClient_Metadata, getOrigBrokerId);
ZEND_METHOD(SimpleKafkaClient_Metadata, getOrigBrokerName);
ZEND_METHOD(SimpleKafkaClient_Metadata, getBrokers);
ZEND_METHOD(SimpleKafkaClient_Metadata, getTopics);


static const zend_function_entry class_SimpleKafkaClient_Metadata_methods[] = {
	ZEND_ME(SimpleKafkaClient_Metadata, getOrigBrokerId, arginfo_class_SimpleKafkaClient_Metadata_getOrigBrokerId, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata, getOrigBrokerName, arginfo_class_SimpleKafkaClient_Metadata_getOrigBrokerName, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata, getBrokers, arginfo_class_SimpleKafkaClient_Metadata_getBrokers, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata, getTopics, arginfo_class_SimpleKafkaClient_Metadata_getTopics, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
