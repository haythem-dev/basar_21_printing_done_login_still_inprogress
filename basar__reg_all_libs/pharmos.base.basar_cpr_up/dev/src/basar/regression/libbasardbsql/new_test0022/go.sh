#!/bin/bash

# Regression test runner script for new_test0022

cd "$(dirname "$0")"

# Set up library paths for runtime
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../ext/log4cplus/lib/debug:../../../../ext/boost/lib:$LD_LIBRARY_PATH"

# Clean previous build
rm -f new_test0022 go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building new_test0022" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f new_test0022 ]; then
    echo "" 2>&1 | tee -a go.out
    echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
    exit 1
fi

echo "" 2>&1 | tee -a go.out
echo "Build successful - binary created" 2>&1 | tee -a go.out

# Check linked libraries
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Checking linked libraries" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
ldd new_test0022 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./new_test0022 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
