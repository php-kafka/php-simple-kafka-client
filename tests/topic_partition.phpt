--TEST--
SimpleKafkaClient\TopicPartition
--FILE--
<?php

$topar = new SimpleKafkaClient\TopicPartition("test", RD_KAFKA_PARTITION_UA);

var_dump($topar);

$topar = new SimpleKafkaClient\TopicPartition("test", RD_KAFKA_PARTITION_UA, 42);

var_dump($topar);

$topar = new SimpleKafkaClient\TopicPartition("test", RD_KAFKA_PARTITION_UA, 42, ['test']);

var_dump($topar);

var_dump(array(
    "topic" => $topar->getTopicName(),
    "partition" => $topar->getPartition(),
    "offset" => $topar->getOffset(),
    "metadata" => $topar->getMetadata(),
));

$topar
    ->setTopicName("foo")
    ->setPartition(123)
    ->setOffset(43)
    ->setMetadata(2);

var_dump($topar);
--EXPECT--
object(SimpleKafkaClient\TopicPartition)#1 (4) {
  ["topic"]=>
  string(4) "test"
  ["partition"]=>
  int(-1)
  ["offset"]=>
  int(0)
  ["metadata"]=>
  NULL
}
object(SimpleKafkaClient\TopicPartition)#2 (4) {
  ["topic"]=>
  string(4) "test"
  ["partition"]=>
  int(-1)
  ["offset"]=>
  int(42)
  ["metadata"]=>
  NULL
}
object(SimpleKafkaClient\TopicPartition)#1 (4) {
  ["topic"]=>
  string(4) "test"
  ["partition"]=>
  int(-1)
  ["offset"]=>
  int(42)
  ["metadata"]=>
  array(1) {
    [0]=>
    string(4) "test"
  }
}
array(4) {
  ["topic"]=>
  string(4) "test"
  ["partition"]=>
  int(-1)
  ["offset"]=>
  int(42)
  ["metadata"]=>
  array(1) {
    [0]=>
    string(4) "test"
  }
}
object(SimpleKafkaClient\TopicPartition)#1 (4) {
  ["topic"]=>
  string(3) "foo"
  ["partition"]=>
  int(123)
  ["offset"]=>
  int(43)
  ["metadata"]=>
  int(2)
}
