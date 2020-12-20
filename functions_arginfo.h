/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e9788b5cc0cda9d51b182a72d08b4e699b7b5962 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kafka_get_err_descs, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kafka_err2str, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, errorCode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kafka_thread_cnt, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kafka_offset_tail, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_FUNCTION(kafka_get_err_descs);
ZEND_FUNCTION(kafka_err2str);
ZEND_FUNCTION(kafka_thread_cnt);
ZEND_FUNCTION(kafka_offset_tail);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(kafka_get_err_descs, arginfo_kafka_get_err_descs)
	ZEND_FE(kafka_err2str, arginfo_kafka_err2str)
	ZEND_FE(kafka_thread_cnt, arginfo_kafka_thread_cnt)
	ZEND_FE(kafka_offset_tail, arginfo_kafka_offset_tail)
	ZEND_FE_END
};
