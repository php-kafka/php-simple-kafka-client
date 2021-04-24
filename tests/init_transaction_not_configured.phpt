--TEST--
initTransaction() not configured
--SKIPIF--
<?php
require __DIR__ . '/integration-tests-check.php';
?>
--FILE--
<?php
require __DIR__ . '/integration-tests-check.php';

$conf = new SimpleKafkaClient\Configuration();

$conf->set('metadata.broker.list', getenv('TEST_KAFKA_BROKERS'));

$producer = new SimpleKafkaClient\Producer($conf);

try {
    $producer->initTransactions(10000);
} catch (SimpleKafkaClient\KafkaErrorException $e) {
    echo $e->getMessage() . PHP_EOL;
    echo $e->getCode() . PHP_EOL;
    echo $e->getFile() . PHP_EOL;
    echo $e->getLine() . PHP_EOL;
}

--EXPECTF--
_NOT_CONFIGURED
-145
%s/tests/init_transaction_not_configured.php
11
