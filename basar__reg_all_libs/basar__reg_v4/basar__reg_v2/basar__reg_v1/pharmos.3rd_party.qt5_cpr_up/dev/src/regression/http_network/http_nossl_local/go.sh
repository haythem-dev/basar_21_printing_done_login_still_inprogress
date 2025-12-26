#!/bin/bash

# HTTP Network Test (No SSL - Local Server Version) - AIX Build Script
# This version tests against a local HTTP server instead of external services

echo "Qt HTTP Network Test (No SSL - Local Server)"
echo "============================================="

# Get the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Set Qt environment (relative to test location)
# Path from http_nossl_local back to pkg-install
QTDIR="$SCRIPT_DIR/../../../pkg-install"
export QTDIR
export PATH="$QTDIR/bin:$PATH"
export LD_LIBRARY_PATH="$QTDIR/lib:$LD_LIBRARY_PATH"

echo "Qt environment:"
echo "QTDIR: $QTDIR"
echo "PATH: $PATH"
echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo ""

# Debug: Check if qmake exists
echo "Debug: Checking Qt installation..."
echo "qmake path: $(which qmake)"
if [ -f "$QTDIR/bin/qmake" ]; then
    echo "✓ qmake found at: $QTDIR/bin/qmake"
else
    echo "✗ qmake NOT found at: $QTDIR/bin/qmake"
    echo "Actual QTDIR contents:"
    ls -la "$QTDIR/bin/" | head -5
fi
echo ""

# Clean previous build
echo "Cleaning previous build..."
rm -f http_nossl_local Makefile *.o moc_*.cpp go.out

# Generate Makefile
echo "Generating Makefile..."
qmake -makefile http_nossl_local.pro

if [ $? -ne 0 ]; then
    echo "ERROR: qmake failed"
    exit 1
fi

# Build the test
echo "Building test..."
gmake clean
gmake

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    exit 1
fi

# Check library dependencies
echo "Checking library dependencies..."
echo "http_nossl_local needs:" > go.out
ldd http_nossl_local >> go.out 2>&1

echo ""
echo "Library dependencies:"
cat go.out
echo ""

# Check if local test server is available
echo "Checking for local test server..."
if curl -s "http://localhost:8080/get" > /dev/null 2>&1; then
    echo "✓ Local test server is available at http://localhost:8080"
else
    echo "⚠ Local test server not found at http://localhost:8080"
    echo "Start the test server with: python3 ../local_test_server.py 8080"
    echo "Continuing with test (will show connection errors)..."
fi
echo ""

# Run the test
echo "Running HTTP network test (local server)..."
echo "==========================================="
./http_nossl_local 2>&1 | tee -a go.out

# Save exit code
EXIT_CODE=${PIPESTATUS[0]}

echo ""
echo "Test completed with exit code: $EXIT_CODE"

# Clean up build artifacts
echo "Cleaning up..."
rm -f Makefile *.o moc_*.cpp

exit $EXIT_CODE