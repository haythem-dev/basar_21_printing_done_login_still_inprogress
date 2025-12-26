# Quick Start Guide: Adding Regression Tests to Basar

## Setup (One-Time)

### 1. Create the Regression Directory Structure
```bash
cd pharmos.base.basar_cpr_up/dev/src/basar
mkdir -p regression
cd regression
```

### 2. Copy the Pattern Checker from Qt5
```bash
cp ../../../pharmos.3rd_party.qt5_cpr_up/dev/src/regression/check_patterns.sh .
chmod +x check_patterns.sh
```

### 3. Create the Main Orchestration Script
Save as `regression/do_regression.sh`:
```bash
#!/bin/bash
# Copy the script from REGRESSION_TEST_ANALYSIS.md section 3.3
```

## Creating Your First Regression Test

### Step 1: Create Test Directory
```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
mkdir -p libbasarcmnutil/hello_basar
cd libbasarcmnutil/hello_basar
```

### Step 2: Create Test Program (main.cpp)
```cpp
#include "libbasarcmnutil.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Hello from Basar!" << std::endl;
    std::cout << "Version: " << basar::cmnutil::getVersion() << std::endl;
    std::cout << "Test completed successfully" << std::endl;
    return 0;
}
```

### Step 3: Create Makefile
```makefile
TARGET_NAME = hello_basar

# Adjust these paths based on your installation
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../..
LOG4CPLUS_INC = ../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../ext/log4cplus/lib/debug

LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) \
       -lbasarcmnutil -lbasardebug -llog4cplus

INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC)

CXXFLAGS = -g -std=c++11

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGET_NAME) *.o go.out
```

### Step 4: Create Build & Run Script (go.sh)
```bash
#!/bin/bash

cd "$(dirname "$0")"

# Set up library paths
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../ext/log4cplus/lib/debug:$LD_LIBRARY_PATH"

# Clean previous build
rm -f hello_basar go.out

# Build
echo "=== Building hello_basar ===" 2>&1 | tee go.out
make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check if build succeeded
if [ ! -f hello_basar ]; then
    echo "ERROR: Build failed" 2>&1 | tee -a go.out
    exit 1
fi

echo "=== Build successful ===" 2>&1 | tee -a go.out

# Run the test
echo "=== Running test ===" 2>&1 | tee -a go.out
./hello_basar 2>&1 | tee -a go.out
exit_code=$?

# Report exit code
echo "=== Exit code: $exit_code ===" 2>&1 | tee -a go.out

exit $exit_code
```

Make it executable:
```bash
chmod +x go.sh
```

### Step 5: Create Expected Patterns (patterns.txt)
```
Hello from Basar!
Version:
Test completed successfully
Build successful
Exit code: 0
libbasarcmnutil.so
```

### Step 6: Run Your Test
```bash
# From the test directory
./go.sh

# Check output matches patterns
../../check_patterns.sh go.out patterns.txt
```

## Running All Regression Tests

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
```

## Test Template Generator Script

Save as `regression/new_test.sh`:
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <library_name> <test_name>"
    echo "Example: $0 libbasarcmnutil date_operations"
    exit 1
fi

LIBRARY=$1
TEST_NAME=$2
TEST_DIR="${LIBRARY}/${TEST_NAME}"

# Create directory
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

# Create main.cpp
cat > main.cpp << 'EOF'
#include "libbasarcmnutil.h"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        // TODO: Add your test code here
        std::cout << "Test: ${TEST_NAME}" << std::endl;
        std::cout << "Test completed successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
EOF

# Create Makefile
cat > makefile << 'EOF'
TARGET_NAME = ${TEST_NAME}

BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../..
LOG4CPLUS_INC = ../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../ext/log4cplus/lib/debug

LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) \
       -lbasarcmnutil -lbasardebug -llog4cplus

INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC)

CXXFLAGS = -g -std=c++11

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGET_NAME) *.o go.out
EOF

# Create go.sh
cat > go.sh << 'EOF'
#!/bin/bash

cd "$(dirname "$0")"

export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../ext/log4cplus/lib/debug:$LD_LIBRARY_PATH"

rm -f ${TEST_NAME} go.out

echo "=== Building ${TEST_NAME} ===" 2>&1 | tee go.out
make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

if [ ! -f ${TEST_NAME} ]; then
    echo "ERROR: Build failed" 2>&1 | tee -a go.out
    exit 1
fi

echo "=== Build successful ===" 2>&1 | tee -a go.out

echo "=== Running test ===" 2>&1 | tee -a go.out
./${TEST_NAME} 2>&1 | tee -a go.out
exit_code=$?

echo "=== Exit code: $exit_code ===" 2>&1 | tee -a go.out

exit $exit_code
EOF

chmod +x go.sh

# Create patterns.txt
cat > patterns.txt << 'EOF'
Test: ${TEST_NAME}
Test completed successfully
Build successful
Exit code: 0
EOF

echo "Test template created in: $TEST_DIR"
echo "Next steps:"
echo "1. Edit $TEST_DIR/main.cpp - add your test code"
echo "2. Update $TEST_DIR/patterns.txt - add expected output patterns"
echo "3. Run: cd $TEST_DIR && ./go.sh"
echo "4. Validate: ../../check_patterns.sh go.out patterns.txt"
```

Make it executable:
```bash
chmod +x new_test.sh
```

## Using the Template Generator

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./new_test.sh libbasarcmnutil my_new_test
cd libbasarcmnutil/my_new_test
# Edit main.cpp and patterns.txt
./go.sh
```

## Debugging Failed Tests

### 1. Check the Build Output
```bash
cat go.out | grep -i error
```

### 2. Check Library Paths
```bash
ldd ./your_test_program
```

### 3. Run Test Manually
```bash
export LD_LIBRARY_PATH="../../../library/lib/debug:$LD_LIBRARY_PATH"
./your_test_program
```

### 4. Verify Patterns
```bash
# See what patterns failed
../../check_patterns.sh go.out patterns.txt
```

## Common Issues

### Issue: "cannot find -lbasarcmnutil"
**Solution**: Build basar libraries first
```bash
cd pharmos.base.basar_cpr_up/dev/src/basar
make all
```

### Issue: "error while loading shared libraries"
**Solution**: Check LD_LIBRARY_PATH in go.sh points to correct locations

### Issue: Pattern not matching
**Solution**: 
1. Check exact output in go.out
2. Patterns use grep, so they're partial matches
3. Make sure pattern text appears exactly in output

## Integration with CI/CD

Add to your build pipeline:
```bash
# Build basar
make all

# Run unit tests
proc/run_ut.sh

# Run regression tests
cd regression
./do_regression.sh
```

## Next Steps

1. Create 2-3 simple regression tests
2. Validate they work correctly
3. Document any environment-specific setup needed
4. Train team on adding new tests
5. Integrate into regular testing workflow
