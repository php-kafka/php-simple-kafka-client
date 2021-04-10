/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d6b7b0c7918396c561979bc7407be3a48035bde7 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_Broker_getId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SimpleKafkaClient_Metadata_Broker_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SimpleKafkaClient_Metadata_Broker_getPort arginfo_class_SimpleKafkaClient_Metadata_Broker_getId


ZEND_METHOD(SimpleKafkaClient_Metadata_Broker, getId);
ZEND_METHOD(SimpleKafkaClient_Metadata_Broker, getHost);
ZEND_METHOD(SimpleKafkaClient_Metadata_Broker, getPort);


static const zend_function_entry class_SimpleKafkaClient_Metadata_Broker_methods[] = {
	ZEND_ME(SimpleKafkaClient_Metadata_Broker, getId, arginfo_class_SimpleKafkaClient_Metadata_Broker_getId, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata_Broker, getHost, arginfo_class_SimpleKafkaClient_Metadata_Broker_getHost, ZEND_ACC_PUBLIC)
	ZEND_ME(SimpleKafkaClient_Metadata_Broker, getPort, arginfo_class_SimpleKafkaClient_Metadata_Broker_getPort, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
