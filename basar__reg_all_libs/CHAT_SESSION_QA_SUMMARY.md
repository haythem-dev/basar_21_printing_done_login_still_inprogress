# Basar Regression Test Analysis - Q&A Session Summary

## Session Date: November 26, 2025

---

## Q1: Qt5 Regression Test Pattern Analysis

**Question:** Analyze the Qt5 regression test pattern used in the Basar package.

**Response:** 
The Qt5 regression test pattern was analyzed. Each test follows a simple structure:
- `main.cpp` - Test source code
- `go.sh` - Build and run script
- `patterns.txt` - Expected output patterns for validation
- `.pro` file (Qt5) or `makefile` (Basar)

The pattern matching system uses `check_patterns.sh` to grep for expected patterns in `go.out`. Tests pass only if **100%** of patterns are found.

---

## Q2: How Did You Explain Errors in Logs?

**Question:** How did you explain errors in log?

**Response:**
Three distinct error scenarios were identified and explained:

### Scenario 1: On Replit (Linux)
```
g++: error: unrecognized command-line option '-maix64'
```
- **Cause:** AIX-specific compiler flags don't work on Linux
- **Result:** 0% pattern match (nothing compiles)

### Scenario 2: On AIX - Standalone Tests
- Build succeeds
- Program runs normally
- **Result:** 100% pattern match

### Scenario 3: On AIX - Basar Library Tests
```
ld: 0711-224 WARNING: Duplicate symbol: .__init_aix_libgcc_cxa_atexit
```
- Build succeeds
- Program crashes before main() executes
- **Result:** 15-50% pattern match (only build patterns found)

---

## Q3: Example of Runtime Error (libBml.a)

**Question:** [User provided log showing libBml.a error]
```
exec(): 0509-036 Cannot load program ./connection_basic
        0509-022 Cannot load module libBml.a.
        0509-026 System error: A file or directory in the path name does not exist.
```

**Response:**
This error shows the dependency chain:
```
connection_basic
    └── libbasardbsql.so
            └── libBml.a  ❌ NOT FOUND (IBM Informix ESQL library)
```

**Solution:** Add CICS library path to LIBPATH:
```bash
CICS_LIB="/software/ae/marathon/lib/tcaccess/8.0.20.1/lib"
export LIBPATH="${CICS_LIB}:$LIBPATH"
```

The user confirmed they added this fix to go.sh.

---

## Q4: go.sh is OK - Confirmation

**Question:** even go.sh is ok

**Response:**
Confirmed that all test infrastructure components are correctly implemented:

| Component | Status |
|-----------|--------|
| go.sh scripts | ✅ Correct |
| check_patterns.sh | ✅ Correct |
| do_regression.sh | ✅ Correct |
| main.cpp test code | ✅ Correct |
| makefiles | ✅ Correct |

The failures are purely environmental (missing dependencies), not code issues.

---

## Q5: Test Works via go.sh but Fails Directly

**Question:** [User showed that `./go.sh` works but `./connection_basic` directly fails with libBml.a error]

**Response:**
Explained shell environment variable behavior:

```
┌─────────────────────────────────────────────────────────────┐
│  Your Shell Session                                         │
│  LIBPATH = (not set)                                        │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  ./go.sh (creates subshell)                         │   │
│  │  export LIBPATH="/software/ae/..."                  │   │
│  │  ./connection_basic  ← Works (LIBPATH set)          │   │
│  └─────────────────────────────────────────────────────┘   │
│         ↑                                                   │
│    Script ends → LIBPATH setting disappears                │
│                                                             │
│  ./connection_basic  ← Fails (LIBPATH not set)             │
└─────────────────────────────────────────────────────────────┘
```

The `export` in go.sh only affects the script's process. This is expected behavior - tests should be run via go.sh.

---

## Q6: Give Me the Export Command

**Question:** ok .. give me the export ?

**Response:**
```bash
export LIBPATH="/software/ae/marathon/lib/tcaccess/8.0.20.1/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:../../../../../ext/openssl/lib:$LIBPATH"
```

---

## Q7: Program Runs But No Output (MARATHON_LOG_CONFIGDIR)

**Question:** [User showed program runs but only displays logging warning, no test output]
```
connection_basic:
basar.cmnutil.Logging.Logging [libbasarcmnutil_logging.cpp:91]:
    environment variable <MARATHON_LOG_CONFIGDIR> not set
```

**Response:**
Analyzed the Basar library source code (`libbasarcmnutil_logging.cpp`):

```cpp
if ( ! CONFIGDIR )
{
    debug::printError(msg.c_str(), execNameShort.c_str());
    outofScope = true;
}
// ... later ...
if (outofScope)
    exit(EXIT_FAILURE);  // ← Program exits here!
```

**Root Cause:** The Basar library calls `exit()` during static initialization if `MARATHON_LOG_CONFIGDIR` is not set. The main() function never executes.

---

## Q8: But Output is on Console!

**Question:** but output is on console !!

**Response:**
Clarified the execution flow:

```
Program starts
    ↓
Library static initialization runs
    ↓
Logging() constructor called
    ↓
Checks MARATHON_LOG_CONFIGDIR → NOT SET
    ↓
Prints error: "environment variable not set"
    ↓
exit(EXIT_FAILURE);  ← PROGRAM ENDS HERE
    ↓
main() NEVER REACHED
    ↓
Your cout statements never execute
```

The console output seen is from the **library's error message**, not from the test's main().

---

## Q9: Let's Proceed with Option 1 (Set Env Vars)

**Question:** lets proceed with option 1

**Response:**
Updated all 6 go.sh files for Basar library tests with logging configuration:

### Files Updated:
1. libbasarcmnutil/hello_basar/go.sh
2. libbasarcmnutil/date_operations/go.sh
3. libbasarcmnutil/decimal_arithmetic/go.sh
4. libbasardbsql/connection_basic/go.sh
5. libbasardbsql/database_info_test/go.sh
6. libbasardbsql/exception_handling/go.sh

### Added Configuration:
```bash
# Setup logging configuration (required by Basar libraries)
LOG_CONFIG_DIR="/tmp/basar_regression/config"
LOG_LOGS_DIR="/tmp/basar_regression/logs"
mkdir -p "$LOG_CONFIG_DIR" "$LOG_LOGS_DIR"

export MARATHON_LOG_CONFIGDIR="$LOG_CONFIG_DIR"
export MARATHON_LOG_LOGSDIR="$LOG_LOGS_DIR"

# Create minimal config file for this test
cat > "$LOG_CONFIG_DIR/<test_name>.cfg" << 'EOF'
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
log4cplus.appender.APPEND_STDOUT.layout=log4cplus::PatternLayout
log4cplus.appender.APPEND_STDOUT.layout.ConversionPattern=%m%n
EOF
```

---

## Summary of Issues Identified and Resolved

| Issue | Root Cause | Solution |
|-------|------------|----------|
| `-maix64` error on Replit | AIX flags on Linux | Run on AIX (expected) |
| libBml.a not found | LIBPATH missing CICS path | Added CICS_LIB to go.sh |
| Direct binary execution fails | LIBPATH not set in shell | Use go.sh (sets environment) |
| Program exits before main() | MARATHON_LOG_CONFIGDIR not set | Added logging config to go.sh |

---

## Key Learnings

1. **go.sh encapsulates environment setup** - Always run tests via go.sh, not directly
2. **Basar libraries require logging config** - Without `MARATHON_LOG_CONFIGDIR`, library calls `exit()`
3. **Pattern matching requires 100%** - Even 99% match = FAIL
4. **Standalone tests prove infrastructure** - They pass because they have no Basar library dependencies

---

## Files Modified in This Session

1. `libbasardbsql/connection_basic/go.sh` - Added logging config
2. `libbasarcmnutil/hello_basar/go.sh` - Added logging config
3. `libbasarcmnutil/date_operations/go.sh` - Added logging config
4. `libbasarcmnutil/decimal_arithmetic/go.sh` - Added logging config
5. `libbasardbsql/database_info_test/go.sh` - Added logging config
6. `libbasardbsql/exception_handling/go.sh` - Added logging config
7. `QT5_PATTERN_BASAR_ADAPTATION_SUMMARY.md` - Created comprehensive analysis

---

*Document generated: November 26, 2025*
