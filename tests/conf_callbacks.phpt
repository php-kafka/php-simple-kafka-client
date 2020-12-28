--TEST--
Kafka\Configuration
--SKIPIF--
--FILE--
<?php

$conf = new Kafka\Configuration();

echo "Setting offset_commit callback\n";
$conf->setOffsetCommitCb(function () { });
$dump = $conf->dump();
var_dump(isset($dump["offset_commit_cb"]));

echo "Setting rebalance callback\n";
$conf->setRebalanceCb(function () { });
$dump = $conf->dump();
var_dump(isset($dump["rebalance_cb"]));


--EXPECT--
Setting offset_commit callback
bool(true)
Setting rebalance callback
bool(true)
