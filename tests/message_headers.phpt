--TEST--
Message headers
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$delivered = 0;

$conf = new SimpleKafkaClient\Configuration();
$conf->setErrorCb(function ($producer, $errorCode, $errstr) {
    printf("%s: %s\n", rd_kafka_err2str($errorCode), $errstr);
    exit;
});
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$conf->setDrMsgCb(function ($producer, $msg) use (&$delivered) {
    if ($msg->err) {
        throw new Exception("Message delivery failed: " . $msg->getErrorString());
    }
    $delivered++;
});

$producer = new SimpleKafkaClient\Producer($conf);

$topicName = sprintf("test_kafka_%s", uniqid());

$topic = $producer->getTopicHandle($topicName);

if (!$producer->getMetadata(false, 2*1000, $topic)) {
    echo "Failed to get metadata, is broker down?\n";
}

$headers = [
    ['key' => 'value'],
    [
        'key1' => 'value1',
        'key2' => 'value2',
        'key3' => 'value3',
    ],
    ['gzencoded' => gzencode('gzdata')],
    [],
    null,
    ['key'],
];

foreach ($headers as $index => $header) {
    $topic->producev(0, 0, "message $index", null, $header);
    $producer->poll(0);
}

$producer->flush(10000);

printf("%d messages delivered\n", $delivered);

$conf = new SimpleKafkaClient\Configuration();
$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));
$conf->set('group.id','test');
$conf->set('auto.offset.reset','earliest');
$conf->set('enable.partition.eof', 'true');
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

$messages = [];

while (true) {
    $msg = $consumer->consume(10000);
    if (!$msg || $msg->err === RD_KAFKA_RESP_ERR__PARTITION_EOF) {
        break;
    }

    if (RD_KAFKA_RESP_ERR_NO_ERROR !== $msg->err) {
        throw new Exception($msg->getErrorString(), $msg->err);
    }

    $headersString = isset($msg->headers) ? $msg->headers : [];
    array_walk($headersString, function(&$value, $key) {
        if ('gzencoded' === $key) {
            $value = gzdecode($value);
        }
        $value = "{$key}: {$value}";
    });
    if (empty($headersString)) {
        $headersString = "none";
    } else {
        $headersString = implode(", ", $headersString);
    }
    printf("Got message: %s | Headers: %s\n", $msg->payload, $headersString);
}
--EXPECT--
6 messages delivered
Got message: message 0 | Headers: key: value
Got message: message 1 | Headers: key1: value1, key2: value2, key3: value3
Got message: message 2 | Headers: gzencoded: gzdata
Got message: message 3 | Headers: none
Got message: message 4 | Headers: none
Got message: message 5 | Headers: none
