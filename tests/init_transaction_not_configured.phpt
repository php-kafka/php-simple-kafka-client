--TEST--
initTransaction() not configured
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new Kafka\Configuration();
if (RD_KAFKA_VERSION >= 0x090000 && false !== getenv('TEST_KAFKA_BROKER_VERSION')) {
    $conf->set('broker.version.fallback', getenv('TEST_KAFKA_BROKER_VERSION'));
}

$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$producer = new Kafka\Producer($conf);

try {
    $producer->initTransactions(10000);
} catch (Kafka\KafkaErrorException $e) {
    echo $e->getMessage() . PHP_EOL;
    echo $e->getCode() . PHP_EOL;
    echo $e->getFile() . PHP_EOL;
    echo $e->getLine() . PHP_EOL;
}

--EXPECTF--
_NOT_CONFIGURED
-145
%s/tests/init_transaction_not_configured.php
14
