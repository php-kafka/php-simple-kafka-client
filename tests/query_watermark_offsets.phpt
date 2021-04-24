--TEST--
Produce, consume
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new SimpleKafkaClient\Configuration();
$conf->set('client.id', 'pure-php-producer');
$conf->set('metadata.broker.list', 'kafka:9096');
$conf->set('compression.codec', 'snappy');
$conf->set('message.timeout.ms', '5000');

$producer = new SimpleKafkaClient\Producer($conf);
$topic = $producer->getTopicHandle('pure-php-test-topic-watermark');
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
$high = 0;
$low = 0;
$result = $producer->queryWatermarkOffsets('pure-php-test-topic-watermark', 0,$low, $high, 10000);
var_dump($low);
var_dump($high);
--EXPECT--
int(0)
int(1)
