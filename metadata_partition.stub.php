<?php

/** @generate-function-entries */

namespace Kafka\Metadata;

class Partition
{
    public function getId(): int {}

    public function getErrorCode(): int {}

    public function getLeader(): int {}

    public function getReplicas(): Collection {}

    public function getIsrs(): Collection {}
}
