--TEST--
Produce, consume
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
if (!class_exists("SimpleKafkaClient\\KafkaErrorException")) {
    echo "skip";
}
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$delivered = 0;

$conf = new SimpleKafkaClient\Configuration();
$conf->set('transactional.id', 'transactional-producer');

$conf->setLogCb(function ($kafka, $level, $facility, $message) {});
$conf->setErrorCb(function ($producer, $errorCode, $errstr) {
    printf("%s: %s\n", rd_kafka_err2str($errorCode), $errstr);
    exit;
});
$conf->setDrMsgCb(function ($producer, $msg) use (&$delivered) {
    if ($msg->err) {
        throw new Exception("Message delivery failed: " . $msg->getErrorString());
    }
    $delivered++;
});
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$producer = new SimpleKafkaClient\Producer($conf);

$producer->initTransactions(10000);
$producer->beginTransaction();

$topicName = sprintf("test_kafka_%s", uniqid());

$topic = $producer->getTopicHandle($topicName);

if (!$producer->getMetadata(false, 5*1000, $topic)) {
    echo "Failed to get metadata, is broker down?\n";
}

for ($i = 0; $i < 10; $i++) {
    $topic->produce(0, 0, "message $i");
    $producer->poll(0);
}

$producer->flush(10000);

$producer->commitTransaction(10000);

printf("%d messages delivered\n", $delivered);

$conf = new SimpleKafkaClient\Configuration();
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));
$conf->setErrorCb(function ($producer, $errorCode, $errstr) {
    // non fatal errors are retried by librdkafka
    if (RD_KAFKA_RESP_ERR__FATAL !== $errorCode) {
        return;
    }

    printf("%s: %s\n", rd_kafka_err2str($errorCode), $errstr);
    exit;
});
$conf->set('group.id','test');
$conf->set('auto.offset.reset','earliest');
$conf->set('enable.partition.eof', 'true');
$consumer = new SimpleKafkaClient\Consumer($conf);
$consumer->subscribe([$topicName]);

$messages = [];

while (true) {
    $msg = $consumer->consume(10000);
    // librdkafka before 1.0 returns message with RD_KAFKA_RESP_ERR__PARTITION_EOF when reaching topic end.
    if (!$msg || $msg->err === RD_KAFKA_RESP_ERR__PARTITION_EOF) {
        break;
    }

    if (RD_KAFKA_RESP_ERR_NO_ERROR !== $msg->err) {
        throw new Exception($msg->getErrorString(), $msg->err);
    }

    printf("Got message: %s\n", $msg->payload);
}
--EXPECT--
10 messages delivered
Got message: message 0
Got message: message 1
Got message: message 2
Got message: message 3
Got message: message 4
Got message: message 5
Got message: message 6
Got message: message 7
Got message: message 8
Got message: message 9
