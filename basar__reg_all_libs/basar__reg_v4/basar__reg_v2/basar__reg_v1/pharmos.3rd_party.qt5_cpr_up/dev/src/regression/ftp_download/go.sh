#!/usr/bin/ksh -x

echo "=========================================="
echo "Qt 5.15.16 FTP Download Regression Test"
echo "Target: AIX 64-bit"
echo "=========================================="

# Set script to exit on any error
set -e

# Get the directory of this script
SCRIPT_DIR=$(dirname "$0")

# Set QTDIR using AIX-compatible path resolution
# Navigate from regression/ftp_download to the src directory and find pkg-install
cd "$SCRIPT_DIR"
cd ../../
QTDIR_CANDIDATE=$(pwd)/pkg-install

# Check if Qt installation exists, if not, check alternative locations
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

echo "QTDIR set to: $QTDIR"

# Update PATH to include Qt binaries
export PATH=$QTDIR/bin:$PATH

# Verify Qt installation
if [ ! -f "$QTDIR/bin/qmake" ]; then
    echo "ERROR: qmake not found at $QTDIR/bin/qmake"
    exit 1
fi

if [ ! -f "$QTDIR/lib/libQt5Core.so" ]; then
    echo "ERROR: Qt5Core library not found at $QTDIR/lib/"
    exit 1
fi

# Fix qmake permissions if needed
chmod +x $QTDIR/bin/qmake 2>/dev/null || true

echo "=========================================="
echo "Qt 5.15.16 FTP Regression Test - AIX 64-bit"
echo "Build Phase Starting..."
echo "=========================================="

# Clean previous build
rm -f .qmake.stash ftp_download main.o

# Generate Makefile - ADD EXPLICIT PATTERN OUTPUT
echo "qmake -makefile ftp_download.pro" | tee go.out
qmake -makefile ftp_download.pro 2>&1 | tee -a go.out
if [ $? -ne 0 ]; then
    echo "ERROR: qmake failed" | tee -a go.out
    exit 1
fi

# Compile the project - ADD EXPLICIT PATTERN OUTPUT
echo "gmake" | tee -a go.out
gmake 2>&1 | tee -a go.out
if [ $? -ne 0 ]; then
    echo "ERROR: gmake compilation failed" | tee -a go.out
    exit 1
fi

echo "Build completed successfully!" | tee -a go.out
echo "" | tee -a go.out

# Display binary info - ADD PATTERN FOR BINARY VERIFICATION
echo "Binary verification patterns:" | tee -a go.out
ls -al ftp_download 2>&1 | tee -a go.out
echo "" | tee -a go.out
echo "Library dependencies:" | tee -a go.out
ldd ftp_download 2>&1 | tee -a go.out
echo "" | tee -a go.out

echo "==========================================" | tee -a go.out
echo "Execution Phase Starting..." | tee -a go.out
echo "==========================================" | tee -a go.out

# Execute the FTP download test
echo "Test command: ./ftp_download 10.156.136.177 /software/etc/profile_phoenix dummy.txt --user devuser --password phosix+" | tee -a go.out
echo "" | tee -a go.out

# Execute test and capture all output
./ftp_download 10.156.136.177 /software/etc/profile_phoenix dummy.txt --user devuser --password phosix+ 2>&1 | tee -a go.out

# Capture exit code
TEST_EXIT_CODE=$?

echo "" | tee -a go.out
echo "Test execution completed with exit code: $TEST_EXIT_CODE" | tee -a go.out

# Display final file information - FIX EXACT FORMAT MATCH
if [ -f "dummy.txt" ]; then
    file_size=$(wc -c < dummy.txt)
    echo "Final file size: ${file_size} bytes" | tee -a go.out
    echo "File exists: YES" | tee -a go.out
else
    echo "File exists: NO" | tee -a go.out
fi

echo "==========================================" | tee -a go.out
echo "Execution Phase Completed" | tee -a go.out
echo "==========================================" | tee -a go.out

# Exit with the same code as the test
exit $TEST_EXIT_CODE