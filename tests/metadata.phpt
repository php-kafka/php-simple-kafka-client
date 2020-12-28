--TEST--
Metadata
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$delivered = 0;

$conf = new Kafka\Configuration();
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

$producer = new Kafka\Producer($conf);

$topicName = "test_kafka_metadata";

$topic = $producer->getTopicHandle($topicName);

if (!$producer->getMetadata(false, 2*1000, $topic)) {
    echo "Failed to get metadata, is broker down?\n";
}

$topic->produce(0, 0, "some test mesage");

$producer->flush(10000);
printf("%d messages delivered\n", $delivered);
$metadata = $producer->getMetadata(false, 2*1000, $topic);
echo $metadata->getOrigBrokerName() . PHP_EOL;

$topics = $metadata->getTopics();

while ($topics->valid()) {
    echo $topics->current()->getTopic() . PHP_EOL;
    echo $topics->current()->getPartitions()->count() . PHP_EOL;
    echo $topics->current()->getPartitions()->current()->getLeader() . PHP_EOL;
    $topics->next();
}

echo $metadata->getBrokers()->current()->getHost() . PHP_EOL;
echo $metadata->getBrokers()->current()->getPort() . PHP_EOL;
--EXPECT--
1 messages delivered
kafka:9092/1
test_kafka_metadata
1
1
kafka
9092


