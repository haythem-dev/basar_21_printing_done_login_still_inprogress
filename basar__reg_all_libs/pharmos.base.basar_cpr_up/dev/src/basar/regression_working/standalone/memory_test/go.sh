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
cat > "$LOG_CONFIG_DIR/memory_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

cd "$(dirname "$0")"

# Clean and build
rm -f memory_test go.out
make clean 2>&1 | tee go.out
make       2>&1 | tee -a go.out

# Check that binary was created
if [ -f memory_test ]; then
    echo "" 2>&1 | tee -a go.out
    echo "Build successful - binary created" 2>&1 | tee -a go.out
else
    echo "" 2>&1 | tee -a go.out
    echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
    exit 1
fi

# Show linked libraries
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Linked libraries" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd memory_test 2>&1 | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
./memory_test 2>&1 | tee -a go.out
exit_code=$?

# Report result
echo "" 2>&1 | tee -a go.out
if [ $exit_code -eq 0 ]; then
    echo "Test execution completed with exit code 0" 2>&1 | tee -a go.out
else
    echo "Test execution failed with exit code $exit_code" 2>&1 | tee -a go.out
fi

exit $exit_code