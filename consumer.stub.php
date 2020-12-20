<?php

/** @generate-function-entries */

namespace Kafka;

class Consumer
{
    public function __construct(Configuration $configuration) {}

    public function assign(array $topics): void {}

    public function getAssignment(): array {}

    public function subscribe(array $topics): void {}

    public function getSubscription(): array {}

    public function unsubscribe(): void {}

    public function consume(int $timeoutMs): Message {}

    /** @param Message|array $messageOrOffsets */
    public function commit($messageOrOffsets): void {}

    /** @param Message|array $messageOrOffsets */
    public function commitAsync($messageOrOffsets): void {}

    public function close(): void {}

    public function getMetadata(bool $allTopics, int $timeoutMs, ConsumerTopic $topic): Metadata {}

    public function getTopicHandle(string $topic): ConsumerTopic {}

    public function getCommittedOffsets(array $topics, int $timeoutMs): array {}

    public function getOffsetPositions(array $topics): array {}

    public function offsetsForTimes(array $topicPartitions, int $timeoutMs): array {}

    public function queryWatermarkOffsets(string $topic, int $partition, int &$low, int &$high, int $timeoutMs): void {}
}
