--TEST--
Allow null payload
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new SimpleKafkaClient\Configuration();
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$topicName = sprintf('test_kafka_%s', uniqid());

$producer = new SimpleKafkaClient\Producer($conf);
$topic = $producer->getTopicHandle($topicName);

$topic->produce(0, 0, NULL, 'message_key_1');

$producer->flush(10000);

$conf->set('group.id','test');
$conf->set('auto.offset.reset','earliest');
$conf->setErrorCb(function ($producer, $errorCode, $errstr) {
    // non fatal errors are retried by librdkafka
    if (RD_KAFKA_RESP_ERR__FATAL !== $errorCode) {
        return;
    }

    printf("%s: %s\n", rd_kafka_err2str($errorCode), $errstr);
    exit;
});
$consumer = new SimpleKafkaClient\Consumer($conf);

$consumer->subscribe([$topicName]);

while (true) {
    $message = $consumer->consume(1000);
    if ($message === null) {
        continue;
    }

    if (RD_KAFKA_RESP_ERR_NO_ERROR === $message->err) {
        var_dump($message->payload);
        var_dump($message->key);
        break;
    }
}

--EXPECTF--
NULL
string(13) "message_key_1"
