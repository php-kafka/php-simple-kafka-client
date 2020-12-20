--TEST--
TopicPartition destruct for high level consumer
--FILE--
<?php
use Kafka\Configuration;
use Kafka\Consumer;

$conf = new Configuration();
$conf->set('group.id','test');
$consumer = new Consumer($conf);
$topic = $consumer->getTopicHandle('test');
unset($topic);
var_dump(isset($topic));
--EXPECT--
bool(false)
