--TEST--
constants
--FILE--
<?php
var_dump(kafka_err2str(RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION));
var_dump(kafka_err2name(RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION));
--EXPECT--
string(24) "Local: Unknown partition"
string(18) "_UNKNOWN_PARTITION"
