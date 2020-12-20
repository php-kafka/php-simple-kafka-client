<?php

/** @generate-function-entries */

namespace Kafka;

abstract class Kafka
{
    public function getMetadata(bool $allTopics, int $timeoutMs, Topic $topic): Metadata {}

    public function getTopicHandle(string $topic): Topic {}

    public function getOutQLen(): int {}

    public function poll(int $timeoutMs): int {}

    public function queryWatermarkOffsets(string $topic, int $partition, int &$low, int &$high, int $timeoutMs): void {}

    public function offsetsForTimes(array $topicPartitions, int $timeoutMs): array {}
}
