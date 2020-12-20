/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e34e94f591ac3caf4ed3916d8d15252f96c8fc6b */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Kafka_Metadata_Partition_getId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Metadata_Partition_getErr arginfo_class_Kafka_Metadata_Partition_getId

#define arginfo_class_Kafka_Metadata_Partition_getLeader arginfo_class_Kafka_Metadata_Partition_getId

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Kafka_Metadata_Partition_getReplicas, 0, 0, Kafka\\Metadata\\Collection, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Kafka_Metadata_Partition_getIsrs arginfo_class_Kafka_Metadata_Partition_getReplicas


ZEND_METHOD(Kafka_Metadata_Partition, getId);
ZEND_METHOD(Kafka_Metadata_Partition, getErr);
ZEND_METHOD(Kafka_Metadata_Partition, getLeader);
ZEND_METHOD(Kafka_Metadata_Partition, getReplicas);
ZEND_METHOD(Kafka_Metadata_Partition, getIsrs);


static const zend_function_entry class_Kafka_Metadata_Partition_methods[] = {
	ZEND_ME(Kafka_Metadata_Partition, getId, arginfo_class_Kafka_Metadata_Partition_getId, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Partition, getErr, arginfo_class_Kafka_Metadata_Partition_getErr, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Partition, getLeader, arginfo_class_Kafka_Metadata_Partition_getLeader, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Partition, getReplicas, arginfo_class_Kafka_Metadata_Partition_getReplicas, ZEND_ACC_PUBLIC)
	ZEND_ME(Kafka_Metadata_Partition, getIsrs, arginfo_class_Kafka_Metadata_Partition_getIsrs, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
