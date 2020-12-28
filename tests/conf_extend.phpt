--TEST--
Kafka\Configuration
--FILE--
<?php

class TestBug extends Kafka\Configuration
{
    public function set(string $name, string $value): void
    {
        parent::set($name, $value);
    }
}

$conf = new TestBug();
$conf->set('metadata.broker.list', '127.0.0.1');

echo "done" . PHP_EOL;
--EXPECT--
done
