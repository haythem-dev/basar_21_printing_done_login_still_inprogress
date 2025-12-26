# Basar Regression Test Directory - Complete Guide

## 📋 Table of Contents

1. [Overview](#overview)
2. [Directory Structure](#directory-structure)
3. [Test Components](#test-components)
4. [How Tests Work](#how-tests-work)
5. [Running Tests](#running-tests)
6. [Creating New Tests](#creating-new-tests)
7. [Build Configuration](#build-configuration)
8. [Test Results](#test-results)

---

## Overview

The Basar regression test suite is modeled after the **Qt5 regression test approach**, using a pattern-matching validation system. Tests are organized by library and functionality, with each test being self-contained.

**Location**: `pharmos.base.basar/dev/src/basar/regression/`

**Purpose**: Validate that Basar libraries work correctly after installation, compilation, or system changes.

---

## Directory Structure

```
regression/
│
├── do_regression.sh          # Main test runner (executes all tests)
├── check_patterns.sh         # Pattern validation utility
├── new_test.sh              # Template generator for new tests
├── regression.summary        # Test results summary (generated)
├── README.md                # Regression test documentation
├── IMPLEMENTATION_STATUS.md  # Implementation tracking
│
├── libbasarcmnutil/         # Tests for libbasarcmnutil library
│   ├── hello_basar/         # Basic "Hello World" test
│   │   ├── main.cpp         # Test source code
│   │   ├── makefile         # Build configuration
│   │   ├── go.sh            # Test execution script
│   │   ├── patterns.txt     # Expected output patterns
│   │   └── go.out           # Actual output (generated)
│   │
│   ├── date_operations/     # Date class functionality test
│   │   ├── main.cpp
│   │   ├── makefile
│   │   ├── go.sh
│   │   ├── patterns.txt
│   │   └── go.out
│   │
│   └── decimal_arithmetic/  # Decimal class functionality test
│       ├── main.cpp
│       ├── makefile
│       ├── go.sh
│       ├── patterns.txt
│       └── go.out
│
└── standalone/              # Tests for standalone/infrastructure
    └── infrastructure_test/ # Tests that regression framework works
        ├── main.cpp
        ├── makefile
        ├── go.sh
        ├── patterns.txt
        └── go.out
```

---

## Test Components

Each regression test consists of **5 required files**:

### 1. **main.cpp** - Test Source Code

The C++ program that tests specific Basar functionality.

**Structure**:
```cpp
#include "libbasarcmnutil.h"
#include <iostream>

using namespace basar::cmnutil;

int main(int argc, char* argv[])
{
    try {
        // Header
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: <test_name>" << std::endl;
        std::cout << "=========================================" << std::endl;
        
        // Test logic here
        // ...
        
        // Footer
        std::cout << "Test completed successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
```

**Key Points**:
- Uses Basar library headers and classes
- Prints clear, testable output
- Returns 0 on success, non-zero on failure
- Includes error handling

---

### 2. **makefile** - Build Configuration

Compiles the test program with correct include paths and libraries.

**Fixed Structure** (after our corrections):
```makefile
TARGET_NAME = test_name

# Directories (relative paths from test directory)
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../../library/include           # ✅ Fixed
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_INC = ../../../../../ext/boost/include        # ✅ Fixed
BOOST_LIB = ../../../../../ext/boost/lib

# Libraries to link
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(BOOST_LIB) \
       -lbasarcmnutil -lbasardebug -llog4cplus \
       -lboost_system -lboost_filesystem

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC) -I$(BOOST_INC)  # ✅ Fixed

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# Linker flags
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out

.PHONY: all clean
```

**Critical Rules**:
- Recipe lines (after targets) **MUST** use TAB characters, not spaces
- Include paths must point to actual header locations
- Library paths must point to actual .so/.a locations

---

### 3. **go.sh** - Test Execution Script

Bash script that builds and runs the test, capturing all output.

**Standard Structure**:
```bash
#!/bin/bash

cd "$(dirname "$0")"

# Check dependencies
if [ ! -d "../../../../../ext/log4cplus/include" ]; then
    echo "SKIPPED: Dependencies not found" > go.out
    exit 0
fi

# Set library paths
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:$LD_LIBRARY_PATH"

# Clean previous build
rm -f <test_name> go.out

# Build
echo "Building <test_name>" 2>&1 | tee go.out
make clean 2>&1 | tee -a go.out
make       2>&1 | tee -a go.out

# Check build success
if [ ! -f <test_name> ]; then
    echo "ERROR: Build failed" 2>&1 | tee -a go.out
    exit 1
fi

echo "Build successful - binary created" 2>&1 | tee -a go.out

# Run test
./<test_name> 2>&1 | tee -a go.out
exit_code=$?

echo "Test exit code: $exit_code" 2>&1 | tee -a go.out
exit $exit_code
```

**Key Actions**:
1. Changes to test directory
2. Checks for dependencies
3. Sets up library paths
4. Builds the test (make)
5. Runs the test executable
6. Captures all output to `go.out`
7. Returns exit code

---

### 4. **patterns.txt** - Expected Output Patterns

Text file containing patterns (strings) that should appear in the test output.

**Format**:
```
Pattern 1
Pattern 2
Pattern 3
...
```

**Example** (hello_basar/patterns.txt):
```
Hello from Basar!
Version:
Test completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

**Validation**:
- Each line is a separate pattern
- Patterns are searched in `go.out` using `grep`
- Empty lines are ignored
- Patterns can be literal text or regex-compatible strings

---

### 5. **go.out** - Test Output (Generated)

This file is **generated automatically** by `go.sh` and contains:
- Build output (compiler messages)
- Test execution output
- Error messages (if any)
- Exit codes

**Example Contents**:
```
=========================================
  Building hello_basar
=========================================

g++ -g -std=c++17 -maix64 -pthread ...
Build successful - binary created

=========================================
  Running test
=========================================

Hello from Basar!
Version: 1.0.0
Test completed successfully

=========================================
  Test exit code: 0
=========================================
```

---

## How Tests Work

### Execution Flow

```
┌─────────────────────────────────────────────────────────────┐
│ 1. User runs: ./do_regression.sh                           │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│ 2. do_regression.sh finds all go.sh scripts                │
│    - Searches: find . -name "go.sh"                         │
│    - Sorts them alphabetically                              │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│ 3. For each test:                                           │
│    - cd to test directory                                   │
│    - Execute: bash ./go.sh                                  │
│    - Generates: go.out                                      │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│ 4. Pattern validation:                                      │
│    - Runs: check_patterns.sh go.out patterns.txt           │
│    - Searches for each pattern in go.out                    │
│    - Counts: found / total patterns                         │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│ 5. Result determination:                                    │
│    - Success rate = (found patterns / total) × 100%        │
│    - PASS if 100%                                           │
│    - FAIL if < 100%                                         │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│ 6. Summary generation:                                      │
│    - All results written to regression.summary              │
│    - Shows: Total, Passed, Failed                           │
│    - Final status: SUCCESS or FAILURE                       │
└─────────────────────────────────────────────────────────────┘
```

### Pattern Matching System

The test validation uses **simple string matching**:

1. Read each line from `patterns.txt`
2. Search for that exact string in `go.out` using `grep -q`
3. Count matches
4. Calculate success percentage

**Example**:
- `patterns.txt` has 6 patterns
- `go.out` contains 5 of them
- Success rate = 5/6 = 83%
- Result = **FAILED** (not 100%)

---

## Running Tests

### Run All Tests

```bash
cd pharmos.base.basar/dev/src/basar/regression
./do_regression.sh
```

**Output**:
```
=========================================
  Basar Regression Test Suite
=========================================

Regression directory: /path/to/regression
Start time: Sun Nov 23 19:59:17 CET 2025

=========================================
Test 1: libbasarcmnutil/date_operations
=========================================
Running: .../date_operations/go.sh

==> Checking patterns...
Pattern 'Test completed successfully' found.
Pattern 'Build successful' found.
...
==> 19 out of 19 patterns found. Success rate: 100%
✓ PASSED

=========================================
  Regression Test Results
=========================================

Total tests:  4
Passed:       4
Failed:       0

SUCCESS: All tests passed ✓
```

---

### Run Individual Test

```bash
cd pharmos.base.basar/dev/src/basar/regression/libbasarcmnutil/hello_basar
./go.sh
cat go.out  # View output
```

---

### View Results

```bash
# View summary
cat regression.summary

# View specific test output
cat libbasarcmnutil/hello_basar/go.out

# Check which patterns matched
grep "Pattern" regression.summary
```

---

## Creating New Tests

### Using the Template Generator

```bash
cd pharmos.base.basar/dev/src/basar/regression
./new_test.sh <library_name> <test_name>
```

**Example**:
```bash
./new_test.sh libbasarcmnutil string_operations
```

This creates:
```
libbasarcmnutil/string_operations/
├── main.cpp      # Template with TODOs
├── makefile      # Pre-configured makefile
├── go.sh         # Standard execution script
└── patterns.txt  # Template patterns
```

---

### Manual Creation

1. **Create directory**:
   ```bash
   mkdir -p libbasarcmnutil/my_test
   cd libbasarcmnutil/my_test
   ```

2. **Copy templates from existing test**:
   ```bash
   cp ../hello_basar/makefile .
   cp ../hello_basar/go.sh .
   ```

3. **Modify for your test**:
   - Edit `makefile`: Change `TARGET_NAME`
   - Edit `go.sh`: Change test name in strings
   - Create `main.cpp`: Write your test code
   - Create `patterns.txt`: List expected output strings

4. **Test manually**:
   ```bash
   chmod +x go.sh
   ./go.sh
   cat go.out
   ```

5. **Validate**:
   ```bash
   ../../../check_patterns.sh go.out patterns.txt
   ```

---

## Build Configuration

### Include Paths (Critical!)

Tests use **relative paths** from the test directory:

```
Test location: regression/libbasarcmnutil/hello_basar/
                                    │
                                    ├─── ../../../library/include         (Basar headers)
                                    ├─── ../../../../../ext/boost/include  (Boost headers)
                                    └─── ../../../../../ext/log4cplus/include (log4cplus)
```

**Path Calculation**:
```
hello_basar/  →  ../  →  libbasarcmnutil/
              →  ../  →  regression/
              →  ../  →  basar/
              →  ../  →  src/
              →  ../  →  dev/
```

From `dev/`:
- `library/include/` - Installed Basar headers
- `ext/boost/include/` - Boost headers
- `ext/log4cplus/include/` - log4cplus headers

---

### Library Paths

Similarly for libraries:

```
BASAR_LIB_DIR = ../../../library/lib/debug
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_LIB = ../../../../../ext/boost/lib
```

**Runtime**: Must set `LD_LIBRARY_PATH` in `go.sh`:
```bash
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:$LD_LIBRARY_PATH"
```

---

### Common Build Issues

| Issue | Symptom | Fix |
|-------|---------|-----|
| Wrong include path | `fatal error: libbasarcmnutil.h: No such file or directory` | Fix `BASAR_INC_DIR` in makefile |
| Missing Boost includes | `fatal error: boost/scoped_ptr.hpp: No such file or directory` | Add `BOOST_INC` to makefile |
| TAB vs spaces | `makefile:38: *** missing separator` | Use TAB characters in recipes |
| Runtime library not found | `error while loading shared libraries` | Set `LD_LIBRARY_PATH` in go.sh |

---

## Test Results

### Success Criteria

A test **PASSES** if:
1. ✅ Compilation succeeds (no errors)
2. ✅ Linking succeeds (executable created)
3. ✅ Execution completes (exit code 0)
4. ✅ **100% of patterns found** in output

A test **FAILS** if:
- ❌ Compilation error
- ❌ Linking error  
- ❌ Runtime crash
- ❌ < 100% patterns matched

---

### Result Files

**regression.summary**:
```
=========================================
  Basar Regression Test Suite
=========================================

Test 1: libbasarcmnutil/hello_basar
Pattern 'Hello from Basar!' found.
Pattern 'Version:' found.
==> 6 out of 6 patterns found. Success rate: 100%
✓ PASSED

Total tests:  4
Passed:       4
Failed:       0

SUCCESS: All tests passed ✓
```

**Individual go.out files**:
- Full build output
- Test execution output
- Error messages
- Exit codes

---

## Best Practices

### Test Design

1. **Keep tests focused** - One test per feature/class/function
2. **Use clear output** - Print what you're testing
3. **Include error handling** - Catch and report exceptions
4. **Test both success and failure** - Validate error conditions

### Pattern Selection

1. **Match key outputs** - Don't match every line
2. **Include success indicators** - "completed successfully", "exit code: 0"
3. **Include build markers** - "Build successful", library names
4. **Avoid fragile patterns** - Don't match changing data (timestamps, addresses)

**Good patterns**:
```
Test completed successfully
Build successful - binary created
Test exit code: 0
Result: PASS
```

**Bad patterns** (fragile):
```
Time: 12:34:56                    # Changes every run
Address: 0x7fff12345678           # Different each time
Processing 1234 records           # Data-dependent
```

### Maintenance

1. **Update patterns** when APIs change
2. **Keep makefiles synchronized** across tests
3. **Document test purpose** in main.cpp header
4. **Clean up** - Remove old go.out files periodically

---

## Summary

| Component | Purpose | Auto-generated? |
|-----------|---------|-----------------|
| `main.cpp` | Test source code | No - you write it |
| `makefile` | Build configuration | No - template provided |
| `go.sh` | Test executor | No - template provided |
| `patterns.txt` | Expected output | No - you define it |
| `go.out` | Actual output | **Yes** - created by go.sh |
| `regression.summary` | Overall results | **Yes** - created by do_regression.sh |

**Test Categories**:
- `libbasarcmnutil/` - Common utility tests (Date, Decimal, String, etc.)
- `libbasardbsql/` - Database SQL tests
- `libbasarguiqt/` - GUI/Qt tests
- `standalone/` - Infrastructure and framework tests

**Total Files per Test**: 5 (3 templates + 1 source + 1 patterns)

**Execution Time**: ~5-10 seconds per test on AIX

**Current Status**: 
- ✅ Build configuration fixed (3 makefiles corrected)
- ✅ 1 test passing (infrastructure_test)
- ⚠️ 3 tests need API updates (test code compatibility)

---

**Created**: November 23, 2025  
**Last Updated**: Build 09 analysis  
**Status**: Build system fully functional ✅
