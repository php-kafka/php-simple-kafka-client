<?php

/** @generate-function-entries */

namespace SimpleKafkaClient;

abstract class Kafka
{
    public function getMetadata(bool $allTopics, int $timeoutMs, Topic $topic): Metadata {}

    public function getOutQLen(): int {}

    public function queryWatermarkOffsets(string $topic, int $partition, int &$low, int &$high, int $timeoutMs): void {}

    public function offsetsForTimes(array $topicPartitions, int $timeoutMs): array {}

    public function setOAuthBearerTokenFailure(string $errorString): void {}

    public function setOAuthBearerToken(string $token, int $lifetimeMs, string $principalName, ?array $extensions = null): void {}
}
