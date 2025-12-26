#!/bin/bash

# ============================================================================
# Setup logging configuration (required by Basar libraries)
# ============================================================================
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

# Create minimal config file for this test (console output only)
cat > "$LOG_CONFIG_DIR/manager_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Set up library paths (AIX uses LIBPATH, not LD_LIBRARY_PATH)
# CICS library path (required for libbasardbsql.so which links against libBml.a)
# libBml.a is in the CICS/tcaccess library, NOT in INFORMIXDIR!
CICS_LIB="${DEVLIB_PATH:-/software/ae}/marathon/lib/tcaccess/8.0.20.1/lib"

# OpenSSL path MUST come before system paths to override /usr/lib/libcrypto.so
if [ -n "$INFORMIXDIR" ]; then
    echo "Using INFORMIXDIR: $INFORMIXDIR"
    export LIBPATH="${CICS_LIB}:../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    echo "WARNING: INFORMIXDIR not set - Informix libraries may not be found"
    export LIBPATH="${CICS_LIB}:../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$LIBPATH"
fi

# Clean previous build
rm -f manager_test *.o go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building manager_test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

# Compile with proper library order (dependencies listed after dependents)
g++ -g -std=c++17 -maix64 -pthread \
    -I../../../library/include \
    -I../../../../../ext/log4cplus/include \
    -I../../../../../ext/boost/include \
    -o manager_test main.cpp \
    -L../../../library/lib/debug \
    -L../../../../../ext/log4cplus/lib/debug \
    -L../../../../../ext/boost/lib \
    -lbasardbaspect \
    -lbasarproperty \
    -lbasardbsql \
    -lbasarcmnutil \
    -lbasardebug \
    -llog4cplus \
    -lboost_system \
    -lboost_filesystem \
    -Wl,-brtl 2>&1 | grep -v "0711-224\|0711-345" | tee -a go.out

# Check if build succeeded
if [ ! -f manager_test ]; then
    echo "" 2>&1 | tee -a go.out
    echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
    exit 1
fi

echo "" 2>&1 | tee -a go.out
echo "Build successful - binary created" 2>&1 | tee -a go.out

# Check linked libraries
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Checking linked libraries" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd manager_test 2>&1 | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./manager_test 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code