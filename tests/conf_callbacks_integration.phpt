--TEST--
Kafka\Configuration
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new Kafka\Configuration();
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$delivered = 0;
$conf->setDrMsgCb(function (Kafka\Producer $producer, Kafka\Message $message) use (&$delivered) {
    if (RD_KAFKA_RESP_ERR_NO_ERROR !== $message->err) {
        $errorStr = rd_kafka_err2str($message->err);

        echo sprintf('Message FAILED (%s, %s) to send with payload => %s', $message->err, $errorStr, $message->payload) . PHP_EOL;
    } else {
        // message successfully delivered
        ++$delivered;
    }
});

$producer = new Kafka\Producer($conf);

$topicName = sprintf("test_kafka_%s", uniqid());
$topic = $producer->getTopicHandle($topicName);

for ($i = 0; $i < 10; $i++) {
    $topic->produce(0, 0, "message $i");
}

$producer->flush(10000);

$conf = new Kafka\Configuration();
$conf->set('auto.offset.reset', 'earliest');
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));
$conf->set('group.id', sprintf("test_kafka_group_%s", uniqid()));
$conf->set('enable.partition.eof', 'true');
$conf->set('statistics.interval.ms', 10);
$conf->set('log_level', (string) LOG_DEBUG);
$conf->set('debug', 'all');

$conf->setOffsetCommitCb(function ($consumer, $error, $topicPartitions) {
    echo "Offset " . $topicPartitions[0]->getOffset() . " committed.\n";
});

$statsCbCalled = false;
$conf->setStatsCb(function ($consumer, $json) use (&$statsCbCalled) {
    if ($statsCbCalled) {
        return;
    }

    $statsCbCalled = true;
});

$logCbCalled = false;
$conf->setLogCb(function (Kafka\Consumer $consumer, int $level, string $facility, string $message) use (&$logCbCalled) {
    // suppress current bug in librdkafka https://github.com/edenhill/librdkafka/issues/2767
    $logCbCalled = true ;
});

$conf->setErrorCb(function ($kafka, int $errorCode, string $reason) {
    // suppress current bug in librdkafka https://github.com/edenhill/librdkafka/issues/2767
});

$consumer = new Kafka\Consumer($conf);
$consumer->subscribe([$topicName]);

while (true) {
    $msg = $consumer->consume(15000);

    if (RD_KAFKA_RESP_ERR__PARTITION_EOF === $msg->err) {
        break;
    }

    if (RD_KAFKA_RESP_ERR_NO_ERROR !== $msg->err) {
        throw new Exception($msg->getErrorString(), $msg->err);
    }

    $consumer->commit($msg);
}

var_dump($statsCbCalled);
var_dump($logCbCalled);
var_dump($delivered);

--EXPECT--
Offset 1 committed.
Offset 2 committed.
Offset 3 committed.
Offset 4 committed.
Offset 5 committed.
Offset 6 committed.
Offset 7 committed.
Offset 8 committed.
Offset 9 committed.
Offset 10 committed.
bool(true)
bool(true)
int(10)
