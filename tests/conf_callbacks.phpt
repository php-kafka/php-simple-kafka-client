--TEST--
Kafka\Configuration
--SKIPIF--
--FILE--
<?php

$conf = new Kafka\Configuration();

echo "Setting consume callback\n";
$conf->setConsumeCb(function () { });
$dump = $conf->dump();
var_dump(isset($dump["consume_cb"]));

echo "Setting offset_commit callback\n";
$conf->setOffsetCommitCb(function () { });
$dump = $conf->dump();
var_dump(isset($dump["offset_commit_cb"]));

echo "Setting rebalance callback\n";
$conf->setRebalanceCb(function () { });
$dump = $conf->dump();
var_dump(isset($dump["rebalance_cb"]));


--EXPECT--
Setting consume callback
bool(true)
Setting offset_commit callback
bool(true)
Setting rebalance callback
bool(true)
