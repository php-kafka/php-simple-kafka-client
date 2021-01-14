dnl $Id$
dnl config.m4 for extension simple_kafka_client

PHP_ARG_WITH(simple_kafka_client, for kafka client support,
[  --with-simple-kafka-client             Include kafka client support])

dnl Check whether the extension is enabled at all
if test "$PHP_SIMPLE_KAFKA_CLIENT" != "no"; then

  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/librdkafka/rdkafka.h"  # you most likely want to change this
  if test -r $PHP_SIMPLE_KAFKA_CLIENT/$SEARCH_FOR; then # path given as parameter
    RDKAFKA_DIR=$PHP_SIMPLE_KAFKA_CLIENT
  else # search default path list
    AC_MSG_CHECKING([for librdkafka/rdkafka.h" in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        RDKAFKA_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$RDKAFKA_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the rdkafka distribution])
  fi

  PHP_ADD_INCLUDE($RDKAFKA_DIR/include)

  SOURCES="simple_kafka_client.c producer.c metadata.c metadata_broker.c metadata_topic.c metadata_partition.c metadata_collection.c configuration.c topic.c message.c functions.c consumer.c topic_partition.c kafka_exception.c"

  LIBNAME=rdkafka
  LIBSYMBOL=rd_kafka_new

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $RDKAFKA_DIR/$PHP_LIBDIR, SIMPLE_KAFKA_CLIENT_SHARED_LIBADD)
    AC_DEFINE(HAVE_RDKAFKALIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong rdkafka lib version or lib not found])
  ],[
    -L$RDKAFKA_DIR/$PHP_LIBDIR -lm
  ])

  ORIG_LDFLAGS="$LDFLAGS"
  ORIG_CPPFLAGS="$CPPFLAGS"
  LDFLAGS="-L$RDKAFKA_DIR/$PHP_LIBDIR -lm"
  CPPFLAGS="-I$RDKAFKA_DIR/include"

  AC_MSG_CHECKING([for librdkafka version])
  AC_EGREP_CPP(yes,[
#include <librdkafka/rdkafka.h>
#if RD_KAFKA_VERSION >= 0x000b0000
  yes
#endif
  ],[
    AC_MSG_RESULT([>= 1.4.0])
  ],[
    AC_MSG_ERROR([librdkafka version 1.4.0 or greater required.])
  ])

  LDFLAGS="$ORIG_LDFLAGS"
  CPPFLAGS="$ORIG_CPPFLAGS"

  PHP_SUBST(SIMPLE_KAFKA_CLIENT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(simple_kafka_client, $SOURCES, $ext_shared)
fi
