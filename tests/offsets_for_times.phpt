--TEST--
Produce, consume
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new Configuration();
$conf->set('client.id', 'pure-php-producer');
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$producer = new Producer($conf);
$topic = $producer->getTopicHandle('pure-php-test-topic-offsets');
$time = time();
$topic->producev(
    RD_KAFKA_PARTITION_UA,
    RD_KAFKA_MSG_F_BLOCK, // will block produce if queue is full
    'special-message',
    'special-key',
    [
        'special-header' => 'awesome'
    ]
);
$result = $producer->flush(20000);

$topicPartition = new TopicPartition('pure-php-test-topic-offsets', 0, $time);
$result = $producer->offsetsForTimes([$topicPartition], 10000);
var_dump($result[0]->getTopicName());
var_dump($result[0]->getPartition());
var_dump($result[0]->getOffset());
--EXPECT--
string(27) "pure-php-test-topic-offsets"
int(0)
int(0)
