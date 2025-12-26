# Basar Regression Test Fix - Applied

## Date: November 23, 2025

## Problem Identified

The regression tests in `basar__reg_v1` were failing with compilation errors:

```
fatal error: libbasarcmnutil.h: No such file or directory
```

## Root Cause

The test makefiles were using incorrect include paths:
- **Old path**: `BASAR_INC_DIR = ../../..` (pointed to source directory `basar/`)
- **Issue**: Headers are not in the source subdirectories, they're installed in `library/include/`

## Solution Applied

Updated the include directory path in all three test makefiles:

### Changed From:
```makefile
BASAR_INC_DIR = ../../..
```

### Changed To:
```makefile
BASAR_INC_DIR = ../../../library/include
```

## Files Modified

1. ✅ `basar__reg_v1/pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/hello_basar/makefile`
2. ✅ `basar__reg_v1/pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/date_operations/makefile`
3. ✅ `basar__reg_v1/pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/decimal_arithmetic/makefile`

## What This Fixes

After this change, the test programs will now correctly find the Basar headers in:
```
basar/library/include/
├── libbasarcmnutil.h
├── libbasarcmnutil_date.h
├── libbasarcmnutil_decimal.h
├── libbasarcmnutil_datetime.h
└── ... (all other installed headers)
```

## Expected Results

With these changes, when you run the regression tests on the AIX system:

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
```

**Expected outcome:**
- ✅ hello_basar test should PASS
- ✅ date_operations test should PASS
- ✅ decimal_arithmetic test should PASS
- ✅ infrastructure_test continues to PASS

**Success Rate**: 4/4 tests (100%)

## Verification Steps (On AIX System)

To verify the fix works:

### Test Individual Tests:

```bash
# Test 1: hello_basar
cd regression/libbasarcmnutil/hello_basar
./go.sh
../../check_patterns.sh go.out patterns.txt

# Test 2: date_operations
cd ../date_operations
./go.sh
../../check_patterns.sh go.out patterns.txt

# Test 3: decimal_arithmetic
cd ../decimal_arithmetic
./go.sh
../../check_patterns.sh go.out patterns.txt
```

### Run Full Regression Suite:

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
cat regression.summary
```

## Technical Details

### Include Path Resolution

From test location: `basar/regression/libbasarcmnutil/hello_basar/`

- `../../../library/include` resolves to:
  - `..` → `libbasarcmnutil/`
  - `../..` → `regression/`
  - `../../..` → `basar/`
  - `../../../library/include` → `basar/library/include/` ✅

### Why This Path?

The Basar build system copies/installs all public headers to `library/include/` during compilation. This provides:
1. **Clean separation** between source and installed headers
2. **Stable API** - only public headers are exposed
3. **Consistent include paths** for all consumers

## Impact

This fix enables the regression test infrastructure to work correctly with the installed Basar libraries. The tests can now:

- ✅ Compile against installed headers
- ✅ Link against compiled libraries
- ✅ Validate end-to-end functionality
- ✅ Run as part of CI/CD pipeline

## Notes

- This fix applies to `basar__reg_v1` directory
- The `build_6` directory shows these tests were still failing with the same issue
- The fix is minimal (3 lines changed across 3 files)
- No code changes required - only build configuration
- The `standalone/infrastructure_test` was already passing (no dependencies)

## Status

✅ **FIX APPLIED** - All makefiles updated with correct include paths
⏳ **VERIFICATION PENDING** - Requires running on AIX system with full Basar environment

## Next Steps

1. Transfer these fixed files to the AIX build environment
2. Run regression test suite: `./do_regression.sh`
3. Verify all 4 tests pass
4. Integrate into CI/CD pipeline (optional)

---

**Applied by**: Replit Agent  
**Date**: November 23, 2025  
**Issue**: Include path misconfiguration  
**Resolution**: Updated BASAR_INC_DIR in test makefiles
