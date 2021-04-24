--TEST--
TopicPartition destruct for high level consumer
--FILE--
<?php
use SimpleKafkaClient\Configuration;
use SimpleKafkaClient\Consumer;

$conf = new Configuration();
$conf->set('group.id','test');
$consumer = new Consumer($conf);
$topic = $consumer->getTopicHandle('test');
unset($topic);
var_dump(isset($topic));
--EXPECT--
bool(false)
