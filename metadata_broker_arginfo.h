/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 6ebc975620a8a396520899d51de9831b8b2c0680 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_Broker_getId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_Broker_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Metadata_Broker_getPort arginfo_class_Kafka_Metadata_Broker_getId


ZEND_METHOD(Kafka_Metadata_Broker, getId);
ZEND_METHOD(Kafka_Metadata_Broker, getHost);
ZEND_METHOD(Kafka_Metadata_Broker, getPort);


static const zend_function_entry class_Kafka_Metadata_Broker_methods[] = {
	ZEND_ME(Kafka_Metadata_Broker, getId, arginfo_class_Kafka_Metadata_Broker_getId, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Broker, getHost, arginfo_class_Kafka_Metadata_Broker_getHost, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Broker, getPort, arginfo_class_Kafka_Metadata_Broker_getPort, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
