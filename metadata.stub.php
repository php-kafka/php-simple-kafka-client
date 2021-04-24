<?php

/** @generate-function-entries */

namespace SimpleKafkaClient;

class Metadata
{
    public function getOrigBrokerId(): int {}

    public function getOrigBrokerName(): string {}

    public function getBrokers(): Metadata\Collection {}

    public function getTopics(): Metadata\Collection {}
}
