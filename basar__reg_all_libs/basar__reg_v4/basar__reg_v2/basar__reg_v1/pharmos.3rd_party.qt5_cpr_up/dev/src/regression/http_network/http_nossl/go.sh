#!/bin/bash

# HTTP Network Test (No SSL - Remote Server Version) - AIX Build Script
# This version tests against remote HTTP servers instead of a local server

echo "Qt HTTP Network Test (No SSL - Remote Server)"
echo "============================================="

# Get the script directory

# Set Qt environment (relative to test location)
# Navigate from regression/http_network/http_nossl to find pkg-install
SCRIPT_DIR=$(dirname "$0")
cd "$SCRIPT_DIR"
cd ../../../
QTDIR_CANDIDATE=$(pwd)/pkg-install

# Check if Qt installation exists
if [ -d "$QTDIR_CANDIDATE" ]; then
    export QTDIR="$QTDIR_CANDIDATE"
elif [ -d "$(pwd)/../pkg-install" ]; then
    export QTDIR="$(pwd)/../pkg-install"
else
    echo "ERROR: Qt installation not found. Expected at $QTDIR_CANDIDATE"
    echo "Available directories:"
    ls -la "$(pwd)"
    echo "Please ensure Qt is built before running regression tests."
    exit 1
fi

# Navigate back to the test directory where .pro file is located
cd "$SCRIPT_DIR"

export PATH="$QTDIR/bin:$PATH"

echo "Qt environment:" | tee -a go.out
echo "QTDIR: $QTDIR" | tee -a go.out
echo "PATH: $PATH" | tee -a go.out
echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH" | tee -a go.out
echo "" | tee -a go.out

# Debug: Check if qmake exists
echo "Debug: Checking Qt installation..." | tee -a go.out
echo "qmake path: $(which qmake)" | tee -a go.out
if [ -f "$QTDIR/bin/qmake" ]; then
    echo "✓ qmake found at: $QTDIR/bin/qmake" | tee -a go.out
else
    echo "✗ qmake NOT found at: $QTDIR/bin/qmake" | tee -a go.out
    echo "Actual QTDIR contents:" | tee -a go.out
    ls -la "$QTDIR/bin" 2>&1 | tee -a go.out || echo "Directory not accessible" | tee -a go.out
    exit 1
fi

# Clean previous build
echo "Cleaning previous build..." | tee -a go.out
if [ -f Makefile ]; then
    make clean
    rm -f Makefile
fi

# Configure project
echo "Running qmake..." | tee go.out
$QTDIR/bin/qmake -o Makefile http_nossl.pro 2>&1 | tee -a go.out
if [ $? -ne 0 ]; then
    echo "❌ qmake failed!" | tee -a go.out
    exit 1
else
    echo "✓ qmake completed successfully" | tee -a go.out
fi

# Build project
echo "Building project..." | tee -a go.out
make 2>&1 | tee -a go.out
if [ $? -ne 0 ]; then
    echo "❌ Build failed!" | tee -a go.out
    exit 1
else
    echo "✓ Build completed successfully" | tee -a go.out
fi

# Check DNS and network connectivity first
echo "Checking DNS and network connectivity..." | tee -a go.out
chmod +x check_dns.sh
./check_dns.sh 2>&1 | tee -a go.out

echo "" | tee -a go.out
echo "Running HTTP network test (Remote Server)..." | tee -a go.out
./http_nossl 2>&1 | tee -a go.out
EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ]; then
    echo "✓ Test completed successfully!" | tee -a go.out
else
    echo "❌ Test failed with exit code $EXIT_CODE" | tee -a go.out
fi

exit $EXIT_CODE