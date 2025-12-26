#!/bin/bash

# Regression test runner for time_edit_test
# AIX 7.2 Compatible

cd "$(dirname "$0")"

# Check if dependencies are available
if [ ! -d "../../../../../ext/log4cplus/include" ]; then
    echo "SKIPPED: log4cplus not found - Basar environment not fully set up" > go.out
    echo "This test requires the full Basar development environment" >> go.out
    exit 0
fi

# Setup logging configuration
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

cat > "$LOG_CONFIG_DIR/time_edit_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF

# Set up library paths (AIX uses LIBPATH)
if [ -n "$INFORMIXDIR" ]; then
    export LIBPATH="../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    export LIBPATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:../../../../../ext/openssl/lib:$LIBPATH"
fi

# Clean previous build
rm -f time_edit_test go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building time_edit_test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f time_edit_test ]; then
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
ldd time_edit_test 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./time_edit_test 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
