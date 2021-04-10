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
#include "php_simple_kafka_client_int.h"
#include "ext/spl/spl_iterators.h"
#include "Zend/zend_interfaces.h"
#include "metadata_collection_arginfo.h"
#include "Zend/zend_exceptions.h"

typedef struct _object_intern {
    zval                             zmetadata;
    const void                       *items;
    size_t                           item_cnt;
    size_t                           item_size;
    size_t                           position;
    kafka_metadata_collection_ctor_t ctor;
    zend_object                      std;
} object_intern;

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp);

static zend_class_entry *ce;
static zend_object_handlers handlers;

static void free_object(zend_object *object) /* {{{ */
{
    object_intern *intern = php_kafka_from_obj(object_intern, object);

    if (intern->items) {
        zval_dtor(&intern->zmetadata);
    }

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *create_object(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    object_intern *intern;

    intern = ecalloc(1, sizeof(object_intern)+ zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &handlers;

    return retval;
}
/* }}} */

static object_intern * get_object(zval *zmti)
{
    object_intern *omti = Z_KAFKA_P(object_intern, zmti);

    if (!omti->items) {
        zend_throw_exception_ex(NULL, 0, "SimpleKafkaClient\\Metadata\\Collection::__construct() has not been called");
        return NULL;
    }

    return omti;
}

static HashTable *get_debug_info(Z_KAFKA_OBJ *object, int *is_temp) /* {{{ */
{
    zval ary;
    object_intern *intern;
    size_t i;
    zval item;

    *is_temp = 1;

    array_init(&ary);

    intern = kafka_get_debug_object(object_intern, object);
    if (!intern) {
        return Z_ARRVAL(ary);
    }
    
    for (i = 0; i < intern->item_cnt; i++) {
        ZVAL_NULL(&item);
        intern->ctor(&item, &intern->zmetadata, (char *)intern->items + i * intern->item_size);
        add_next_index_zval(&ary, &item);
    }

    return Z_ARRVAL(ary);
}
/* }}} */

/* {{{ proto int SimpleKafkaClient\Metadata\Collection::count()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, count)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_LONG(intern->item_cnt);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Metadata\Collection::rewind()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, rewind)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    intern->position = 0;
}
/* }}} */

/* {{{ proto mixed SimpleKafkaClient\Metadata\Collection::current()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, current)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (intern->position >= intern->item_cnt) {
        zend_throw_exception(ce_kafka_exception, "Called current() on invalid iterator", 0);
        return;
    }

    intern->ctor(return_value, &intern->zmetadata, (char *)intern->items + intern->position * intern->item_size);
}
/* }}} */

/* {{{ proto mixed SimpleKafkaClient\Metadata\Collection::key()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, key)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    if (intern->position >= intern->item_cnt) {
        zend_throw_exception(ce_kafka_exception, "Called key() on invalid iterator", 0);
        return;
    }

    RETURN_LONG(intern->position);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Metadata\Collection::next()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, next)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    intern->position++;
}
/* }}} */

/* {{{ proto bool SimpleKafkaClient\Metadata\Collection::valid()
   */
ZEND_METHOD(SimpleKafkaClient_Metadata_Collection, valid)
{
    object_intern *intern;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
    ZEND_PARSE_PARAMETERS_END();

    intern = get_object(getThis());
    if (!intern) {
        return;
    }

    RETURN_BOOL(intern->position < intern->item_cnt);
}
/* }}} */

void kafka_metadata_collection_init(INIT_FUNC_ARGS)
{
    zend_class_entry tmpce;

    INIT_NS_CLASS_ENTRY(tmpce, "SimpleKafkaClient\\Metadata", "Collection", class_SimpleKafkaClient_Metadata_Collection_methods);
    ce = zend_register_internal_class(&tmpce);
    ce->create_object = create_object;
    zend_class_implements(ce, 2, spl_ce_Countable, spl_ce_Iterator);

    handlers = kafka_default_object_handlers;
    handlers.get_debug_info = get_debug_info;
    handlers.free_obj = free_object;
    handlers.offset = XtOffsetOf(object_intern, std);
}

void kafka_metadata_collection_obj_init(zval *return_value, Z_KAFKA_OBJ *zmetadata, const void * items, size_t item_cnt, size_t item_size, kafka_metadata_collection_ctor_t ctor)
{
    object_intern *intern;

    if (object_init_ex(return_value, ce) != SUCCESS) {
        return;
    }

    intern = Z_KAFKA_P(object_intern, return_value);
    if (!intern) {
        return;
    }

#if PHP_MAJOR_VERSION < 8
    ZVAL_ZVAL(&intern->zmetadata, zmetadata, 1, 0);
#endif
    intern->items = items;
    intern->item_cnt = item_cnt;
    intern->item_size = item_size;
    intern->ctor = ctor;
    intern->position = 0;
}
