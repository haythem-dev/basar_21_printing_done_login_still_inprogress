#!/bin/bash

cd "$(dirname "$0")"

# Setup logging configuration
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

# Create minimal log config
cat > "$LOG_CONFIG_DIR/library_loading.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF

# Clean previous build
rm -f library_loading go.out

# Build
echo "=========================================" 2>&1 | tee go.out
echo "  Building: library_loading" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | grep -v "0711-224\|0711-345" | tee -a go.out
make       2>&1 | grep -v "0711-224\|0711-345" | tee -a go.out

if [ ! -f library_loading ]; then
    echo "" 2>&1 | tee -a go.out
    echo "ERROR: Build failed" 2>&1 | tee -a go.out
    exit 1
fi

echo "" 2>&1 | tee -a go.out
echo "Build successful - binary created" 2>&1 | tee -a go.out

# Set library paths (NO LDAP libraries needed!)
export LIBPATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:$LIBPATH"

# Check linked libraries
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Checking linked libraries" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd library_loading 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running: library_loading" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./library_loading 2>&1 | tee -a go.out
exit_code=$?

echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code