/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3950e26cab1ce664cffda7d6df362e2b488d2fe3 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_Topic_getTopic, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_Topic_getErr, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Metadata_Topic_getPartitions, 0, 0, Kafka\\Metadata\\Collection, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Kafka_Metadata_Topic, getTopic);
ZEND_METHOD(Kafka_Metadata_Topic, getErr);
ZEND_METHOD(Kafka_Metadata_Topic, getPartitions);


static const zend_function_entry class_Kafka_Metadata_Topic_methods[] = {
	ZEND_ME(Kafka_Metadata_Topic, getTopic, arginfo_class_Kafka_Metadata_Topic_getTopic, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Topic, getErr, arginfo_class_Kafka_Metadata_Topic_getErr, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Topic, getPartitions, arginfo_class_Kafka_Metadata_Topic_getPartitions, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
