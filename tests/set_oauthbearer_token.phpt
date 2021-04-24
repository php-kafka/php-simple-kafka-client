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
$conf->set('security.protocol', 'SASL_PLAINTEXT');
$conf->set('sasl.mechanisms', 'OAUTHBEARER');

$conf->setErrorCb(function($kafka, $errorCode, $errorString) {
    var_dump($errorString);
});

$producer = new SimpleKafkaClient\Producer($conf);
$producer->setOAuthBearerToken('token', 100000 + time() * 1000, 'principal', ['test'=>'key']);
$producer->poll(-1);
echo 'Done';
--EXPECT--
Done
