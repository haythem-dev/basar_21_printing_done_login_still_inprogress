#!/bin/bash

# Regression test runner script for date_operations

cd "$(dirname "$0")"

# ============================================================================
# Setup logging configuration (required by Basar libraries)
# ============================================================================
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

# Create minimal config file for this test (console output only)
cat > "$LOG_CONFIG_DIR/date_operations.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Set up library paths for runtime (AIX uses LIBPATH, not LD_LIBRARY_PATH)
# Add Informix libraries required by libbasarcmnutil.so
# Add ghostscript library required by libbasarcmnutil.so
# Add OpenSSL libraries required by libbasarcmnutil.so
if [ -n "$INFORMIXDIR" ]; then
    export LIBPATH="../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    echo "WARNING: INFORMIXDIR not set - Informix libraries may not be found" >> go.out
    # boost/lib is REQUIRED for runtime (libboost_system.so, libboost_filesystem.so)
    export LIBPATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:../../../../../ext/openssl/lib:$LIBPATH"
fi

# Clean previous build
rm -f date_operations go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building date_operations" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f date_operations ]; then
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
ldd date_operations 2>&1 | grep -E "libbasar|liblog4cplus|libssl|libz" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./date_operations 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code