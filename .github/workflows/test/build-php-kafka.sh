#!/bin/sh

set -e

echo "Building php-kafka-client with PHP version:"
php --version

if [ $MEMORY_CHECK -eq 1 ]; then
    PHP_KAFKA_CLIENT_CFLAGS="-Wall -Werror -Wno-deprecated-declarations"
fi

cd php-kafka
phpize
CFLAGS="PHP_KAFKA_CLIENT_CFLAGS" ./configure
make

echo "extension=$(pwd)/modules/kafka_client.so"|sudo tee /usr/local/etc/php/kafka_client.ini >/dev/null
