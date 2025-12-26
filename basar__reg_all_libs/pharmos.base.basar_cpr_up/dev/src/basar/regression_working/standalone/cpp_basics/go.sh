
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
cat > "$LOG_CONFIG_DIR/cpp_basics.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
# ============================================================================

# Compile
make clean
make > go.out 2>&1

# Check if build succeeded
if [ ! -f cpp_basics ]; then
    echo "" >> go.out
    echo "ERROR: Build failed - executable not created" >> go.out
    exit 1
fi

echo "Build successful - binary created" >> go.out

# Run the test
./cpp_basics >> go.out 2>&1
exit_code=$?

echo "Exit code: $exit_code" >> go.out
echo "Test execution completed with exit code $exit_code" >> go.out

exit $exit_code
