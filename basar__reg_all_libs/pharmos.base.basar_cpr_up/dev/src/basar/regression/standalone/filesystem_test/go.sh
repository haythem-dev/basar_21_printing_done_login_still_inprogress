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
cat > "$LOG_CONFIG_DIR/filesystem_test.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Basar Regression Test Runner
# Test: filesystem_test
# Library: standalone (no external dependencies)

echo "========================================="
echo "  Building: filesystem_test"
echo "========================================="

# Clean previous build
make clean > /dev/null 2>&1

# Build
echo "Compiling..."
make 2>&1

if [ $? -eq 0 ]; then
    echo "Build successful - binary created"
    echo ""
    echo "========================================="
    echo "  Running: filesystem_test"
    echo "========================================="
    echo ""
    
    # Run the test
    ./filesystem_test
    exit_code=$?
    
    echo ""
    echo "Test exit code: $exit_code"
else
    echo "Build FAILED"
    exit 1
fi
