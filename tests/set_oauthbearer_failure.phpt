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
$producer->setOAuthBearerTokenFailure('something');
$producer->poll(-1);
--EXPECT--
string(51) "Failed to acquire SASL OAUTHBEARER token: something"
