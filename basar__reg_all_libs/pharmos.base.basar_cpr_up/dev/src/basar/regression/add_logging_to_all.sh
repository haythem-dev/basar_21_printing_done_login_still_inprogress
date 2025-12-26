
#!/bin/bash

# Script to add logging configuration to all go.sh files in regression tests
# This ensures all tests have proper MARATHON_LOG_CONFIGDIR and MARATHON_LOG_LOGSDIR setup

REGRESSION_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Adding logging configuration to all regression test go.sh files..."
echo "Working directory: $REGRESSION_DIR"
echo ""

# Find all go.sh files in regression subdirectories
find "$REGRESSION_DIR" -type f -name "go.sh" -path "*/lib*/*/go.sh" | while read -r go_file; do
    # Extract test name from path
    test_name=$(basename "$(dirname "$go_file")")
    
    echo "Processing: $go_file (test: $test_name)"
    
    # Check if already has logging configuration
    if grep -q "MARATHON_LOG_CONFIGDIR" "$go_file"; then
        echo "  ✓ Already has logging configuration - skipping"
        continue
    fi
    
    # Create backup
    cp "$go_file" "$go_file.bak"
    
    # Create temporary file with logging configuration
    cat > /tmp/logging_config.tmp << EOF

# ============================================================================
# Setup logging configuration (required by Basar libraries)
# ============================================================================
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "\$LOG_CONFIG_DIR" "\$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="\$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="\$LOG_LOGS_DIR"

# Create minimal config file for this test (console output only)
cat > "\$LOG_CONFIG_DIR/${test_name}.cfg" << 'EOFCONFIG'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOFCONFIG
# ============================================================================

EOF
    
    # Find insertion point (after shebang and cd command, before first export or command)
    awk '
        BEGIN { inserted = 0 }
        /^#!\/bin\/bash/ || /^#!\/bin\/ksh/ { print; next }
        /^cd "?\$\(dirname/ { print; getline; print; inserted = 1; system("cat /tmp/logging_config.tmp"); next }
        { print }
    ' "$go_file.bak" > "$go_file"
    
    # If insertion didn't happen, try alternate pattern
    if ! grep -q "MARATHON_LOG_CONFIGDIR" "$go_file"; then
        awk '
            BEGIN { inserted = 0 }
            /^#!\/bin\/bash/ || /^#!\/bin\/ksh/ { print; next }
            /^$/ && !inserted && NR > 2 { inserted = 1; system("cat /tmp/logging_config.tmp"); }
            { print }
        ' "$go_file.bak" > "$go_file"
    fi
    
    # Verify it was added
    if grep -q "MARATHON_LOG_CONFIGDIR" "$go_file"; then
        echo "  ✓ Logging configuration added successfully"
        rm "$go_file.bak"
    else
        echo "  ✗ Failed to add logging configuration - restored backup"
        mv "$go_file.bak" "$go_file"
    fi
    
    # Also fix LD_LIBRARY_PATH to LIBPATH for AIX
    if grep -q "LD_LIBRARY_PATH" "$go_file"; then
        sed -i 's/LD_LIBRARY_PATH/LIBPATH/g' "$go_file"
        echo "  ✓ Fixed LD_LIBRARY_PATH → LIBPATH for AIX"
    fi
    
    echo ""
done

# Clean up
rm -f /tmp/logging_config.tmp

echo "Done! All go.sh files have been updated."
echo ""
echo "To verify, run:"
echo "  grep -r 'MARATHON_LOG_CONFIGDIR' . --include='go.sh' | wc -l"
