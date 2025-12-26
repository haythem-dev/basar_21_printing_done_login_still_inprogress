#!/bin/bash

cd "$(dirname "$0")"

# Check if log4cplus is available
if [ ! -d "../../../../ext/log4cplus/include" ]; then
    echo "SKIPPED: log4cplus not found - Basar environment not fully set up" > go.out
    echo "This test requires the full Basar development environment" >> go.out
    exit 0
fi

# Setup logging
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

cat > "$LOG_CONFIG_DIR/libbasarprinting_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF

# Library paths
export LIBPATH="../../library/lib/debug:../../../../ext/log4cplus/lib/debug:../../../../ext/boost/lib:../../../../ext/libssh2/lib/debug:../../../../ext/ghostscript/lib:../../../../ext/zlib/lib/debug:../../../../ext/openssl/lib:$LIBPATH"

# Clean and build
rm -f libbasarprinting_test go.out
echo "Building libbasarprinting_test..." > go.out

# We use the existing makefile but might need to build a test binary
# For now, let's assume we compile main.cpp against the library
g++ -g -std=c++17 -maix64 -pthread -I. -I../../../library/include -I../../../../ext/log4cplus/include -I../../../../ext/boost/include \
    -o libbasarprinting_test main.cpp \
    -L../../../library/lib/debug -L../../../../ext/log4cplus/lib/debug -L../../../../ext/boost/lib \
    -lbasarprinting -lbasarcmnutil -lbasardebug -llog4cplus -lboost_system -lboost_filesystem \
    -Wl,-brtl >> go.out 2>&1

if [ ! -f libbasarprinting_test ]; then
    echo "ERROR: Build failed" >> go.out
    exit 1
fi

./libbasarprinting_test >> go.out 2>&1
exit $?
