#!/bin/bash

# Regression test runner script for configuration_test

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
cat > "$LOG_CONFIG_DIR/configuration_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Clean previous build
rm -f configuration_test go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building: configuration_test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f configuration_test ]; then
    echo "" 2>&1 | tee -a go.out
    echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
    exit 1
fi

echo "" 2>&1 | tee -a go.out
echo "Build successful - binary created" 2>&1 | tee -a go.out

# ============================================================================
# Set up library paths for runtime (AIX uses LIBPATH, not LD_LIBRARY_PATH)
# 
# IMPORTANT: libbasarlogin depends on libbasardbsql.so which requires:
#   - libBml.a (Informix BML library) - located in CICS library path, NOT INFORMIXDIR!
#   - Other Informix ESQL libraries
#
# ALSO: libldap.so (from /opt/freeware) requires system OpenSSL:
#   - /opt/freeware/lib must be in LIBPATH for libssl.so.1.1
#
# This matches the pattern used in working libbasardbsql/connection_basic/go.sh
# ============================================================================

# CICS library path (required for libbasardbsql.so which links against libBml.a)
CICS_LIB="${DEVLIB_PATH:-/software/ae}/marathon/lib/tcaccess/8.0.20.1/lib"

if [ -n "$INFORMIXDIR" ]; then
    echo "Using INFORMIXDIR: $INFORMIXDIR" 2>&1 | tee -a go.out
    echo "Using CICS_LIB: $CICS_LIB" 2>&1 | tee -a go.out
    export LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
    echo "Using CICS_LIB: $CICS_LIB" 2>&1 | tee -a go.out
    echo "WARNING: INFORMIXDIR not set - Informix libraries may not be found" 2>&1 | tee -a go.out
    export LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$LIBPATH"
fi

# Check linked libraries
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Checking linked libraries" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd configuration_test 2>&1 | grep -E "libbasar|liblog4cplus|libBml|libssl" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running: configuration_test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./configuration_test 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
