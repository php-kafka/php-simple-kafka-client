--TEST--
KafkaErrorException
--FILE--
<?php

$e = new Kafka\KafkaErrorException('exception message', -100, 'exception description', true, false, true);

echo sprintf('Exception message: %s', $e->getMessage()) . PHP_EOL;
echo sprintf('Exception code: %d', $e->getCode()) . PHP_EOL;
echo sprintf('Exception description: %s', $e->getErrorString()) . PHP_EOL;
echo sprintf('Exception is fatal: %b', $e->isFatal()) . PHP_EOL;
echo sprintf('Exception is retriable: %b', $e->isRetriable()) . PHP_EOL;
echo sprintf('Exception requires transaction abort: %b', $e->transactionRequiresAbort()) . PHP_EOL;
--EXPECT--
Exception message: exception message
Exception code: -100
Exception description: exception description
Exception is fatal: 1
Exception is retriable: 0
Exception requires transaction abort: 1
