#!/bin/sh

set -ex

if ! [ -f ~/build-cache/librdkafka/usr/local/include/librdkafka/rdkafka.h ] || ! [ -f ~/build-cache/librdkafka/usr/local/bin/kafkacat ]; then
    echo "librdkafka build is not cached"

    git clone --depth 1 --branch "${LIBRDKAFKA_VERSION:-1.5.0}" "${LIBRDKAFKA_REPOSITORY_URL:-https://github.com/edenhill/librdkafka.git}"

    cd librdkafka
    ./configure
    make
    mkdir -p ~/build-cache/librdkafka
    sudo make install DESTDIR=$HOME/build-cache/librdkafka
    test -f ~/build-cache/librdkafka/usr/local/include/librdkafka/rdkafka.h || echo "librdkafka build failed"

    sudo rsync -a ~/build-cache/librdkafka/ /
    sudo ldconfig
    cd ..

    git clone --depth 1 --branch "1.6.0" "${LIBRDKAFKA_REPOSITORY_URL:-https://github.com/edenhill/kafkacat.git}"

    cd kafkacat
    ./configure
    make
    sudo make install DESTDIR=$HOME/build-cache/librdkafka

else
    echo "librdkafka build is cached"
fi

sudo rsync -av ~/build-cache/librdkafka/ /
sudo ldconfig
