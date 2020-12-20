#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka.h"
#include "php_kafka_int.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "php_kafka_int.h"
#include "kafka_exception_arginfo.h"

zend_class_entry * ce_kafka_error_exception;
zend_class_entry * ce_kafka_exception;

void create_kafka_error(zval *return_value, const rd_kafka_error_t *error) /* {{{ */
{
    object_init_ex(return_value, ce_kafka_error_exception);

    zend_update_property_string(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("message"), rd_kafka_error_name(error));
    zend_update_property_long(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("code"), rd_kafka_error_code(error));
    zend_update_property_string(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("error_string"), rd_kafka_error_string(error));
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("isFatal"), rd_kafka_error_is_fatal(error));
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("isRetriable"), rd_kafka_error_is_retriable(error));
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("transactionRequiresAbort"), rd_kafka_error_txn_requires_abort(error));

    Z_ADDREF_P(return_value);
}
/* }}} */

/* {{{ proto Kafka\KafkaErrorException::__construct(string $message, int $code[, string $error_string, bool $isFatal, bool $isRetriable, bool $transactionRequiresAbort]) */
PHP_METHOD(Kafka_KafkaErrorException, __construct)
{
    char *message, *error_string = "";
    size_t message_length = 0, error_string_length = 0;
    zend_bool isFatal = 0, isRetriable = 0, transactionRequiresAbort = 0;
    zend_long code = 0;


	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 6)
		Z_PARAM_STRING(message, message_length)
		Z_PARAM_LONG(code)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(error_string, error_string_length)
		Z_PARAM_BOOL(isFatal)
		Z_PARAM_BOOL(isRetriable)
		Z_PARAM_BOOL(transactionRequiresAbort)
	ZEND_PARSE_PARAMETERS_END();

    zend_update_property_string(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("message"), message);
    zend_update_property_long(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("code"), code);
    zend_update_property_string(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("error_string"), error_string);
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("isFatal"), isFatal);
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("isRetriable"), isRetriable);
    zend_update_property_bool(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("transactionRequiresAbort"), transactionRequiresAbort);
}
/* }}} */

/* {{{ proto void Kafka\KafkaErrorException::getErrorString()
    Get name of error */
PHP_METHOD(Kafka_KafkaErrorException, getErrorString)
{
    zval *res;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    res = kafka_read_property(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("error_string"), 0);

    if (!res || Z_TYPE_P(res) != IS_STRING) {
        return;
    }

    ZVAL_DEREF(res);
    ZVAL_COPY(return_value, res);
}
/* }}} */


/* {{{ proto void Kafka\KafkaErrorException::isFatal()
    Return true if error is fatal */
PHP_METHOD(Kafka_KafkaErrorException, isFatal)
{
    zval *res;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    res = kafka_read_property(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("isFatal"), 0);

    if (!res || (Z_TYPE_P(res) != IS_TRUE && Z_TYPE_P(res) != IS_FALSE)) {
        return;
    }

    ZVAL_DEREF(res);
    ZVAL_COPY(return_value, res);
}
/* }}} */

/* {{{ proto void Kafka\KafkaErrorException::isRetriable()
    Return true if error is fatal */
PHP_METHOD(Kafka_KafkaErrorException, isRetriable)
{
    zval *res;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    res = kafka_read_property(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("isRetriable"), 0);

    if (!res || (Z_TYPE_P(res) != IS_TRUE && Z_TYPE_P(res) != IS_FALSE)) {
        return;
    }

    ZVAL_DEREF(res);
    ZVAL_COPY(return_value, res);
}
/* }}} */

/* {{{ proto void Kafka\KafkaErrorException::transactionRequiresAbort()
    Return true if error is fatal */
PHP_METHOD(Kafka_KafkaErrorException, transactionRequiresAbort)
{
    zval *res;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    res = kafka_read_property(ce_kafka_error_exception, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("transactionRequiresAbort"), 0);

    if (!res || (Z_TYPE_P(res) != IS_TRUE && Z_TYPE_P(res) != IS_FALSE)) {
        return;
    }

    ZVAL_DEREF(res);
    ZVAL_COPY(return_value, res);
}
/* }}} */

void kafka_error_init() /* {{{ */
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "Exception", NULL);
    ce_kafka_exception = zend_register_internal_class_ex(&ce, zend_ce_exception);

    INIT_NS_CLASS_ENTRY(ce, "Kafka", "KafkaErrorException", class_Kafka_KafkaErrorException_methods);
    ce_kafka_error_exception = zend_register_internal_class_ex(&ce, ce_kafka_exception);

    zend_declare_property_null(ce_kafka_error_exception, ZEND_STRL("error_string"), ZEND_ACC_PRIVATE);
    zend_declare_property_bool(ce_kafka_error_exception, ZEND_STRL("isFatal"), 0, ZEND_ACC_PRIVATE);
    zend_declare_property_bool(ce_kafka_error_exception, ZEND_STRL("isRetriable"), 0, ZEND_ACC_PRIVATE);
    zend_declare_property_bool(ce_kafka_error_exception, ZEND_STRL("transactionRequiresAbort"), 0, ZEND_ACC_PRIVATE);
} /* }}} */
