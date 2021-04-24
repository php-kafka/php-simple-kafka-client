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
 
#ifndef PHP_KAFKA_INT_H
#define PHP_KAFKA_INT_H

#include "librdkafka/rdkafka.h"

typedef struct _kafka_topic_object {
    rd_kafka_topic_t    *rkt;
    zval               zrk;
    zend_object         std;
} kafka_topic_object;

typedef struct _kafka_conf_callback {
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
} kafka_conf_callback;

typedef struct _kafka_conf_callbacks {
    zval zrk;
    kafka_conf_callback *error;
    kafka_conf_callback *rebalance;
    kafka_conf_callback *dr_msg;
    kafka_conf_callback *stats;
    kafka_conf_callback *consume;
    kafka_conf_callback *offset_commit;
    kafka_conf_callback *log;
} kafka_conf_callbacks;

typedef struct _kafka_conf_object {
    rd_kafka_conf_t         *conf;
    kafka_conf_callbacks cbs;
    zend_object                 std;
} kafka_conf_object;

typedef struct _kafka_topic_partition_intern {
    char        *topic;
    int32_t     partition;
    int64_t     offset;
    zend_object std;
} kafka_topic_partition_intern;

typedef struct _kafka_object {
    rd_kafka_type_t         type;
    rd_kafka_t              *rk;
    kafka_conf_callbacks    cbs;
	HashTable				topics;
    zend_object             std;
} kafka_object;

typedef void (*kafka_metadata_collection_ctor_t)(zval *renurn_value, zval *zmetadata, const void *object);


#if PHP_MAJOR_VERSION >= 8

#define Z_KAFKA_OBJ zend_object

#define Z_KAFKA_PROP_OBJ(object) Z_OBJ_P(object)

#define kafka_get_debug_object(type, object) php_kafka_from_obj(type, object)

#else // PHP 7

#define IS_MIXED 16

#define Z_KAFKA_OBJ zval

#define Z_KAFKA_PROP_OBJ(object) object

#define kafka_get_debug_object(type, object) get_object(object)

#define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, type_hint, allow_null, default_value) \
    ZEND_ARG_INFO(pass_by_ref, name)

#define Z_PARAM_ARRAY_HT_OR_NULL(dest) \
	Z_PARAM_ARRAY_HT_EX(dest, 1, 0)

#define Z_PARAM_LONG_OR_NULL(dest, is_null) \
	Z_PARAM_LONG_EX(dest, is_null, 1, 0)

#define Z_PARAM_OBJECT_OF_CLASS_OR_NULL(dest, _ce) \
	Z_PARAM_OBJECT_OF_CLASS_EX(dest, _ce, 1, 0)

#define Z_PARAM_STRING_OR_NULL(dest, dest_len) \
	Z_PARAM_STRING_EX(dest, dest_len, 1, 0)

#define Z_PARAM_ZVAL_OR_NULL(dest) \
	Z_PARAM_ZVAL_EX(dest, 1, 0)

#endif

#ifdef PHP_WIN32
#	define PHP_SIMPLE_KAFKA_CLIENT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIMPLE_KAFKA_CLIENT_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIMPLE_KAFKA_CLIENT_API
#endif

extern zend_class_entry * ce_kafka_conf;
extern zend_class_entry * ce_kafka_consumer;
extern zend_class_entry * ce_kafka_error_exception;
extern zend_class_entry * ce_kafka_exception;
extern zend_class_entry * ce_kafka_producer;
extern zend_class_entry * ce_kafka_message;
extern zend_class_entry * ce_kafka_consumer_topic;
extern zend_class_entry * ce_kafka_producer_topic;
extern zend_class_entry * ce_kafka_topic;
extern zend_class_entry * ce_kafka_topic_partition;
extern zend_module_entry simple_kafka_client_module_entry;
extern zend_object_handlers kafka_default_object_handlers;

#define Z_KAFKA_P(php_kafka_type, zobject) php_kafka_from_obj(php_kafka_type, Z_OBJ_P(zobject))

#define php_kafka_from_obj(php_kafka_type, object) \
    ((php_kafka_type*)((char *)(object) - XtOffsetOf(php_kafka_type, std)))

#define phpext_kafka_ptr &simple_kafka_client_module_entry

#define PHP_SIMPLE_KAFKA_CLIENT_VERSION "0.1.2"


static inline void kafka_call_function(zend_fcall_info *fci, zend_fcall_info_cache *fci_cache, zval *retval, uint32_t param_count, zval params[])
{
    int local_retval;
    zval local_retval_zv;

    if (retval) {
        local_retval = 0;
    } else {
        local_retval = 1;
        retval = &local_retval_zv;
    }

    fci->retval = retval;
    fci->params = params;
    fci->param_count = param_count;

    zend_call_function(fci, fci_cache);

    if (local_retval) {
        zval_ptr_dtor(retval);
    }
}

static inline zval *kafka_read_property(zend_class_entry *scope, Z_KAFKA_OBJ *object, const char *name, size_t name_length, zend_bool silent)
{
    zval rv;
    return zend_read_property(scope, object, name, name_length, silent, &rv);
}


static inline char *kafka_hash_get_current_key_ex(HashTable *ht, HashPosition *pos)
{
    zend_string* key;
    zend_ulong index;

    if (zend_hash_get_current_key_ex(ht, &key, &index, pos) == HASH_KEY_IS_STRING) {
        return key->val;
    }

    return NULL;
}

void kafka_error_init();
void create_kafka_error(zval *return_value, const rd_kafka_error_t *error);
void kafka_conf_init(INIT_FUNC_ARGS);
void kafka_conf_callbacks_dtor(kafka_conf_callbacks *cbs);
void kafka_conf_callbacks_copy(kafka_conf_callbacks *to, kafka_conf_callbacks *from);
void kafka_message_init(INIT_FUNC_ARGS);
void kafka_message_new(zval *return_value, const rd_kafka_message_t *message);
void kafka_message_list_to_array(zval *return_value, rd_kafka_message_t **messages, long size);
void kafka_metadata_init(INIT_FUNC_ARGS);
void kafka_metadata_obj_init(zval *return_value, const rd_kafka_metadata_t *metadata);
void kafka_metadata_broker_init(INIT_FUNC_ARGS);
void kafka_metadata_broker_ctor(zval *return_value, zval *zmetadata, const void *metadata_broker);
void kafka_metadata_collection_init(INIT_FUNC_ARGS);
void kafka_metadata_collection_obj_init(zval *return_value, Z_KAFKA_OBJ *zmetadata, const void * items, size_t item_cnt, size_t item_size, kafka_metadata_collection_ctor_t ctor);
void kafka_metadata_partition_init(INIT_FUNC_ARGS);
void kafka_metadata_partition_ctor(zval *return_value, zval *zmetadata, const void *metadata_partition);
void kafka_metadata_topic_init(INIT_FUNC_ARGS);
void kafka_metadata_topic_ctor(zval *return_value, zval *zmetadata, const void *metadata_topic);
void kafka_topic_init(INIT_FUNC_ARGS);
void kafka_metadata_topic_partition_init(INIT_FUNC_ARGS);
void kafka_topic_partition_init(zval *z, char *topic, int32_t partition, int64_t offset);
void kafka_topic_partition_list_to_array(zval *return_value, rd_kafka_topic_partition_list_t *list);

kafka_topic_partition_intern * get_topic_partition_object(zval *z);
rd_kafka_topic_partition_list_t * array_arg_to_kafka_topic_partition_list(int argnum, HashTable *ary);
kafka_topic_object * get_kafka_topic_object(zval *zrkt);
kafka_conf_object * get_kafka_conf_object(zval *zconf);
kafka_object * get_kafka_object(zval *zrk);

ZEND_METHOD(Kafka, __construct);

#endif /* PHP_KAFKA_INT_H */
