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
cat > "$LOG_CONFIG_DIR/exception_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Basar Regression Test Runner
# Test: exception_test
# Library: libbasarcmnutil

echo "========================================="
echo "  Building: exception_test"
echo "========================================="


# Set up library paths (AIX uses LIBPATH, not LD_LIBRARY_PATH)
# OpenSSL path MUST come before system paths to override /usr/lib/libcrypto.so
if [ -n "$INFORMIXDIR" ]; then
    export LIBPATH="../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    export LIBPATH="../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$LIBPATH"
fi

# Clean previous build
make clean > /dev/null 2>&1

# Build
echo "Compiling..."
make 2>&1

if [ $? -eq 0 ]; then
    echo "Build successful - binary created"
    echo ""
    echo "========================================="
    echo "  Running: exception_test"
    echo "========================================="
    echo ""
    
    # Check library dependencies
    echo "Library dependencies:"
    ldd ./exception_test 2>/dev/null || echo "ldd not available"
    echo ""
    
    # Run the test
    ./exception_test
    exit_code=$?
    
    echo ""
    echo "Test exit code: $exit_code"
else
    echo "Build FAILED"
    exit 1
fi
