#!/bin/sh

set -e

echo "Building php-kafka with PHP version:"
php --version

if [ $MEMORY_CHECK -eq 1 ]; then
    PHP_KAFKA_CFLAGS="-Wall -Werror -Wno-deprecated-declarations"
fi

cd php-kafka
phpize
CFLAGS="$PHP_KAFKA_CFLAGS" ./configure
make

echo "extension=$(pwd)/modules/kafka.so"|sudo tee /usr/local/etc/php/kafka.ini >/dev/null
