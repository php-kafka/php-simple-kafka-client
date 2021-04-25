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
$conf->set('sasl.oauthbearer.config', 'principalClaimName=azp');
$conf->setOAuthBearerTokenRefreshCb(function($kafka, $oAuthBearerConfig) {
    var_dump($oAuthBearerConfig);
});

$conf->setErrorCb(function($kafka, $errorCode, $errorString) {
    var_dump($errorString);
});

$producer = new SimpleKafkaClient\Producer($conf);
$producer->poll(-1);
--EXPECT--
string(22) "principalClaimName=azp"
