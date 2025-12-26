
# Adding Basar Library Regression Tests

## Overview

This guide explains how to add regression tests for Basar's internal libraries (like `libbasarcmnutil`, `libbasardbsql`, `libbasarguiqt`, etc.) using the automated test template generator `new_test.sh`.

**Note:** These are tests for Basar's own libraries developed by Pharmos, not third-party external dependencies like Qt5, Boost, or log4cplus.

## Prerequisites

- Access to the Basar regression directory: `pharmos.base.basar_cpr_up/dev/src/basar/regression`
- Basic understanding of C++ and makefiles
- The third-party library must be built and available in the library path

## Supported Libraries

The regression framework currently supports:

- **libbasarcmnutil** - Common utilities (Date, Decimal, Logging, etc.)
- **libbasardbsql** - Database SQL operations
- **libbasardbaspect** - Database aspect-oriented programming
- **libbasarguiqt** - Qt5 GUI components
- **libbasarlogin** - Login/authentication functionality

---

## Step-by-Step Process

### 1. Navigate to the Regression Directory

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
```

### 2. Run the Test Template Generator

Use the `new_test.sh` script to automatically create a complete test template:

```bash
./new_test.sh <library_name> <test_name>
```

**Example for libbasarcmnutil:**

```bash
./new_test.sh libbasarcmnutil decimal_operations
```

**Example for libbasardbsql:**

```bash
./new_test.sh libbasardbsql connection_pool_test
```

**Example for libbasarguiqt:**

```bash
./new_test.sh libbasarguiqt main_window_test
```

### 3. What Gets Created Automatically

The script creates a directory structure:

```
libbasarcmnutil/
└── decimal_operations/
    ├── main.cpp          # Test program skeleton
    ├── makefile          # Build configuration
    ├── go.sh             # Build and run script
    └── patterns.txt      # Expected output patterns
```

---

## Detailed Example: Adding a libbasarcmnutil Test

### Step 1: Generate the Template

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./new_test.sh libbasarcmnutil decimal_operations
```

**Output:**
```
Creating test directory: libbasarcmnutil/decimal_operations
Creating main.cpp...
Creating makefile...
Creating go.sh...
Creating patterns.txt...

Test template created successfully!

Next steps:
  1. Edit libbasarcmnutil/decimal_operations/main.cpp - add your test code
  2. Update libbasarcmnutil/decimal_operations/patterns.txt - add expected output patterns
  3. If needed, update libbasarcmnutil/decimal_operations/makefile - adjust libraries/includes
  4. Run: cd libbasarcmnutil/decimal_operations && ./go.sh
  5. Validate: ../../check_patterns.sh go.out patterns.txt
```

### Step 2: Edit main.cpp

Navigate to the test directory and edit the main program:

```bash
cd libbasarcmnutil/decimal_operations
```

Replace the TODO section in `main.cpp` with your actual test code:

```cpp
#include "libbasarcmnutil/libbasarcmnutil.h"
#include <iostream>

using namespace basar::cmnutil;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: decimal_operations" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test Decimal arithmetic operations
        std::cout << "Testing Decimal arithmetic..." << std::endl;
        
        Decimal dec1("123.45");
        Decimal dec2("67.89");
        
        std::cout << "dec1 = " << dec1.toString().c_str() << std::endl;
        std::cout << "dec2 = " << dec2.toString().c_str() << std::endl;
        
        Decimal sum = dec1 + dec2;
        std::cout << "Addition: " << sum.toString().c_str() << std::endl;
        
        Decimal diff = dec1 - dec2;
        std::cout << "Subtraction: " << diff.toString().c_str() << std::endl;
        
        Decimal prod = dec1 * dec2;
        std::cout << "Multiplication: " << prod.toString().c_str() << std::endl;
        
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
```

### Step 3: Update patterns.txt

Edit `patterns.txt` to include expected output strings (one per line):

```
Basar Regression Test: decimal_operations
Testing Decimal arithmetic
dec1 = 123.45
dec2 = 67.89
Addition: 191.34
Subtraction: 55.56
Multiplication:
Test completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

### Step 4: Verify/Adjust Makefile (Usually No Changes Needed)

The generated makefile should work for most cases. Review it:

```makefile
TARGET_NAME = decimal_operations

# Directories
BASAR_LIB_DIR = ../../../../library/lib/debug
BASAR_INC_DIR = ../../../..
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_LIB = ../../../../../ext/boost/lib

# Libraries
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(BOOST_LIB) \
       -lbasarcmnutil -lbasardebug -llog4cplus \
       -lboost_system -lboost_filesystem

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC)

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# Linker flags
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out
```

**Note:** For other libraries, you may need to add additional `-l` flags.

### Step 5: Run the Test

```bash
./go.sh
```

This script will:
1. Clean previous builds
2. Compile the test program
3. Run the test
4. Capture all output to `go.out`

### Step 6: Validate Output

Check if the test output matches expected patterns:

```bash
../../check_patterns.sh go.out patterns.txt
```

**Expected output:**
```
Checking go.out for patterns from patterns.txt

Pattern 'Basar Regression Test: decimal_operations' found.
Pattern 'Testing Decimal arithmetic' found.
Pattern 'dec1 = 123.45' found.
Pattern 'dec2 = 67.89' found.
Pattern 'Addition: 191.34' found.
Pattern 'Subtraction: 55.56' found.
Pattern 'Multiplication:' found.
Pattern 'Test completed successfully' found.
Pattern 'Build successful - binary created' found.
Pattern 'Test exit code: 0' found.
Pattern 'libbasarcmnutil.so' found.

11 out of 11 patterns found. Success rate: 100%
```

---

## Special Considerations for Different Libraries

### libbasardbsql Tests

For database tests, you may need to configure database connection info:

```cpp
#include "libbasardbsql/libbasardbsql.h"

using namespace basar::dbsql;

int main(int argc, char* argv[])
{
    try {
        std::cout << "Testing DatabaseInfo configuration..." << std::endl;
        
        DatabaseInfo dbInfo;
        dbInfo.setODBC("test_dsn");
        std::cout << "ODBC DSN set successfully" << std::endl;
        
        DatabaseInfo dbInfoInfx;
        dbInfoInfx.setInfx("test_server", "test_database");
        std::cout << "Informix configuration set successfully" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
```

**Makefile adjustment:** Add database libraries:

```makefile
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) \
       -lbasardbsql -lbasarcmnutil -lbasardebug -llog4cplus \
       -lodbc  # Add ODBC library
```

### libbasarguiqt Tests (Qt5 GUI)

For Qt5 GUI tests, you need to:

1. **Include Qt5 headers:**

```cpp
#include "libbasarguiqt/libbasarguiqt.h"
#include <QApplication>

using namespace basar::guiqt;

int main(int argc, char* argv[])
{
    try {
        std::cout << "Testing libbasarguiqt..." << std::endl;
        
        // Initialize Qt application
        QApplication app(argc, argv);
        
        // Test Manager initialization
        std::cout << "Initializing Manager..." << std::endl;
        Manager::instance();
        std::cout << "Manager initialized successfully" << std::endl;
        
        // Test MainBackground window
        MainBackground* mainBg = new MainBackground();
        mainBg->setWindowTitle("Basar Test");
        std::cout << "Main window created" << std::endl;
        
        delete mainBg;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
```

2. **Update makefile for Qt5:**

```makefile
# Add Qt5 paths
QT5_INC = ../../../../../ext/qt5/include
QT5_LIB = ../../../../../ext/qt5/lib

# Libraries
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(QT5_LIB) \
       -lbasarguiqt -lbasarcmnutil -lbasardebug -llog4cplus \
       -lQt5Core -lQt5Gui -lQt5Widgets

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC) -I$(QT5_INC)

# May need -fPIC flag
CXXFLAGS = -g -std=c++17 -maix64 -pthread -fPIC
```

---

## Common Test Patterns

### 1. Version Information Test

```cpp
std::cout << "Version: " << basar::cmnutil::getVersion() << std::endl;
std::cout << "Full Version: " << basar::cmnutil::getFullVersion().c_str() << std::endl;
```

### 2. Date/Time Operations Test

```cpp
Date today = Date::getCurrent();
std::cout << "Today: " << today.toString("").c_str() << std::endl;

DateTime now = DateTime::getCurrent();
std::cout << "Now: " << now.toString("").c_str() << std::endl;
```

### 3. Exception Handling Test

```cpp
try {
    // Trigger an expected exception
    Date invalidDate("invalid");
}
catch (const basar::Exception& e) {
    std::cout << "Expected exception caught: " << e.what().c_str() << std::endl;
}
```

---

## Running All Regression Tests

After adding your test, run the complete regression suite:

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
```

This will:
- Run all tests in all library directories
- Generate a summary report: `regression.summary`

---

## Troubleshooting

### Issue 1: Build Fails - Library Not Found

**Error:**
```
ld: cannot find -lbasarcmnutil
```

**Solution:**
Ensure the Basar libraries are built:

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar
make all
```

### Issue 2: Runtime Error - Shared Library Not Loaded

**Error:**
```
error while loading shared libraries: libbasarcmnutil.so
```

**Solution:**
The `go.sh` script should set `LD_LIBRARY_PATH`. Verify it contains:

```bash
export LD_LIBRARY_PATH="../../../library/lib/debug:$LD_LIBRARY_PATH"
```

### Issue 3: Pattern Mismatch

**Error:**
```
Pattern 'Test completed successfully' NOT found.
Success rate: 90%
```

**Solution:**
- Check `go.out` for actual output
- Adjust `patterns.txt` to match actual output
- Ensure patterns are exact matches (case-sensitive)

### Issue 4: Test Crashes Before Output

**Problem:** No output in `go.out` but exit code is 0

**Solution:**
- Check for static initialization errors
- Run test manually with `gdb` or `dbx`
- Add more verbose output earlier in main()

---

## Best Practices

1. **Keep Tests Simple:** Each test should verify one specific functionality
2. **Use Meaningful Names:** Test names should describe what they test
3. **Document Expected Behavior:** Add comments explaining what should happen
4. **Test Error Cases:** Include tests for exception handling
5. **Verify Library Linking:** Always check `ldd` output in `go.out`
6. **Pattern Precision:** Use specific patterns that won't match unintended output
7. **Clean Output:** Make test output clear and readable

---

## AIX vs Windows Script Versions

### Platform-Specific Differences

There are now **two separate scripts** for creating regression tests:

1. **`new_test.sh`** - Original/Windows version
2. **`new_test_aix.sh`** - AIX-specific version (recommended for AIX 7.2)

#### Key Difference: Heredoc and Variable Handling

**Windows Version (`new_test.sh`):**
- Uses heredoc with direct variable substitution: `cat > makefile << EOF`
- Makefile variables like `${TEST_NAME}` are expanded by bash during heredoc creation
- Works reliably on Windows/Git Bash and Linux

**AIX Version (`new_test_aix.sh`):**
- Uses heredoc with quoted EOF marker: `cat > makefile << 'EOF'`
- Prevents shell variable expansion within the heredoc
- Uses placeholder strings (e.g., `TEST_NAME_PLACEHOLDER`) that are replaced via `sed` after heredoc
- **Critical fix:** Eliminates duplicate variable assignments that appeared after EOF in older versions

### What Was Wrong with the Old AIX Script

The original `new_test.sh` script had lines 132-163 containing duplicate makefile variable assignments **outside** the heredoc block:

```bash
# Lines 90-130: Heredoc creates makefile
cat > makefile << 'EOF'
TARGET_NAME = ${TEST_NAME}
BASAR_LIB_DIR = ../../../library/lib/debug
# ... rest of makefile content ...
EOF

# Lines 132-163: DUPLICATE ASSIGNMENTS (WRONG!)
BASAR_LIB_DIR = ../../../library/lib/debug  # ← Executed as shell command!
BASAR_INC_DIR = ../../..                     # ← Executed as shell command!
LOG4CPLUS_INC = ../../../../ext/log4cplus/include
# ... etc
```

**Why this caused errors on AIX:**
- These lines appeared **after** the heredoc's `EOF` marker
- The shell tried to **execute** them as shell commands (not makefile content)
- Result: `command not found` and `syntax error` messages

### Common AIX Errors (Now Resolved)

If you encounter these errors, you're using the old buggy version:

```bash
./new_test.sh: line 132: BASAR_LIB_DIR: command not found
./new_test.sh: line 133: BASAR_INC_DIR: command not found
./new_test.sh: line 134: LOG4CPLUS_INC: command not found
./new_test.sh: line 135: LOG4CPLUS_LIB: command not found
./new_test.sh: line 138: syntax error near unexpected token `('
```

**Solution:** Use `new_test_aix.sh` instead, which:
- ✅ Has NO duplicate variable assignments after EOF
- ✅ Uses placeholders + `sed` replacement strategy
- ✅ Generates clean makefiles without shell syntax errors

### Verification on AIX

To verify the AIX script works correctly on AIX 7.2:

```bash
# Navigate to regression directory
cd pharmos.base.basar_cpr_up/dev/src/basar/regression

# Create a test using AIX-specific script
./new_test_aix.sh libbasarcmnutil test_verify

# Expected output (no errors):
Creating test directory: libbasarcmnutil/test_verify
Creating main.cpp...
Creating makefile...
Creating go.sh...
Creating patterns.txt...

Test template created successfully!
```

**Recommended Usage by Platform:**
- **AIX 7.2**: Use `./new_test_aix.sh` (eliminates shell syntax errors)
- **Windows/Linux**: Use `./new_test.sh` (original version)
- **Both work**: But AIX version is more robust for AIX environments

### AIX-Specific Makefile Configuration

The generated makefiles on AIX include these critical flags:

```makefile
# 64-bit AIX compilation
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# AIX runtime linking for shared libraries
LDFLAGS = -maix64 -pthread -Wl,-brtl
```

**Important AIX Flags:**
- `-maix64`: Generate 64-bit AIX object files
- `-Wl,-brtl`: Enable runtime linking (required for .so files on AIX)
- `-pthread`: POSIX threads support

### Platform Compatibility Matrix

| Platform | Shell | GCC Version | Status |
|----------|-------|-------------|--------|
| Windows (Git Bash) | bash 4.4+ | MinGW GCC 8+ | ✅ Tested |
| AIX 7.2 | ksh/bash | GCC 12 | ✅ Tested |
| Linux | bash | GCC 8+ | ✅ Compatible |

---

## Quick Reference

### Create New Test

**On AIX 7.2:**
```bash
./new_test_aix.sh <library> <test_name>
```

**On Windows/Linux:**
```bash
./new_test.sh <library> <test_name>
```

### Run Single Test
```bash
cd <library>/<test_name>
./go.sh
```

### Validate Test Output
```bash
../../check_patterns.sh go.out patterns.txt
```

### Run All Tests
```bash
./do_regression.sh
```

### View Test Results
```bash
cat regression.summary
```

---

## Summary

The `new_test_aix.sh` (AIX) and `new_test.sh` (Windows/Linux) scripts automate the creation of regression tests for Basar libraries. The process is:

1. **Generate template:**
   - AIX: `./new_test_aix.sh libbasarcmnutil my_test`
   - Windows/Linux: `./new_test.sh libbasarcmnutil my_test`
2. Edit `main.cpp` to implement test logic
3. Update `patterns.txt` with expected output
4. Adjust `makefile` if needed (additional libraries)
5. Run: `./go.sh`
6. Validate: `../../check_patterns.sh go.out patterns.txt`

This ensures consistent test structure and makes it easy to add comprehensive regression coverage for all Basar libraries.

**Cross-Platform Notes:**
- **AIX 7.2**: Use `new_test_aix.sh` to avoid shell syntax errors from duplicate variable assignments
- **Windows/Linux**: Use `new_test.sh` (original version works fine)
- Both scripts generate identical test structures, only differing in their internal heredoc handling
