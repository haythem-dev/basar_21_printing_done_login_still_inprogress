# Build 15 - Quick Reference

## What Was Fixed ✅

### 1. Compilation Error (date_operations)
**Error**: `Int32 was not declared in this scope`  
**Fix**: Added `using basar::Int32;` at line 7  
**File**: `date_operations/main.cpp`

### 2. Makefile Syntax Error (cpp_basics)
**Error**: `missing separator (TAB vs spaces)`  
**Fix**: Replaced spaces with TAB characters  
**File**: `cpp_basics/makefile`

---

## What Cannot Be Fixed (Without AIX Investigation) ⚠️

### 3. Runtime Crashes (decimal_arithmetic, hello_basar)
**Issue**: Programs crash before producing any output  
**Cause**: Library loading failure or GCC/AIX runtime conflict  
**Evidence**: Linker warning `Duplicate symbol: .__init_aix_libgcc_cxa_atexit`

**Requires**:
- Check library dependencies on AIX
- Debug with actual error output from go.out files
- Investigate GCC 12 / AIX 7.2 compatibility

---

## Expected Results

### Minimum (60% pass rate)
- ✅ date_operations: PASS (namespace fixed)
- ✅ cpp_basics: PASS (makefile fixed)
- ✅ infrastructure_test: PASS (already working)
- ❌ decimal_arithmetic: FAIL (runtime crash)
- ❌ hello_basar: FAIL (runtime crash)

### Optimistic (100% pass rate)
If runtime crashes resolve on AIX:
- ✅ All 5 tests PASS

---

## Next Steps

1. Deploy to AIX 7.2
2. Run `./do_regression.sh`
3. If runtime crashes persist:
   - Check `go.out` files for error messages
   - Verify library dependencies with `ldd`
   - Debug with AIX debugger `dbx`

---

## Files Modified

1. `pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/date_operations/main.cpp`
2. `pharmos.base.basar_cpr_up/dev/src/basar/regression/standalone/cpp_basics/makefile`

**Total Changes**: 3 lines across 2 files
