/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: da83d0319c899361606dfa0ccf0fd439aeeabfbb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_getOrigBrokerId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_getOrigBrokerName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Metadata_getBrokers, 0, 0, Kafka\\Metadata\\Collection, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Metadata_getTopics arginfo_class_Kafka_Metadata_getBrokers


ZEND_METHOD(Kafka_Metadata, getOrigBrokerId);
ZEND_METHOD(Kafka_Metadata, getOrigBrokerName);
ZEND_METHOD(Kafka_Metadata, getBrokers);
ZEND_METHOD(Kafka_Metadata, getTopics);


static const zend_function_entry class_Kafka_Metadata_methods[] = {
	ZEND_ME(Kafka_Metadata, getOrigBrokerId, arginfo_class_Kafka_Metadata_getOrigBrokerId, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata, getOrigBrokerName, arginfo_class_Kafka_Metadata_getOrigBrokerName, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata, getBrokers, arginfo_class_Kafka_Metadata_getBrokers, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata, getTopics, arginfo_class_Kafka_Metadata_getTopics, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
