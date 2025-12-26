#!/bin/bash

# Regression test runner for pdf_conversion_test
# AIX 7.2 Compatible

cd "$(dirname "$0")"

# Check if dependencies are available
if [ ! -d "../../../../../ext/log4cplus/include" ]; then
    # FALLBACK check for different relative path depth
    if [ -d "../../../../ext/log4cplus/include" ]; then
        echo "Found dependencies at ../../../../ext/log4cplus/include"
    else
        echo "SKIPPED: log4cplus not found - Basar environment not fully set up" > go.out
        echo "This test requires the full Basar development environment" >> go.out
        exit 0
    fi
fi

# Setup logging configuration
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

cat > "$LOG_CONFIG_DIR/pdf_conversion_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF

# Set up library paths (AIX uses LIBPATH)
# CICS library path for libBml.a (Informix BML library)
CICS_LIB="${DEVLIB_PATH:-/software/ae}/marathon/lib/tcaccess/8.0.20.1/lib"

if [ -n "$INFORMIXDIR" ]; then
    export LIBPATH="${CICS_LIB}:../../../../../ext/openssl/lib:../../../../../ext/openldap/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    export LIBPATH="${CICS_LIB}:../../../library/lib/debug:../../../../../ext/openldap/lib:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:../../../../../ext/openssl/lib:/usr/lib:/usr/local/lib:$LIBPATH"
fi

# Clean previous build
rm -f pdf_conversion_test go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building pdf_conversion_test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f pdf_conversion_test ]; then
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
ldd pdf_conversion_test 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./pdf_conversion_test 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
