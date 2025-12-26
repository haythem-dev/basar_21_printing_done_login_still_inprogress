# Pattern Matching Analysis - Why Tests Fail

## Overview
**Total Patterns Expected**: 84 patterns across 8 tests  
**Total Patterns Found (Replit)**: ~0-3 patterns per test  
**Success Rate (Replit)**: 0% - 16%  
**Success Rate (AIX Build 16)**: 15% - 100% (varies by test)

---

## 🔍 Root Cause Analysis

### **The Pattern Matching System**

Each test has TWO files:
1. **main.cpp** - Program that SHOULD produce output
2. **patterns.txt** - Expected output strings to match
3. **go.out** - ACTUAL output from running the test

The regression framework runs:
```bash
./go.sh                          # Build and run test → produces go.out
./check_patterns.sh go.out patterns.txt   # Match patterns
```

**Pattern match succeeds ONLY if ALL patterns found in go.out!**

---

## 📊 Test-by-Test Analysis

### Test 1: `date_operations` (19 patterns)

#### **Expected Patterns:**
```
Basar Date Operations Test
Test 1: Date Creation
Created date: 2024-04-22
Year: 2024
Month: 4
Day: 22
Test 2: Date Arithmetic
After adding 10 days: 2024-05-02
Test 3: Calculate Days Between Dates
Days from date1 to date2: 10
Test 4: Date Validation
2024-02-29 is valid: yes
2023-02-29 is valid: no
Test 5: Current Date
Today is valid: yes
All date operations completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

#### **Actual Output (Replit):**
```
g++: error: unrecognized command-line option '-maix64'
make: *** [makefile:34: date_operations] Error 1
ERROR: Build failed - executable not created
```

#### **Actual Output (AIX Build 16):**
```
g++ -g -std=c++17 -maix64 ... (build command)
ld: 0711-224 WARNING: Duplicate symbol: .__init_aix_libgcc_cxa_atexit
Build successful - binary created
libbasarcmnutil.so
(NO PROGRAM OUTPUT - CRASHED BEFORE main())
```

#### **Why Patterns Not Found:**

**On Replit (Linux):**
- ❌ Compilation fails (AIX flags on Linux)
- ❌ Program never runs
- ✅ Found 0/19 patterns (0%)

**On AIX (Build 16):**
- ✅ Compilation succeeds
- ❌ Runtime crash before main()
- ✅ Found 3/19 patterns (15%)
  - "Build successful - binary created" ✓
  - "Test exit code: 0" ✓
  - "libbasarcmnutil.so" ✓
- ❌ Missing: ALL runtime output patterns (program never executes)

---

### Test 2: `decimal_arithmetic` (16 patterns)

#### **Expected Patterns:**
```
Basar Decimal Arithmetic Test
Test 1: Decimal Creation
Decimal from string '123.45': 123.45
Test 2: Decimal Addition
123.45 + 67.89 = 191.34
Test 3: Decimal Subtraction
123.45 - 67.89 = 55.56
Test 4: Decimal Multiplication
Test 5: Decimal Comparison
100.00 < 200.00: true
200.00 > 100.00: true
Test 6: Decimal Precision
All decimal operations completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

#### **Actual Output (AIX Build 16):**
```
Build successful - binary created  ✓
Test exit code: 0  ✓
libbasarcmnutil.so  ✓
(NO "Basar Decimal Arithmetic Test" - program crashed!)
```

#### **Why Patterns Not Found:**
- ✅ Found 3/16 patterns (18%)
- ❌ Missing: 13 runtime output patterns
- **Root Cause**: Program crashes at runtime before line 8 executes:
  ```cpp
  int main() {
      cout << "Basar Decimal Arithmetic Test" << endl;  // Line 8 - NEVER EXECUTES!
  ```

---

### Test 3: `hello_basar` (6 patterns)

#### **Expected Patterns:**
```
Hello from Basar!
Version:
Test completed successfully
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so
```

#### **Code Analysis:**
```cpp
int main() {
    cout << "Hello from Basar!" << endl;  // Line 7 - FIRST LINE!
    // ... rest of code
}
```

**Line 7 is BEFORE any library calls**, yet it's NOT in output!

#### **Why Patterns Not Found:**
- ✅ Found 3/6 patterns (50%) on AIX
- ❌ "Hello from Basar!" NOT found
- **Critical**: Crash happens BEFORE line 7 of main()
- **Conclusion**: Crash during C++ runtime initialization, not in user code

---

### Test 4: `standalone/infrastructure_test` (6 patterns)

#### **Expected Patterns:**
```
(empty line)
Build successful - binary created
Basar Regression Infrastructure Test
Test execution: PASS
Exit code: 0
Test execution completed with exit code 0
```

#### **Actual Output (AIX Build 16):**
```
Build successful - binary created  ✓
Basar Regression Infrastructure Test  ✓
Test execution: PASS  ✓
Exit code: 0  ✓
Test execution completed with exit code 0  ✓
```

#### **Why This Test PASSES:**
- ✅ Found 6/6 patterns (100%)
- **Key difference**: NO Basar libraries linked!
- Uses ONLY standard C++ `<iostream>`
- No runtime initialization conflicts

---

### Test 5: `standalone/cpp_basics` (9 patterns)

#### **Expected Patterns:**
```
(empty line)
Build successful - binary created
C++ Basics Test
String test: Hello
Vector size: 5
Sum: 15
All tests passed
Exit code: 0
Test execution completed with exit code 0
```

#### **Status:**
- **Build 14**: Failed (makefile TAB error)
- **Build 16**: ✅ PASSES (100%)
- Uses NO Basar libraries
- Proves infrastructure works perfectly

---

## 🎯 Pattern Categories

### **Category A: Build Patterns** (Always Present if Build Succeeds)
```
Build successful - binary created
Test exit code: 0
libbasarcmnutil.so (or other library)
```

**Found Rate:**
- On successful build: 100%
- On failed build: 0%

---

### **Category B: Runtime Patterns** (Program Output)
```
Basar Date Operations Test
Test 1: Date Creation
Created date: 2024-04-22
...all cout statements...
All operations completed successfully
```

**Found Rate:**
- Standalone tests (no Basar libs): 100%
- Basar library tests on AIX: **0%** (programs crash)
- Basar library tests on Replit: **0%** (don't compile)

---

## 📈 Pattern Match Summary Table

| Test | Total Patterns | Build Patterns | Runtime Patterns | Found (Replit) | Found (AIX B16) | Success Rate (AIX) |
|------|----------------|----------------|------------------|----------------|-----------------|-------------------|
| date_operations | 19 | 3 | 16 | 0 | 3 | 15% |
| decimal_arithmetic | 16 | 3 | 13 | 0 | 3 | 18% |
| hello_basar | 6 | 3 | 3 | 0 | 3 | 50% |
| connection_basic | 8 | 3 | 5 | 0 | ? | Unknown |
| database_info_test | 11 | 2 | 9 | 0 | ? | Unknown |
| exception_handling | 9 | 2 | 7 | 0 | ? | Unknown |
| cpp_basics | 9 | 2 | 7 | 0 | 9 | 100% ✅ |
| infrastructure_test | 6 | 2 | 4 | 0 | 6 | 100% ✅ |

---

## 🔴 Why Patterns Are Not Found - The Complete Story

### **Scenario 1: On Replit (Current Environment)**

```
Test starts
  ↓
Makefile runs: g++ ... -maix64 ...
  ↓
ERROR: unrecognized command-line option '-maix64'
  ↓
Build fails - no executable created
  ↓
go.out contains ONLY build error messages
  ↓
Pattern matching: 0/N patterns found
```

**Patterns Missing:** ALL (100%)  
**Why:** AIX compiler flags don't work on Linux

---

### **Scenario 2: On AIX - Standalone Tests (Build 16)**

```
Test starts
  ↓
Makefile runs: g++ ... -maix64 ...  ✓ (works on AIX)
  ↓
Build succeeds
  ↓
Program runs normally
  ↓
cout statements execute
  ↓
go.out contains full program output
  ↓
Pattern matching: 9/9 patterns found ✓
```

**Patterns Found:** 100%  
**Why:** No Basar libraries = no conflicts

---

### **Scenario 3: On AIX - Basar Library Tests (Build 16)**

```
Test starts
  ↓
Makefile runs: g++ ... -maix64 ... -lbasarcmnutil ...
  ↓
Build succeeds ✓
  ↓
Linker WARNING: Duplicate symbol .__init_aix_libgcc_cxa_atexit ⚠️
  ↓
Program starts loading
  ↓
C++ runtime initialization begins
  ↓
💥 CRASH - duplicate symbol conflict
  ↓
Program exits BEFORE main() executes
  ↓
Exit code: 0 (silent crash)
  ↓
go.out contains ONLY build log, NO program output
  ↓
Pattern matching: 3/N patterns found (build patterns only)
```

**Patterns Found:** 15-50% (only build patterns)  
**Patterns Missing:** ALL runtime patterns  
**Why:** Program crashes before executing ANY cout statements

---

## 🎯 **The Critical Insight**

### **Pattern Match Requirement: 100% or FAIL**

The regression framework requires **ALL** patterns to be found:

```bash
# From check_patterns.sh
if [ $found_count -eq $total_count ]; then
    echo "✓ PASSED"
else
    echo "✗ FAILED"  # Even if 99% match!
fi
```

**This is why:**
- date_operations: 3/19 = FAIL (even though 15% found)
- decimal_arithmetic: 3/16 = FAIL (even though 18% found)
- hello_basar: 3/6 = FAIL (even though 50% found)
- cpp_basics: 9/9 = PASS ✓
- infrastructure_test: 6/6 = PASS ✓

---

## 💡 Solutions

### **For Testing on Replit (Short-term)**

Remove AIX-specific flags from makefiles:

```makefile
# BEFORE (AIX-specific):
CXXFLAGS = -g -std=c++17 -maix64 -pthread
LDFLAGS = -maix64 -pthread -Wl,-brtl

# AFTER (Linux-compatible):
CXXFLAGS = -g -std=c++17 -pthread
LDFLAGS = -pthread
```

**Result:** Tests will compile on Replit BUT still need Basar libraries

---

### **For Testing on AIX (Long-term)**

Fix the GCC 12 runtime library conflict:

**Option 1: Alternative Linker Flags**
```makefile
LDFLAGS = -maix64 -pthread -Wl,-brtl -Wl,-bexpall
```

**Option 2: Static Linking**
```makefile
LDFLAGS = -maix64 -pthread -Wl,-bstatic -lbasarcmnutil -Wl,-bdynamic
```

**Option 3: Use IBM XL Compiler Instead of GCC**
```makefile
CXX = xlC_r
CXXFLAGS = -q64 -qlanglvl=extended
LDFLAGS = -q64
```

---

## 📋 Adding New Library Tests - Critical Warnings

### **⚠️ BEFORE Adding libbasardbsql, libbasarguiqt, etc. Tests:**

1. **Fix the runtime crash first!** All new tests will fail with same issue

2. **Expect this pattern:**
   - ✅ Build: Succeeds
   - ✅ Link: Shows library linked
   - ❌ Runtime: NO OUTPUT (crashes before main)
   - ❌ Patterns: Only 3-5 build patterns found (0% runtime patterns)

3. **Test progression:**
   ```
   Step 1: Add test code (follows pattern perfectly)
   Step 2: Run on AIX → Builds successfully
   Step 3: Check go.out → No program output
   Step 4: Check patterns → 15-30% match (build only)
   Step 5: Result: FAIL (same as existing tests)
   ```

### **✅ Safe to Add (Will Work):**
- Standalone C++ tests (no Basar libraries)
- Tests using ONLY standard library
- Infrastructure validation tests

### **❌ Will Fail (Same Runtime Crash):**
- libbasardbsql tests
- libbasarguiqt tests (+ Qt dependency issues)
- libbasarlogin tests
- ANY test linking Basar shared libraries

---

## 🎯 Conclusion

**Why Many Patterns Not Found:**

1. **On Replit**: Platform mismatch (AIX flags on Linux) → 0% compile
2. **On AIX**: Runtime library conflict → 15-50% (build patterns only, NO runtime output)
3. **Pattern system**: Requires 100% match → Even 99% = FAIL

**The Fix Priority:**
1. **First**: Solve GCC 12 duplicate symbol issue on AIX
2. **Second**: Verify existing tests pass (100% patterns)
3. **Third**: Add new library tests

**Until step 1 is fixed, adding new tests will just create more failures with the same root cause.**
