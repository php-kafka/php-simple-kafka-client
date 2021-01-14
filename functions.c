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
#include "Zend/zend_exceptions.h"
#include "functions_arginfo.h"

/* {{{ proto array kafka_get_err_descs()
 * Returns the full list of error codes.
 */
ZEND_FUNCTION(kafka_get_err_descs)
{
    const struct rd_kafka_err_desc *errdescs;
    size_t cnt;
    size_t i;
    int seen_zero = 0;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    rd_kafka_get_err_descs(&errdescs, &cnt);

    array_init_size(return_value, cnt);

    for (i = 0; i < cnt; i++) {
        const struct rd_kafka_err_desc *desc = &errdescs[i];
        zval el;

        if (desc->code == 0) {
            if (seen_zero) {
                continue;
            }
            seen_zero = 1;
        }

        ZVAL_NULL(&el);
        array_init(&el);
        add_assoc_long(&el, "code", desc->code);
        if (desc->name) {
            add_assoc_string(&el, "name", (char*) desc->name);
        } else {
            add_assoc_null(&el, "name");
        }
        if (desc->desc) {
            add_assoc_string(&el, "desc", (char*) desc->desc);
        }else {
            add_assoc_null(&el, "desc");
        }
        add_next_index_zval(return_value, &el);
    }
}
/* }}} */

/* {{{ proto string kafka_err2name(int $err)
 * Returns a human readable representation of a kafka error.
 */
ZEND_FUNCTION(kafka_err2name)
{
    zend_long errorCode;
    const char *errname;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_LONG(errorCode)
	ZEND_PARSE_PARAMETERS_END();

    errname = rd_kafka_err2name(errorCode);

    if (errname) {
        RETURN_STRING(errname);
    }
}
/* }}} */

/* {{{ proto string kafka_err2str(int $err)
 * Returns a human readable representation of a kafka error.
 */
ZEND_FUNCTION(kafka_err2str)
{
    zend_long errorCode;
    const char *errstr;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_LONG(errorCode)
	ZEND_PARSE_PARAMETERS_END();

    errstr = rd_kafka_err2str(errorCode);

    if (errstr) {
        RETURN_STRING(errstr);
    }
}
/* }}} */

/* {{{ proto int kafka_thread_cnt()
 * Retrieve the current number of threads in use by librdkafka.
 */
ZEND_FUNCTION(kafka_thread_cnt)
{
	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    RETURN_LONG(rd_kafka_thread_cnt());
}
/* }}} */

/* {{{ proto int kafka_offset_tail(int $offset)
 * Start consuming `$cnt` messages from topic's current `.._END` offset.
 */
ZEND_FUNCTION(kafka_offset_tail)
{
    zend_long offset;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END();

    RETURN_LONG(RD_KAFKA_OFFSET_TAIL(offset));
}

