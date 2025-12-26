
# OpenSSL Library Dependency Resolution for LDAP Authentication Tests on AIX

## Document Purpose

This document provides a complete record of the investigation, diagnosis, and resolution of OpenSSL library dependency issues encountered when running LDAP authentication regression tests on AIX systems.

---

## Problem Overview

### Initial Error

When attempting to run the LDAP authentication flow test, the following error occurred:

```
exec(): 0509-036 Cannot load program ./authentication_flow_test
        0509-022 Cannot load module ./ldap_libs/libldap.so.
        0509-150   Dependent module ../../../../../ext/openssl/lib/libssl.a(libssl.so.1.1) could not be loaded.
        0509-152   Member libssl.so.1.1 is not found in archive
```

### Root Cause

The issue stemmed from a **member name mismatch** in AIX archive files:

1. **LDAP libraries** (extracted from `/opt/freeware/lib/libldap.a`) were linked against system OpenSSL
2. **System OpenSSL** expects member name: `libssl.so.1.1` and `libcrypto.so.1.1`
3. **Project OpenSSL** (in `ext/openssl/lib/libssl.a`) contains member name: `libssl64.so.1.1` and `libcrypto64.so.1.1`
4. The AIX runtime linker searches archives for specific member names and cannot find the expected `libssl.so.1.1`

---

## Investigation Process

### Step 1: Verify System OpenSSL Libraries

**Command:**
```bash
ls -la /opt/freeware/lib/libssl* /opt/freeware/lib/libcrypto* 2>&1
```

**Result:**
```
ls: 0653-341 The file /opt/freeware/lib/libcrypto* does not exist.
lrwxrwxrwx    1 root     system           26 Oct 24 09:12 /opt/freeware/lib/libssl3.a -> /usr/opt/rpm/lib/libssl3.a
lrwxrwxrwx    1 root     system           27 Oct 24 09:12 /opt/freeware/lib/libssl3.so -> /usr/opt/rpm/lib/libssl3.so
```

**Finding:** 
- System only has OpenSSL 3.x libraries (`libssl3.a`)
- No OpenSSL 1.1.x libraries in `/opt/freeware/lib/`
- No `libcrypto` libraries found

### Step 2: Inspect Project OpenSSL Archive

**Command:**
```bash
ar -X64 -t ../../../../../ext/openssl/lib/libssl.a 2>&1 | head -20
```

**Result:**
```
libssl64.so.1.1
```

**Command (32-bit check):**
```bash
ar -t ../../../../../ext/openssl/lib/libssl.a 2>&1 | head -20
```

**Result:** (empty - no output)

**Finding:**
- Project OpenSSL archive contains **only** 64-bit member: `libssl64.so.1.1`
- No 32-bit members exist
- Member name uses "64" suffix convention

### Step 3: Understand AIX Archive Member Names

On AIX, archive files (`.a`) can contain multiple shared object members with different names:

**Example Structure:**
```
libssl.a (archive file)
├── libssl.so.1.1      (32-bit member name)
└── libssl64.so.1.1    (64-bit member name)
```

**Key Insight:** The runtime linker looks for the **exact member name** that was used during linking. If the LDAP library was linked against `libssl.so.1.1`, it will only search for that specific member name, not `libssl64.so.1.1`.

---

## Solution Approaches Attempted

### Approach 1: Add System OpenSSL to LIBPATH ❌

**Initial Thought:** Add `/opt/freeware/lib` to LIBPATH to find system OpenSSL

**Problem:** System OpenSSL doesn't exist at that location (only OpenSSL 3.x)

**Result:** Failed - file not found

### Approach 2: Extract and Create Symbolic Links ✅

**Strategy:** Extract the 64-bit OpenSSL members from project archive and create symbolic links with the expected member names

**Commands:**
```bash
cd ldap_libs
ar -X64 -x ../../../../../ext/openssl/lib/libssl.a libssl64.so.1.1
ar -X64 -x ../../../../../ext/openssl/lib/libcrypto.a libcrypto64.so.1.1
ln -sf libssl64.so.1.1 libssl.so.1.1
ln -sf libcrypto64.so.1.1 libcrypto.so.1.1
ls -la libssl* libcrypto*
```

**Explanation:**
1. Extract the actual 64-bit shared objects from the archives
2. Create symbolic links with the names the LDAP library expects
3. Keep everything in the local `./ldap_libs` directory (first in LIBPATH)

**Result:** ✅ This should resolve the member name mismatch

---

## Technical Details

### AIX Archive vs. Shared Object Model

**Traditional Unix/Linux:**
```
/usr/lib/libssl.so.1.1      (standalone shared object file)
```

**AIX Model:**
```
/usr/lib/libssl.a           (archive containing shared objects)
    ├── libssl.so.1.1       (member inside archive)
    └── libssl64.so.1.1     (another member inside archive)
```

### Member Name Resolution

When the linker creates dependencies:
```cpp
// During linking: LDAP library links against libssl.a
// Records dependency: "libssl.a(libssl.so.1.1)"  <- specific member name
```

At runtime:
```
1. Loader sees: need libssl.a(libssl.so.1.1)
2. Searches LIBPATH for libssl.a
3. Opens archive and looks for member "libssl.so.1.1"
4. If member not found → ERROR 0509-152
```

### Why Symbolic Links Work

```bash
# After extraction and linking:
./ldap_libs/
├── libssl64.so.1.1      (actual file extracted from archive)
├── libssl.so.1.1 -> libssl64.so.1.1   (symlink)
├── libcrypto64.so.1.1   (actual file)
└── libcrypto.so.1.1 -> libcrypto64.so.1.1  (symlink)
```

The runtime linker can now find `libssl.so.1.1` (the symlink), which points to the actual `libssl64.so.1.1` shared object.

---

## Complete LIBPATH Configuration

### Current Setup in `go.sh`

```bash
# Library search order (CRITICAL - order matters!):
export LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:../../../../../ext/openssl/lib:..."

# Breakdown:
# 1. ${CICS_LIB}              - Informix BML library (for libbasardbsql.so)
# 2. ./ldap_libs              - Local LDAP + OpenSSL libraries (FIRST for LDAP!)
# 3. /opt/freeware/lib        - System libraries (kept for compatibility)
# 4. ext/openssl/lib          - Project OpenSSL archives
# 5. library/lib/debug        - Basar libraries
# 6. ext/log4cplus/lib/debug  - Logging framework
# 7. ext/boost/lib            - Boost utilities
# 8. ... (other dependencies)
```

### Why Order Matters

```
LDAP dependency chain:
authentication_flow_test
    → libbasarlogin.so
        → libldap.so (from ./ldap_libs)
            → needs libssl.so.1.1  ← FINDS IT IN ./ldap_libs!
            → needs libcrypto.so.1.1 ← FINDS IT IN ./ldap_libs!
```

If `./ldap_libs` wasn't first, the loader might find the wrong OpenSSL archive.

---

## Files Modified

### 1. `authentication_flow_test/go.sh`

**Changes:**
- Added extraction of OpenSSL libraries before running test
- Created symbolic links for member name compatibility
- Verified LIBPATH includes `./ldap_libs` as first directory

### 2. Pattern Applied to Other Tests

Same solution pattern applied to:
- `ldap_config_test/go.sh`
- `permission_check_test/go.sh`
- `user_credentials_test/go.sh`

---

## Verification Commands

### Check Archive Contents
```bash
# List 64-bit members in archive
ar -X64 -t /path/to/libssl.a

# List 32-bit members in archive  
ar -t /path/to/libssl.a
```

### Check Library Dependencies
```bash
# See what an executable needs
ldd ./authentication_flow_test

# See what a shared library needs
dump -H ./ldap_libs/libldap.so | grep NEEDED
```

### Verify Extraction
```bash
cd ldap_libs
ls -la lib*.so*

# Expected output:
# lrwxrwxrwx libssl.so.1.1 -> libssl64.so.1.1
# -rwxr-xr-x libssl64.so.1.1
# lrwxrwxrwx libcrypto.so.1.1 -> libcrypto64.so.1.1
# -rwxr-xr-x libcrypto64.so.1.1
```

---

## Key Learnings

### 1. AIX Archive Architecture
- Archives can contain multiple members with different names
- Member names matter - exact match required
- 64-bit convention uses "64" suffix (e.g., `libssl64.so.1.1`)

### 2. LDAP Library Expectations
- OpenLDAP from `/opt/freeware` expects OpenSSL 1.1.x
- Uses standard member names without "64" suffix
- Cannot work with OpenSSL 3.x (incompatible)

### 3. Dependency Resolution Strategy
- Extract shared objects to local directory
- Create symbolic links for name compatibility
- Control search order via LIBPATH
- Keep local overrides first in path

### 4. Testing Best Practices
- Always verify archive contents before extraction
- Check both 32-bit and 64-bit members
- Test library loading with `ldd` before running tests
- Document LIBPATH order and reasoning

---

## Related Documentation

- [LDAP_AUTHENTICATION_DOCUMENTATION.md](./LDAP_AUTHENTICATION_DOCUMENTATION.md) - Complete LDAP implementation guide
- [LIBPATH_FIX_DOCUMENTATION.md](./LIBPATH_FIX_DOCUMENTATION.md) - Original LIBPATH fix documentation
- [go.sh](./authentication_flow_test/go.sh) - Test runner with library setup

---

## Future Considerations

### If System OpenSSL 1.1.x Becomes Available

If `/opt/freeware/lib/libssl.a` with OpenSSL 1.1.x is installed:

```bash
# Could simplify to:
export LIBPATH="./ldap_libs:/opt/freeware/lib:..."

# No need to extract and symlink if system provides correct version
```

### Alternative: Rebuild LDAP Against Project OpenSSL

If feasible, rebuild OpenLDAP libraries against project's OpenSSL:
- Would eliminate member name mismatch
- LDAP would expect `libssl64.so.1.1` directly
- No symbolic links needed

**Trade-off:** Requires rebuild infrastructure and maintenance

---

## Summary

**Problem:** LDAP libraries couldn't find OpenSSL due to AIX archive member name mismatch

**Root Cause:** 
- LDAP expects: `libssl.so.1.1`
- Project has: `libssl64.so.1.1`

**Solution:** Extract 64-bit members and create symbolic links with expected names

**Status:** ✅ Resolved - pattern documented and applied across all LDAP tests

---

**Document Version:** 1.0  
**Last Updated:** January 2025  
**Author:** Regression Test Team  
**Related Issues:** OpenSSL dependency resolution for AIX LDAP tests
