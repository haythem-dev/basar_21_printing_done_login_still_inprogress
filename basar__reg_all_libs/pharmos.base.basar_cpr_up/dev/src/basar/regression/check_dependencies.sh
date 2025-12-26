
#!/bin/bash
# Dependency verification script for Basar regression tests
# Usage: ./check_dependencies.sh <executable>

if [ $# -eq 0 ]; then
    echo "Usage: $0 <executable>"
    exit 1
fi

EXECUTABLE=$1

if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: Executable not found: $EXECUTABLE"
    exit 1
fi

echo "========================================="
echo "  Dependency Analysis for: $EXECUTABLE"
echo "========================================="
echo ""

# Check all shared library dependencies
echo "Direct Dependencies:"
dump -H "$EXECUTABLE" 2>/dev/null | grep "NEEDED" || ldd "$EXECUTABLE"
echo ""

# Check for common issues
echo "Checking for potential issues..."

# Check if linking to system OpenSSL instead of custom
if ldd "$EXECUTABLE" 2>/dev/null | grep -q "/usr/lib/libssl.so"; then
    echo "⚠️  WARNING: Linking to system OpenSSL (/usr/lib/libssl.so)"
    echo "    This may cause 'invalid format' errors on AIX"
    echo "    Solution: Ensure ../../../../../ext/openssl/lib is first in LIBPATH"
fi

# Check for missing Basar libraries
MISSING_LIBS=$(ldd "$EXECUTABLE" 2>&1 | grep "not found" || true)
if [ -n "$MISSING_LIBS" ]; then
    echo "❌ ERROR: Missing libraries detected:"
    echo "$MISSING_LIBS"
    exit 1
fi

echo "✅ All dependencies resolved"
