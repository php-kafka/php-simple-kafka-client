<?php

/** @generate-function-entries */

namespace SimpleKafkaClient;

class TopicPartition
{
    public function __construct(string $topicName, int $partition, int $offset = 0, mixed $metadata = null) {}

    public function getTopicName(): ?string {}

    public function setTopicName(string $topicName): TopicPartition {}

    public function getPartition(): int {}

    public function setPartition(int $partition): TopicPartition {}

    public function getOffset(): int {}

    public function setOffset(int $offset): TopicPartition {}

    /** @param mixed $metadata */
    public function setMetadata($metadata): TopicPartition {}

    /** @return mixed */
    public function getMetadata() {}
}
