#!/bin/sh

set -e

echo "Building php-simple-kafka-client with PHP version:"
php --version

if [ $MEMORY_CHECK -eq 1 ]; then
    PHP_SIMPLE_KAFKA_CLIENT_CFLAGS="-Wall -Werror -Wno-deprecated-declarations"
fi

cd php-simple-kafka-client
phpize
CFLAGS="$PHP_SIMPLE_KAFKA_CLIENT_CFLAGS" ./configure
make

echo "extension=$(pwd)/modules/simple_kafka_client.so"|sudo tee /usr/local/etc/php/simple_kafka_client.ini >/dev/null
