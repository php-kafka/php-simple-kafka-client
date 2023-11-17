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
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_simple_kafka_client_int.h"
#include "Zend/zend_exceptions.h"
#include "configuration_arginfo.h"

zend_class_entry * ce_kafka_conf;

static zend_object_handlers handlers;

static void kafka_conf_callback_dtor(kafka_conf_callback *cb) /* {{{ */
{
    if (cb) {
        zval_ptr_dtor(&cb->fci.function_name);
        efree(cb);
    }
} /* }}} */

void kafka_conf_callbacks_dtor(kafka_conf_callbacks *cbs) /* {{{ */
{
    kafka_conf_callback_dtor(cbs->error);
    cbs->error = NULL;
    kafka_conf_callback_dtor(cbs->rebalance);
    cbs->rebalance = NULL;
    kafka_conf_callback_dtor(cbs->dr_msg);
    cbs->dr_msg = NULL;
    kafka_conf_callback_dtor(cbs->stats);
    cbs->stats = NULL;
    kafka_conf_callback_dtor(cbs->offset_commit);
    cbs->offset_commit = NULL;
    kafka_conf_callback_dtor(cbs->log);
    cbs->log = NULL;
    kafka_conf_callback_dtor(cbs->oauthbearer_refresh);
    cbs->oauthbearer_refresh = NULL;
} /* }}} */

static void kafka_conf_callback_copy(kafka_conf_callback **to, kafka_conf_callback *from) /* {{{ */
{
    if (from) {
        *to = emalloc(sizeof(**to));
        **to = *from;
        zval_copy_ctor(&(*to)->fci.function_name);
    }
} /* }}} */

void kafka_conf_callbacks_copy(kafka_conf_callbacks *to, kafka_conf_callbacks *from) /* {{{ */
{
    kafka_conf_callback_copy(&to->error, from->error);
    kafka_conf_callback_copy(&to->rebalance, from->rebalance);
    kafka_conf_callback_copy(&to->dr_msg, from->dr_msg);
    kafka_conf_callback_copy(&to->stats, from->stats);
    kafka_conf_callback_copy(&to->offset_commit, from->offset_commit);
    kafka_conf_callback_copy(&to->log, from->log);
    kafka_conf_callback_copy(&to->oauthbearer_refresh, from->oauthbearer_refresh);
} /* }}} */

static void kafka_conf_free(zend_object *object) /* {{{ */
{
    kafka_conf_object *intern = php_kafka_from_obj(kafka_conf_object, object);

    if (intern->conf) {
        rd_kafka_conf_destroy(intern->conf);
    }
    kafka_conf_callbacks_dtor(&intern->cbs);

    zend_object_std_dtor(&intern->std);
}
/* }}} */

static zend_object *kafka_conf_new(zend_class_entry *class_type) /* {{{ */
{
    zend_object* retval;
    kafka_conf_object *intern;

    intern = ecalloc(1, sizeof(kafka_conf_object)+ zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    retval = &intern->std;
    retval->handlers = &handlers;

    return retval;
}
/* }}} */

kafka_conf_object * get_kafka_conf_object(zval *zconf)
{
    kafka_conf_object *oconf = Z_KAFKA_P(kafka_conf_object, zconf);

    if (!oconf->conf) {
        zend_throw_exception_ex(NULL, 0, "SimpleKafkaClient\\Configuration::__construct() has not been called");
        return NULL;
    }

    return oconf;
}

static void kafka_conf_error_cb(rd_kafka_t *rk, int err, const char *reason, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[3];

    if (!opaque) {
        return;
    }

    if (!cbs->error) {
        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    ZVAL_LONG(&args[1], err);
    ZVAL_STRING(&args[2], reason);

    kafka_call_function(&cbs->error->fci, &cbs->error->fcc, NULL, 3, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);
}

void kafka_conf_dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *msg, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[3];

    if (!opaque || !cbs->dr_msg) {
        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    kafka_message_new(&args[1], msg);
    if (NULL != msg->_private) {
        ZVAL_ZVAL(&args[2], msg->_private, 1, 0);
    }

    kafka_call_function(&cbs->dr_msg->fci, &cbs->dr_msg->fcc, NULL, 3, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);
}

static int kafka_conf_stats_cb(rd_kafka_t *rk, char *json, size_t json_len, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[3];

    if (!opaque) {
        return 0;
    }

    if (!cbs->stats) {
        return 0;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    ZVAL_STRING(&args[1], json);
    ZVAL_LONG(&args[2], json_len);

    kafka_call_function(&cbs->stats->fci, &cbs->stats->fcc, NULL, 3, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);

    return 0;
}

static void kafka_conf_rebalance_cb(rd_kafka_t *rk, rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *partitions, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[3];

    if (!opaque) {
        return;
    }

    if (!cbs->rebalance) {
        err = rd_kafka_assign(rk, NULL);

        if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
            zend_throw_exception(ce_kafka_exception, rd_kafka_err2str(err), err);
            return;
        }

        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    ZVAL_LONG(&args[1], err);
    kafka_topic_partition_list_to_array(&args[2], partitions);

    kafka_call_function(&cbs->rebalance->fci, &cbs->rebalance->fcc, NULL, 3, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);
}

static void kafka_conf_offset_commit_cb(rd_kafka_t *rk, rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *partitions, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[3];

    if (!opaque) {
        return;
    }

    if (!cbs->offset_commit) {
        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    ZVAL_LONG(&args[1], err);
    kafka_topic_partition_list_to_array(&args[2], partitions);

    kafka_call_function(&cbs->offset_commit->fci, &cbs->offset_commit->fcc, NULL, 3, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);
}

static void kafka_conf_log_cb(const rd_kafka_t *rk, int level, const char *facility, const char *message)
{
    zval args[4];

    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) rd_kafka_opaque(rk);

    if (!cbs->log) {
        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);
    ZVAL_NULL(&args[2]);
    ZVAL_NULL(&args[3]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    ZVAL_LONG(&args[1], level);
    ZVAL_STRING(&args[2], facility);
    ZVAL_STRING(&args[3], message);

    kafka_call_function(&cbs->log->fci, &cbs->log->fcc, NULL, 4, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
    zval_ptr_dtor(&args[2]);
    zval_ptr_dtor(&args[3]);
}

static void kafka_conf_oauthbearer_token_refresh_cb(rd_kafka_t *rk, const char *oauthbearer_config, void *opaque)
{
    kafka_conf_callbacks *cbs = (kafka_conf_callbacks*) opaque;
    zval args[2];

    if (!opaque) {
        return;
    }

    if (!cbs->oauthbearer_refresh) {
        return;
    }

    ZVAL_NULL(&args[0]);
    ZVAL_NULL(&args[1]);

    ZVAL_ZVAL(&args[0], &cbs->zrk, 1, 0);
    if (oauthbearer_config) {
        ZVAL_STRING(&args[1], oauthbearer_config);
    }

    kafka_call_function(&cbs->oauthbearer_refresh->fci, &cbs->oauthbearer_refresh->fcc, NULL, 2, args);

    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
}

/* {{{ proto SimpleKafkaClient\Configuration::__construct() */
ZEND_METHOD(SimpleKafkaClient_Configuration, __construct)
{
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    intern = Z_KAFKA_P(kafka_conf_object, getThis());
    intern->conf = rd_kafka_conf_new();
}
/* }}} */

/* {{{ proto array SimpleKafkaClient\Configuration::dump()
   Dump the configuration properties and values of `conf` to an array */
ZEND_METHOD(SimpleKafkaClient_Configuration, dump)
{
    size_t cntp;
    const char **dump;
    kafka_conf_object *intern;
    size_t i;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 0)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    dump = rd_kafka_conf_dump(intern->conf, &cntp);

    array_init(return_value);

    for (i = 0; i < cntp; i+=2) {
        const char *key = dump[i];
        const char *value = dump[i+1];
        add_assoc_string(return_value, (char*)key, (char*)value);
    }

    rd_kafka_conf_dump_free(dump, cntp);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::set(string $name, string $value)
   Sets a configuration property. */
ZEND_METHOD(SimpleKafkaClient_Configuration, set)
{
    char *name, *value;
    size_t name_len, value_len;
    kafka_conf_object *intern;
    rd_kafka_conf_res_t ret = 0;
    char errstr[512];

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
	    Z_PARAM_STRING(name, name_len)
	    Z_PARAM_STRING(value, value_len)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    errstr[0] = '\0';

    ret = rd_kafka_conf_set(intern->conf, name, value, errstr, sizeof(errstr));

    switch (ret) {
        case RD_KAFKA_CONF_UNKNOWN:
            zend_throw_exception(ce_kafka_exception, errstr, RD_KAFKA_CONF_UNKNOWN);
            return;
        case RD_KAFKA_CONF_INVALID:
            zend_throw_exception(ce_kafka_exception, errstr, RD_KAFKA_CONF_INVALID);
            return;
        case RD_KAFKA_CONF_OK:
            break;
    }
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setErrorCb(callable $callback)
   Sets the error callback */
ZEND_METHOD(SimpleKafkaClient_Configuration, setErrorCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.error) {
        zval_ptr_dtor(&intern->cbs.error->fci.function_name);
    } else {
        intern->cbs.error = ecalloc(1, sizeof(*intern->cbs.error));
    }

    intern->cbs.error->fci = fci;
    intern->cbs.error->fcc = fcc;

    rd_kafka_conf_set_error_cb(intern->conf, kafka_conf_error_cb);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setDrMsgCb(callable $callback)
   Sets the delivery report callback */
ZEND_METHOD(SimpleKafkaClient_Configuration, setDrMsgCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.dr_msg) {
        zval_ptr_dtor(&intern->cbs.dr_msg->fci.function_name);
    } else {
        intern->cbs.dr_msg = ecalloc(1, sizeof(*intern->cbs.dr_msg));
    }

    intern->cbs.dr_msg->fci = fci;
    intern->cbs.dr_msg->fcc = fcc;

    rd_kafka_conf_set_dr_msg_cb(intern->conf, kafka_conf_dr_msg_cb);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setStatsCb(callable $callback)
   Sets the statistics report callback */
ZEND_METHOD(SimpleKafkaClient_Configuration, setStatsCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.stats) {
        zval_ptr_dtor(&intern->cbs.stats->fci.function_name);
    } else {
        intern->cbs.stats = ecalloc(1, sizeof(*intern->cbs.stats));
    }

    intern->cbs.stats->fci = fci;
    intern->cbs.stats->fcc = fcc;

    rd_kafka_conf_set_stats_cb(intern->conf, kafka_conf_stats_cb);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setRebalanceCb(callback $callback)
   Set rebalance callback for use with coordinated consumer group balancing */
ZEND_METHOD(SimpleKafkaClient_Configuration, setRebalanceCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.rebalance) {
        zval_ptr_dtor(&intern->cbs.rebalance->fci.function_name);
    } else {
        intern->cbs.rebalance = ecalloc(1, sizeof(*intern->cbs.rebalance));
    }

    intern->cbs.rebalance->fci = fci;
    intern->cbs.rebalance->fcc = fcc;

    rd_kafka_conf_set_rebalance_cb(intern->conf, kafka_conf_rebalance_cb);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setOffsetCommitCb(callback $callback)
   Set offset commit callback for use with consumer groups */
ZEND_METHOD(SimpleKafkaClient_Configuration, setOffsetCommitCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.offset_commit) {
        zval_ptr_dtor(&intern->cbs.offset_commit->fci.function_name);
    } else {
        intern->cbs.offset_commit = ecalloc(1, sizeof(*intern->cbs.offset_commit));
    }

    intern->cbs.offset_commit->fci = fci;
    intern->cbs.offset_commit->fcc = fcc;

    rd_kafka_conf_set_offset_commit_cb(intern->conf, kafka_conf_offset_commit_cb);
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setLogCb(callback $callback)
   Set offset commit callback for use with consumer groups */
ZEND_METHOD(SimpleKafkaClient_Configuration, setLogCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *conf;
    char errstr[512];

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    conf = get_kafka_conf_object(getThis());
    if (!conf) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (conf->cbs.log) {
        zval_ptr_dtor(&conf->cbs.log->fci.function_name);
    } else {
        conf->cbs.log = ecalloc(1, sizeof(*conf->cbs.log));
    }

    conf->cbs.log->fci = fci;
    conf->cbs.log->fcc = fcc;

    rd_kafka_conf_set_log_cb(conf->conf, kafka_conf_log_cb);
    rd_kafka_conf_set(conf->conf, "log.queue", "true", errstr, sizeof(errstr));
}
/* }}} */

/* {{{ proto void SimpleKafkaClient\Configuration::setOAuthBearerTokenRefreshCb(callable $callback)
   Sets the OAuthBearer token refresh callback */
ZEND_METHOD(SimpleKafkaClient_Configuration, setOAuthBearerTokenRefreshCb)
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    kafka_conf_object *intern;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
	    Z_PARAM_FUNC(fci, fcc)
	ZEND_PARSE_PARAMETERS_END();

    intern = get_kafka_conf_object(getThis());
    if (!intern) {
        return;
    }

    Z_ADDREF_P(&fci.function_name);

    if (intern->cbs.oauthbearer_refresh) {
        zval_ptr_dtor(&intern->cbs.oauthbearer_refresh->fci.function_name);
    } else {
        intern->cbs.oauthbearer_refresh = ecalloc(1, sizeof(*intern->cbs.oauthbearer_refresh));
    }

    intern->cbs.oauthbearer_refresh->fci = fci;
    intern->cbs.oauthbearer_refresh->fcc = fcc;

    rd_kafka_conf_set_oauthbearer_token_refresh_cb(intern->conf, kafka_conf_oauthbearer_token_refresh_cb);
}
/* }}} */

void kafka_conf_init(INIT_FUNC_ARGS)
{
    zend_class_entry tmpce;

    handlers = kafka_default_object_handlers;
    handlers.free_obj = kafka_conf_free;
    handlers.offset = XtOffsetOf(kafka_conf_object, std);

    INIT_NS_CLASS_ENTRY(tmpce, "SimpleKafkaClient", "Configuration", class_SimpleKafkaClient_Configuration_methods);
    ce_kafka_conf = zend_register_internal_class(&tmpce);
    ce_kafka_conf->create_object = kafka_conf_new;
}
