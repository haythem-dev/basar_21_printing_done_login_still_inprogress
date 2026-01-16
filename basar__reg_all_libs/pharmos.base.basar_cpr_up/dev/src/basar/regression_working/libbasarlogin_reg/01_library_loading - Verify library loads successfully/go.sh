#!/bin/bash
# Regression test runner script for library_loading (libbasarlogin) - A.2 aligned with working samples

cd "$(dirname "$0")"

# ============================================================================
# Setup logging configuration (required by Basar libraries)
# ============================================================================
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

cat > "$LOG_CONFIG_DIR/library_loading.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

rm -f library_loading go.out

echo "=========================================" 2>&1 | tee go.out
echo "  Building: library_loading"             2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo ""                                       2>&1 | tee -a go.out

make clean 2>&1 | grep -v "0711-224\|0711-345" | tee -a go.out
make       2>&1 | grep -v "0711-224\|0711-345" | tee -a go.out

if [ ! -f library_loading ]; then
  echo "" 2>&1 | tee -a go.out
  echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
  exit 1
fi

echo "" 2>&1 | tee -a go.out
echo "Build successful - binary created" 2>&1 | tee -a go.out

# ============================================================================
# Runtime: LIBPATH (AIX)
# Align with known working sample (libbasarprinting/file_extension_test)
# ============================================================================
CICS_LIB="${DEVLIB_PATH:-/software/ae}/marathon/lib/tcaccess/8.0.20.1/lib"
OPENLDAP_EXT_LIB="../../../../../ext/openldap/lib"

BASAR_LIB="../../../library/lib/debug"
LOG4CPLUS_LIB="../../../../../ext/log4cplus/lib/debug"
BOOST_LIB="../../../../../ext/boost/lib"
OPENSSL_LIB="../../../../../ext/openssl/lib"
ZLIB_LIB="../../../../../ext/zlib/lib/debug"
LIBSSH2_LIB="../../../../../ext/libssh2/lib/debug"
GHOSTSCRIPT_LIB="../../../../../ext/ghostscript/lib"

if [ -n "$INFORMIXDIR" ]; then
  export LIBPATH="${CICS_LIB}:${OPENSSL_LIB}:${OPENLDAP_EXT_LIB}:${BASAR_LIB}:${LOG4CPLUS_LIB}:${BOOST_LIB}:${LIBSSH2_LIB}:${GHOSTSCRIPT_LIB}:${ZLIB_LIB}:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
else
  export LIBPATH="${CICS_LIB}:${BASAR_LIB}:${OPENLDAP_EXT_LIB}:${LOG4CPLUS_LIB}:${BOOST_LIB}:${LIBSSH2_LIB}:${GHOSTSCRIPT_LIB}:${ZLIB_LIB}:${OPENSSL_LIB}:/usr/lib:/usr/local/lib:$LIBPATH"
fi

echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Checking linked libraries"             2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd library_loading 2>&1 | tee -a go.out

echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running: library_loading"              2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo ""                                       2>&1 | tee -a go.out

./library_loading 2>&1 | tee -a go.out
exit_code=$?

echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code"            2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
