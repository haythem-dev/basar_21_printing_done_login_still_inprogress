#!/bin/bash

# Template generator for creating new Basar regression tests
# Usage: ./new_test.sh <library_name> <test_name>
# Example: ./new_test.sh libbasarcmnutil my_test

if [ $# -ne 2 ]; then
    echo "Usage: $0 <library_name> <test_name>"
    echo ""
    echo "Examples:"
    echo "  $0 libbasarcmnutil date_test"
    echo "  $0 libbasardbsql connection_test"
    echo "  $0 libbasarguiqt window_test"
    echo ""
    echo "Available libraries:"
    echo "  - libbasarcmnutil"
    echo "  - libbasardbsql"
    echo "  - libbasardbaspect"
    echo "  - libbasarguiqt"
    echo "  - libbasarlogin"
    exit 1
fi

LIBRARY=$1
TEST_NAME=$2
TEST_DIR="${LIBRARY}/${TEST_NAME}"

# Validate library name
case "$LIBRARY" in
    libbasarcmnutil|libbasardbsql|libbasardbaspect|libbasarguiqt|libbasarlogin)
        ;;
    *)
        echo "ERROR: Unknown library '$LIBRARY'"
        echo "Valid libraries: libbasarcmnutil, libbasardbsql, libbasardbaspect, libbasarguiqt, libbasarlogin"
        exit 1
        ;;
esac

# Check if test already exists
if [ -d "$TEST_DIR" ]; then
    echo "ERROR: Test directory already exists: $TEST_DIR"
    exit 1
fi

# Create directory
echo "Creating test directory: $TEST_DIR"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

# Create main.cpp
echo "Creating main.cpp..."
cat > main.cpp << EOF
#include "libbasarcmnutil/libbasarcmnutil.h"
#include <iostream>

using namespace basar::cmnutil;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: ${TEST_NAME}" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // TODO: Add your test code here
        std::cout << "Test: ${TEST_NAME}" << std::endl;
        std::cout << "TODO: Implement test logic" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "  Test completed successfully" << std::endl;
        std::cout << "=========================================" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "ERROR: Unknown exception caught" << std::endl;
        return 1;
    }
}
EOF

# Create Makefile
echo "Creating makefile..."
cat > makefile << EOF
##############################################################
#                                                            #
#   Basar Regression Test: ${TEST_NAME}                     #
#                                                            #
##############################################################

TARGET_NAME = ${TEST_NAME}

# Directories
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../../..
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_LIB = ../../../../../ext/boost/lib

# Libraries
LIBS = -L\$(BASAR_LIB_DIR) -L\$(LOG4CPLUS_LIB) -L\$(BOOST_LIB) \\
       -lbasarcmnutil -lbasardebug -llog4cplus \\
       -lboost_system -lboost_filesystem

# Include paths
INCS = -I\$(BASAR_INC_DIR) -I\$(LOG4CPLUS_INC)

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# Linker flags
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: \$(TARGET_NAME)

\$(TARGET_NAME): main.cpp
        \$(CXX) \$(CXXFLAGS) \$(INCS) -o \$@ \$< \$(LIBS) \$(LDFLAGS)

clean:
        rm -f \$(TARGET_NAME) *.o go.out

.PHONY: all clean
EOF

# Create go.sh
echo "Creating go.sh..."
cat > go.sh << 'EOF'
#!/bin/bash

# Regression test runner script for ${TEST_NAME}

cd "$(dirname "$0")"

# Set up library paths for runtime
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:$LD_LIBRARY_PATH"

# Clean previous build
rm -f ${TEST_NAME} go.out

# Build the test
echo "=========================================" 2>&1 | tee go.out
echo "  Building ${TEST_NAME}" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f ${TEST_NAME} ]; then
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
ldd ${TEST_NAME} 2>&1 | grep -E "libbasar|liblog4cplus" | tee -a go.out

# Run the test
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Running test" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "" 2>&1 | tee -a go.out

./${TEST_NAME} 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out
echo "  Test exit code: $exit_code" 2>&1 | tee -a go.out
echo "=========================================" 2>&1 | tee -a go.out

exit $exit_code
EOF

# Fix variable substitution in go.sh
sed -i "s/\${TEST_NAME}/${TEST_NAME}/g" go.sh
chmod +x go.sh

# Create patterns.txt
echo "Creating patterns.txt..."
cat > patterns.txt << EOF
Basar Regression Test: ${TEST_NAME}
Test: ${TEST_NAME}
Test completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
EOF

echo ""
echo "========================================="
echo "  Test template created successfully!"
echo "========================================="
echo ""
echo "Test directory: $TEST_DIR"
echo ""
echo "Next steps:"
echo "  1. Edit $TEST_DIR/main.cpp - add your test code"
echo "  2. Update $TEST_DIR/patterns.txt - add expected output patterns"
echo "  3. If needed, update $TEST_DIR/makefile - adjust libraries/includes"
echo "  4. Run: cd $TEST_DIR && ./go.sh"
echo "  5. Validate: ../../check_patterns.sh go.out patterns.txt"
echo ""
