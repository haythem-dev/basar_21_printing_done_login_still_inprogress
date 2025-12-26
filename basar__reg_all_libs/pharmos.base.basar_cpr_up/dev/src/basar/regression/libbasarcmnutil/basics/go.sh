#!/usr/bin/ksh -x

export BASEDIR=../../../../..
export MARATHON_LOG_CONFIGDIR=$(pwd)
export MARATHON_LOG_LOGSDIR=$(pwd)

cat > test_basar.cfg <<EOF
# log4cplus.properties
log4cplus.rootLogger=DEBUG, console

log4cplus.appender.console=log4cplus::ConsoleAppender
log4cplus.appender.console.layout=log4cplus::PatternLayout
log4cplus.appender.console.layout.ConversionPattern=%d{%Y-%m-%d %H:%M:%S} %-5p %c - %m%n

EOF

/opt/freeware/bin/g++-12 \
  -I$BASEDIR/ext/unittestpp/include \
  -I$BASEDIR/src/pkg-export/basar/include \
  -I$BASEDIR/ext/log4cplus/include \
  -I$BASEDIR/ext/boost/include \
  -fPIC -std=c++17 -Wl,-brtl -maix64 \
  -lpthread -lpthreads -pthread \
  -L$BASEDIR/ext/unittestpp/lib/Release \
  -L$BASEDIR/src/pkg-export/sharedlib/Release \
  -L$BASEDIR/ext/zlib/lib/Release \
  -L$INFORMIXDIR/lib \
  -L$INFORMIXDIR/lib/esql \
  -o test_basar test_basar.cpp \
  -lbasarcmnutil -lbasardebug \
  -lUnitTest++ 2>&1 | tee go.out

ldd ./test_basar | tee -a go.out
./test_basar 2>&1 | tee -a go.out
