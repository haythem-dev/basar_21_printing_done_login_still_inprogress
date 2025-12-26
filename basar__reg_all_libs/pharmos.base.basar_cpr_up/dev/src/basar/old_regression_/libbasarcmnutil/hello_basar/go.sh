#!/bin/bash

cd "$(dirname "$0")"

# Check if log4cplus is available
if [ ! -d "../../../../../ext/log4cplus/include" ]; then
    echo "SKIPPED: log4cplus not found - Basar environment not fully set up" > go.out
    echo "This test requires the full Basar development environment" >> go.out
    exit 0
fi

# ============================================================================
# Setup logging configuration (required by Basar libraries)
# ============================================================================
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

# Create minimal config file for this test (console output only)
cat > "$LOG_CONFIG_DIR/hello_basar.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Set up library paths (AIX uses LIBPATH, not LD_LIBRARY_PATH)
# Add Informix libraries required by libbasarcmnutil.so
# Add ghostscript library required by libbasarcmnutil.so
# Add OpenSSL libraries required by libopensslwrap.so
# NOTE: OpenSSL path MUST come before system paths to override /usr/lib/libssl.so
if [ -n "$INFORMIXDIR" ]; then
    echo "Using INFORMIXDIR: $INFORMIXDIR" >> go.out
    export LIBPATH="../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    echo "WARNING: INFORMIXDIR not set - Informix libraries may not be found" >> go.out
    # Set up library paths for runtime (AIX uses LIBPATH, not LD_LIBRARY_PATH)
    # Complete library path including all dependencies:
    # - Basar libraries (libbasarcmnutil.so, libbasardebug.so)
    # - log4cplus (logging framework)
    # - boost (C++ utilities) - REQUIRED for runtime
    # - libssh2 (SSH/SFTP support) - requires zlib
    # - ghostscript (PDF/PostScript rendering)
    # - zlib (compression library, required by libssh2)
    # - openssl (SSL/TLS support)
    export LIBPATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:../../../../../ext/openssl/lib:$LIBPATH"
fi

# Clean previous build
rm -f hello_basar go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building hello_basar" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f hello_basar ]; then
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
ldd hello_basar 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./hello_basar 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code