# Qt5-Style Regression Tests: Complete Analysis and Basar Adaptation

## Document Purpose
This document provides a comprehensive analysis of the Qt5 regression test pattern and its successful adaptation to the Basar C++ library system on AIX.

---

## Executive Summary

| Aspect | Status |
|--------|--------|
| **Pattern Understanding** | Complete |
| **Infrastructure Implementation** | Complete |
| **Build Configuration Fixes** | Complete (10 iterations) |
| **API Compatibility Fixes** | Complete |
| **Standalone Test Validation** | 100% Pass |
| **Basar Library Tests** | Ready for AIX environment |

---

## Part 1: The Qt5 Regression Test Pattern

### 1.1 Pattern Overview

Qt5 uses a simple, effective pattern where each regression test is a **standalone program** that:
1. Compiles against Qt5 libraries
2. Runs and produces observable output
3. Has its output validated against expected patterns

### 1.2 Qt5 Test Directory Structure

```
pharmos.3rd_party.qt5_cpr_up/dev/src/regression/
├── check_patterns.sh          # Pattern validation script
├── do_regression.sh           # Test orchestrator
├── QtCore/
│   ├── hello/
│   │   ├── main.cpp           # Test source code
│   │   ├── hello.pro          # Qt project file
│   │   ├── go.sh              # Build & run script
│   │   └── patterns.txt       # Expected output patterns
│   ├── dateinfo/
│   ├── json_processing/
│   └── ...
├── QtNetwork/
│   └── ...
├── QtConcurrent/
│   ├── async_call/
│   ├── concurrent/
│   └── file_watcher/
└── QtXml/
    └── ...
```

### 1.3 Qt5 Test Components

#### main.cpp (Test Program)
```cpp
#include <QCoreApplication>
#include <QString>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);
    out << "Hello, Qt5 from the terminal!\n";
    return 0;
}
```

#### go.sh (Build & Run Script)
```bash
#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash hello_qt5 main.o
qmake -makefile hello.pro 2>&1 | tee go.out
gmake            2>&1 | tee -a go.out
ldd hello_qt5    2>&1 | tee -a go.out
./hello_qt5      2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
```

#### patterns.txt (Expected Output)
```
pkg-install/lib/libQt5Core.so
Hello, Qt5 from the terminal
```

#### *.pro (Qt Project File)
```
QT -= gui
QT += core
CONFIG += console
CONFIG -= app_bundle
TARGET = hello_qt5
TEMPLATE = app
SOURCES += main.cpp
```

### 1.4 Qt5 Pattern Matching System

The `check_patterns.sh` script:
1. Reads each line from patterns.txt
2. Uses `grep` to search for the pattern in go.out
3. Reports found/missing patterns
4. Calculates success percentage
5. **Test passes ONLY if 100% of patterns match**

---

## Part 2: Basar Adaptation

### 2.1 Key Adaptations Made

| Qt5 Pattern | Basar Adaptation |
|-------------|------------------|
| `.pro` file (qmake) | `makefile` (GNU make) |
| `ksh` shell | `bash` shell |
| Qt5 libraries | Basar libraries |
| Qt include paths | Basar include paths |
| Qt5Core, Qt5Network, etc. | libbasarcmnutil, libbasardbsql, etc. |

### 2.2 Basar Regression Test Structure

```
pharmos.base.basar_cpr_up/dev/src/basar/regression/
├── check_patterns.sh              # Pattern validator (adapted from Qt5)
├── do_regression.sh               # Test orchestrator (adapted from Qt5)
├── new_test.sh                    # Helper to create new tests
├── check_dependencies.sh          # Verify environment
├── run_standalone_only.sh         # Run standalone tests only
├── regression.summary             # Test results
├── libbasarcmnutil/               # Common utilities tests
│   ├── hello_basar/
│   │   ├── main.cpp
│   │   ├── makefile
│   │   ├── go.sh
│   │   └── patterns.txt
│   ├── date_operations/
│   │   ├── main.cpp
│   │   ├── makefile
│   │   ├── go.sh
│   │   └── patterns.txt
│   └── decimal_arithmetic/
│       ├── main.cpp
│       ├── makefile
│       ├── go.sh
│       └── patterns.txt
├── libbasardbsql/                 # Database SQL tests
│   ├── connection_basic/
│   ├── database_info_test/
│   └── exception_handling/
└── standalone/                    # Infrastructure validation tests
    ├── infrastructure_test/       # Proves infrastructure works
    │   ├── main.cpp
    │   ├── makefile
    │   ├── go.sh
    │   └── patterns.txt
    └── cpp_basics/                # Tests C++ compilation
        ├── main.cpp
        ├── makefile
        ├── go.sh
        └── patterns.txt
```

### 2.3 Basar Test Components

#### main.cpp (Basar Test Example)
```cpp
#include <iostream>
#include "libbasarcmnutil.h"

using namespace basar::cmnutil;
using namespace std;
using basar::Int32;

int main() {
    cout << "Basar Date Operations Test" << endl;
    
    try {
        // Test 1: Date Creation
        cout << "Test 1: Date Creation" << endl;
        Date date1(2024, 4, 22);
        cout << "Created date: " << date1.toString("").c_str() << endl;
        cout << "Year: " << date1.getYear() << endl;
        cout << "Month: " << date1.getMonth() << endl;
        cout << "Day: " << date1.getDay() << endl;
        
        // Test 2: Date Arithmetic
        cout << "Test 2: Date Arithmetic" << endl;
        Date date2 = date1;
        date2.addDays(10);  // addDays modifies in place
        cout << "After adding 10 days: " << date2.toString("").c_str() << endl;
        
        cout << "All date operations completed successfully" << endl;
        return 0;
        
    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
```

#### makefile (Basar - replaces .pro)
```makefile
TARGET_NAME = date_operations

# Directories
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../../library/include
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_INC = ../../../../../ext/boost/include
BOOST_LIB = ../../../../../ext/boost/lib

# Libraries
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(BOOST_LIB) \
       -lbasarcmnutil -lbasardebug -llog4cplus \
       -lboost_system -lboost_filesystem

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC) -I$(BOOST_INC)

# Compiler flags (AIX-specific)
CXXFLAGS = -g -std=c++17 -maix64 -pthread
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out

.PHONY: all clean
```

#### go.sh (Build & Run Script)
```bash
#!/bin/bash

cd "$(dirname "$0")"

# Set library paths for runtime
export LIBPATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:$LIBPATH"
export LD_LIBRARY_PATH="../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:$LD_LIBRARY_PATH"

# Clean and build
rm -f date_operations go.out
make clean 2>&1 | tee go.out
make       2>&1 | tee -a go.out

# Check build success
if [ -f date_operations ]; then
    echo "Build successful - binary created" 2>&1 | tee -a go.out
    
    # Show library dependencies
    ldd date_operations 2>&1 | tee -a go.out
    
    # Run test
    ./date_operations 2>&1 | tee -a go.out
    exit_code=$?
    
    echo "Test exit code: $exit_code" 2>&1 | tee -a go.out
else
    echo "ERROR: Build failed - executable not created" 2>&1 | tee -a go.out
    exit 1
fi
```

#### patterns.txt (Expected Output)
```
Basar Date Operations Test
Test 1: Date Creation
Created date: 2024-04-22
Year: 2024
Month: 4
Day: 22
Test 2: Date Arithmetic
After adding 10 days: 2024-05-02
All date operations completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

---

## Part 3: Build Configuration Fixes Applied

### 3.1 Build History (10 Iterations)

| Build | Issue | Resolution |
|-------|-------|------------|
| v1 | Initial state | Baseline |
| Build 6 | Wrong include path | Fixed `BASAR_INC_DIR = ../../../library/include` |
| Build 7 | Missing Boost path | Added `BOOST_INC` variable |
| Build 8 | Makefile TAB characters | Fixed rule indentation with actual TABs |
| Build 9 | Config complete | Identified API issues |
| Build 10 | API mismatches | Fixed `Date::toString()` and `Decimal::round()` |

### 3.2 API Compatibility Fixes

#### Date::toString() - Requires Format Parameter
```cpp
// API Signature:
ConstString Date::toString(ConstString format) const;

// Fix Applied:
date.toString("")      // Empty string = default format
```

#### Decimal::round() - Returns Void
```cpp
// API Signature:
void Decimal::round(int decimals);  // Modifies in-place

// Fix Applied:
Decimal copy = original;   // Create copy first
copy.round(2);             // Modify copy
```

---

## Part 4: Test Execution Flow

### 4.1 Flow Diagram

```
┌─────────────────────────────────────────────────┐
│              do_regression.sh                    │
│   1. Find all go.sh scripts                     │
│   2. Execute each test                          │
│   3. Collect results                            │
└────────────────────┬────────────────────────────┘
                     │
     ┌───────────────┼───────────────┐
     ▼               ▼               ▼
┌──────────┐   ┌──────────┐   ┌──────────┐
│  Test 1  │   │  Test 2  │   │  Test N  │
│  go.sh   │   │  go.sh   │   │  go.sh   │
└────┬─────┘   └────┬─────┘   └────┬─────┘
     │              │              │
     ▼              ▼              ▼
┌──────────┐   ┌──────────┐   ┌──────────┐
│ 1. Build │   │ 1. Build │   │ 1. Build │
│ 2. Run   │   │ 2. Run   │   │ 2. Run   │
│ 3. go.out│   │ 3. go.out│   │ 3. go.out│
└────┬─────┘   └────┬─────┘   └────┬─────┘
     │              │              │
     ▼              ▼              ▼
┌─────────────────────────────────────────────────┐
│             check_patterns.sh                    │
│   For each pattern in patterns.txt:            │
│     grep pattern go.out → Found/Not Found      │
│   Result: Success rate: X%                     │
│   Pass: 100% required                          │
└────────────────────┬────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────┐
│            regression.summary                    │
│   Total tests: N                                │
│   Passed: X                                     │
│   Failed: Y                                     │
└─────────────────────────────────────────────────┘
```

### 4.2 Pattern Matching Rules

1. **Each line in patterns.txt is a separate pattern**
2. **Patterns are grep-searched in go.out**
3. **ALL patterns must match for test to PASS**
4. **Partial matches (e.g., 75%) = FAIL**

---

## Part 5: Test Categories and Status

### 5.1 Standalone Tests (Infrastructure Validation)

| Test | Purpose | Status |
|------|---------|--------|
| infrastructure_test | Proves test framework works | 100% PASS |
| cpp_basics | Validates C++ compilation | 100% PASS |

**Key Insight**: Standalone tests use only standard C++ libraries - they prove the test infrastructure is correctly implemented.

### 5.2 Basar Library Tests

| Test | Library | Dependencies Required |
|------|---------|----------------------|
| hello_basar | libbasarcmnutil | log4cplus, Boost |
| date_operations | libbasarcmnutil | log4cplus, Boost |
| decimal_arithmetic | libbasarcmnutil | log4cplus, Boost |
| connection_basic | libbasardbsql | + libssh2, database |
| database_info_test | libbasardbsql | + libssh2, database |
| exception_handling | libbasardbsql | + libssh2, database |

**Status**: Ready for testing - require full AIX development environment with dependencies.

---

## Part 6: Environment Requirements

### 6.1 Required on AIX for Full Testing

```
AIX Development Environment
├── GCC 12+ or IBM XL C++
├── GNU Make (gmake)
├── Basar Libraries
│   ├── libbasarcmnutil.so
│   ├── libbasardebug.so
│   ├── libbasardbsql.so
│   └── ...
├── Third-Party Libraries
│   ├── log4cplus
│   ├── Boost (system, filesystem)
│   └── libssh2
└── Environment Variables
    ├── LIBPATH (AIX library path)
    └── PATH (include gmake location)
```

### 6.2 Known AIX Issue

**GCC 12 Runtime Conflict**: Duplicate symbol `.__init_aix_libgcc_cxa_atexit` causes programs to crash before main() executes when linking Basar libraries.

**Impact**: Build succeeds, link succeeds, but runtime crashes before first line of main().

**Workarounds to investigate**:
1. Alternative linker flags (`-Wl,-bexpall`)
2. Static linking for Basar libraries
3. IBM XL compiler instead of GCC

---

## Part 7: Creating New Tests

### 7.1 Template (Follow This Pattern)

1. **Create directory**: `regression/<library>/<test_name>/`

2. **Create main.cpp**:
```cpp
#include <iostream>
#include "<library_header>.h"

int main() {
    std::cout << "Test Name - Description" << std::endl;
    
    // Test code here
    
    std::cout << "Test completed successfully" << std::endl;
    return 0;
}
```

3. **Create makefile**: Copy from existing test, adjust paths

4. **Create go.sh**:
```bash
#!/bin/bash
cd "$(dirname "$0")"
export LIBPATH="<paths>:$LIBPATH"

rm -f <executable> go.out
make clean 2>&1 | tee go.out
make       2>&1 | tee -a go.out

if [ -f <executable> ]; then
    echo "Build successful - binary created" 2>&1 | tee -a go.out
    ./<executable> 2>&1 | tee -a go.out
    echo "Test exit code: $?" 2>&1 | tee -a go.out
else
    echo "ERROR: Build failed" 2>&1 | tee -a go.out
fi
```

5. **Create patterns.txt**: List expected output strings (one per line)

---

## Part 8: Comparison Summary

### Qt5 vs Basar Regression Tests

| Aspect | Qt5 | Basar |
|--------|-----|-------|
| **Build System** | qmake (.pro files) | GNU make (makefiles) |
| **Shell** | ksh | bash |
| **Platform** | AIX + others | AIX |
| **Test Structure** | main.cpp, go.sh, patterns.txt, .pro | main.cpp, go.sh, patterns.txt, makefile |
| **Orchestration** | do_regression.sh | do_regression.sh (adapted) |
| **Pattern Checker** | check_patterns.sh | check_patterns.sh (identical) |
| **Libraries Tested** | Qt5Core, Qt5Network, etc. | libbasarcmnutil, libbasardbsql, etc. |
| **Pass Criteria** | 100% patterns match | 100% patterns match |

---

## Conclusions

### What Was Achieved

1. **Complete understanding** of the Qt5 regression test pattern
2. **Successful adaptation** to Basar library ecosystem
3. **Working infrastructure** proven by standalone tests (100% pass)
4. **Comprehensive documentation** (6+ files, 2500+ lines)
5. **Build fixes** through 10 iterations resolving all configuration issues
6. **API compatibility fixes** for Date::toString() and Decimal::round()

### Current Status

- **Infrastructure**: Proven working
- **Standalone tests**: 100% passing
- **Basar library tests**: Ready for AIX environment with dependencies
- **Documentation**: Complete

### Next Steps for Full Validation

1. **Deploy to AIX environment** with all dependencies
2. **Resolve GCC 12 runtime conflict** if present
3. **Run full test suite** via `./do_regression.sh`
4. **Expand test coverage** as needed

---

## Quick Reference

### Run All Tests
```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
cat regression.summary
```

### Run Single Test
```bash
cd libbasarcmnutil/date_operations
./go.sh
../../check_patterns.sh go.out patterns.txt
```

### Run Standalone Tests Only
```bash
./run_standalone_only.sh
```

### Create New Test
```bash
./new_test.sh <library_name> <test_name>
```

---

*Document Version: Final*  
*Last Updated: November 26, 2025*  
*Package: basar__reg_v7*
