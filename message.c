/**
 *   BSD 3-Clause License
 *
 *  Copyright (c) 2016, Arnaud Le Blanc (Author)
 *  Copyright (c) 2020, Nick Chiu
 *  All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *      list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_kafka_int.h"
#include "ext/spl/spl_iterators.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "message_arginfo.h"

zend_class_entry * ce_kafka_message;

void kafka_message_new(zval *return_value, const rd_kafka_message_t *message)
{
    object_init_ex(return_value, ce_kafka_message);

    rd_kafka_timestamp_type_t tstype;
    int64_t timestamp;

    timestamp = rd_kafka_message_timestamp(message, &tstype);

    rd_kafka_headers_t *message_headers = NULL;
    rd_kafka_resp_err_t header_response;
    const char *header_name = NULL;
    const void *header_value = NULL;
    size_t header_size = 0;
    zval headers_array;
    size_t i;

    zend_update_property_long(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("err"), message->err);

    if (message->rkt) {
        zend_update_property_string(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("topic_name"), rd_kafka_topic_name(message->rkt));
    }
    zend_update_property_long(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("partition"), message->partition);
    if (message->payload) {
        zend_update_property_long(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("timestamp"), timestamp);
        zend_update_property_stringl(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("payload"), message->payload, message->len);
        zend_update_property_long(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("len"), message->len);
    }
    if (message->key) {
        zend_update_property_stringl(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("key"), message->key, message->key_len);
    }
    zend_update_property_long(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("offset"), message->offset);

    if (message->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
        rd_kafka_message_headers(message, &message_headers);
        if (message_headers != NULL) {
            array_init(&headers_array);
            for (i = 0; i < rd_kafka_header_cnt(message_headers); i++) {
                header_response = rd_kafka_header_get_all(message_headers, i, &header_name, &header_value, &header_size);
                if (header_response != RD_KAFKA_RESP_ERR_NO_ERROR) {
                    break;
                }
                add_assoc_stringl(&headers_array, header_name, (const char*)header_value, header_size);
            }
            zend_update_property(NULL, Z_KAFKA_PROP_OBJ(return_value), ZEND_STRL("headers"), &headers_array);
            zval_ptr_dtor(&headers_array);
        }
    }
}

void kafka_message_list_to_array(zval *return_value, rd_kafka_message_t **messages, long size) /* {{{ */
{
    rd_kafka_message_t *msg;
    zval zmsg;
    int i;

    array_init_size(return_value, size);

    for (i = 0; i < size; i++) {
        msg = messages[i];
        ZVAL_NULL(&zmsg);
        kafka_message_new(&zmsg, msg);
        add_next_index_zval(return_value, &zmsg);
    }
} /* }}} */

/* {{{ proto string SimpleKafkaClient\Message::getErrorString()
 *  Returns the error string for an errored KrSimpleKafkaClient\Message or NULL if there was no error.
 */
ZEND_METHOD(Kafka_Message, getErrorString)
{
    zval *zerr;
    zval *zpayload;
    const char *errstr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "") == FAILURE) {
        return;
    }

    zerr = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("err"), 0);

    if (!zerr || Z_TYPE_P(zerr) != IS_LONG) {
        return;
    }

    errstr = rd_kafka_err2str(Z_LVAL_P(zerr));

    if (errstr) {
        RETURN_STRING(errstr);
    }

    zpayload = kafka_read_property(NULL, Z_KAFKA_PROP_OBJ(getThis()), ZEND_STRL("payload"), 0);

    if (zpayload && Z_TYPE_P(zpayload) == IS_STRING) {
        RETURN_ZVAL(zpayload, 1, 0);
    }
}
/* }}} */

void kafka_message_init(INIT_FUNC_ARGS) { /* {{{ */
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "SimpleKafkaClient", "Message", class_Kafka_Message_methods);
    ce_kafka_message = zend_register_internal_class(&ce);

    zend_declare_property_null(ce_kafka_message, ZEND_STRL("err"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("topic_name"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("timestamp"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("partition"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("payload"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("len"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("key"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("offset"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(ce_kafka_message, ZEND_STRL("headers"), ZEND_ACC_PUBLIC);
} /* }}} */
