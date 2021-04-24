--TEST--
Produce, consume
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new SimpleKafkaClient\Configuration();
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$conf->setDrMsgCb(function (SimpleKafkaClient\Producer $kafka, SimpleKafkaClient\Message $message, $opaque) {
    if (RD_KAFKA_RESP_ERR_NO_ERROR !== $message->err) {
        $errorStr = rd_kafka_err2str($message->err);

        echo sprintf('Message FAILED (%s, %s) to send with payload => %s', $message->err, $errorStr, $message->payload) . PHP_EOL;
    } else {
        if (false === is_string($opaque)) {
            $opaque = 'opaque was already freed';
        }

        echo sprintf('Message opaque: %s', $opaque) . PHP_EOL;
    }
});

$producer = new SimpleKafkaClient\Producer($conf);
$topic = $producer->getTopicHandle('pure-php-test-topic');
$amountTestMessages = 10;

for ($i = 0; $i < $amountTestMessages; ++$i) {
    $topic->producev(
        RD_KAFKA_PARTITION_UA,
        RD_KAFKA_MSG_F_BLOCK, // will block produce if queue is full
        sprintf('test message-%d',$i),
        sprintf('test-key-%d', $i),
        [
            'some' => sprintf('header value %d', $i)
        ],
        null,
        "opaque $i"
    );

    $producer->poll(0);
}

$result = $producer->flush(20000);
if (RD_KAFKA_RESP_ERR_NO_ERROR !== $result) {
    echo 'Was not able to shutdown within 20s. Messages might be lost!' . PHP_EOL;
}
--EXPECT--
Message key test-key-0 and opaque: opaque 0
Message key test-key-1 and opaque: opaque 1
Message key test-key-2 and opaque: opaque 2
Message key test-key-3 and opaque: opaque 3
Message key test-key-4 and opaque: opaque 4
Message key test-key-5 and opaque: opaque 5
Message key test-key-6 and opaque: opaque 6
Message key test-key-7 and opaque: opaque 7
Message key test-key-8 and opaque: opaque 8
Message key test-key-9 and opaque: opaque 9
