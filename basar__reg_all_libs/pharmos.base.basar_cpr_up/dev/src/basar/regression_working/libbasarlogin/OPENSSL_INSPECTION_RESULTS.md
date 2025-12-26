
# OpenSSL Library Dependency Inspection Results

## Document Purpose

This document provides a complete record of all inspection commands, outputs, and analysis performed during the resolution of OpenSSL library dependency issues for LDAP authentication tests on AIX.

---

## Initial Problem

**Error:**
```
exec(): 0509-036 Cannot load program ./authentication_flow_test
        0509-022 Cannot load module ./ldap_libs/libldap.so.
        0509-150   Dependent module ../../../../../ext/openssl/lib/libssl.a(libssl.so.1.1) could not be loaded.
        0509-152   Member libssl.so.1.1 is not found in archive
```

**Interpretation:** The AIX runtime linker is looking for a specific member name (`libssl.so.1.1`) inside the OpenSSL archive, but the project's archive contains a different member name (`libssl64.so.1.1`).

---

## System Inspection

### Check System OpenSSL Libraries

**Command:**
```bash
ls -la /opt/freeware/lib/libssl* /opt/freeware/lib/libcrypto* 2>&1
```

**Output:**
```
ls: 0653-341 The file /opt/freeware/lib/libcrypto* does not exist.
lrwxrwxrwx    1 root     system           26 Oct 24 09:12 /opt/freeware/lib/libssl3.a -> /usr/opt/rpm/lib/libssl3.a
lrwxrwxrwx    1 root     system           27 Oct 24 09:12 /opt/freeware/lib/libssl3.so -> /usr/opt/rpm/lib/libssl3.so
```

**Findings:**
- ❌ No OpenSSL 1.1.x libraries in `/opt/freeware/lib/`
- ❌ No `libcrypto` libraries found
- ✅ Only OpenSSL 3.x libraries present (`libssl3.a`, `libssl3.so`)
- ⚠️ OpenSSL 3.x is incompatible with LDAP libraries built against OpenSSL 1.1.x

---

## Project OpenSSL Archive Inspection

### Check 64-bit Members

**Command:**
```bash
ar -X64 -t ../../../../../ext/openssl/lib/libssl.a 2>&1 | head -20
```

**Output:**
```
libssl64.so.1.1
```

**Findings:**
- ✅ Contains one 64-bit member: `libssl64.so.1.1`
- ⚠️ Member name uses "64" suffix convention
- ⚠️ LDAP expects: `libssl.so.1.1` (without "64" suffix)

### Check 32-bit Members

**Command:**
```bash
ar -t ../../../../../ext/openssl/lib/libssl.a 2>&1 | head -20
```

**Output:**
```
(empty - no output)
```

**Findings:**
- ❌ No 32-bit members in the archive
- ✅ Confirms archive only contains 64-bit objects

### Check libcrypto Archive

**Command:**
```bash
ar -X64 -t ../../../../../ext/openssl/lib/libcrypto.a 2>&1 | head -20
```

**Expected Output:**
```
libcrypto64.so.1.1
```

---

## Solution Implementation

### Step 1: Extract OpenSSL Libraries

**Commands:**
```bash
cd ldap_libs
ar -X64 -x ../../../../../ext/openssl/lib/libssl.a libssl64.so.1.1
ar -X64 -x ../../../../../ext/openssl/lib/libcrypto.a libcrypto64.so.1.1
```

**Purpose:** Extract the 64-bit shared objects from the project's OpenSSL archives.

### Step 2: Create Symbolic Links

**Commands:**
```bash
ln -sf libssl64.so.1.1 libssl.so.1.1
ln -sf libcrypto64.so.1.1 libcrypto.so.1.1
```

**Purpose:** Create symbolic links with the member names that LDAP libraries expect.

### Step 3: Verify Extraction

**Command:**
```bash
ls -la libssl* libcrypto*
```

**Expected Output:**
```
-rwxr-xr-x  1 user group 1572864 Jan 15 14:30 libssl64.so.1.1
lrwxrwxrwx  1 user group      15 Jan 15 14:30 libssl.so.1.1 -> libssl64.so.1.1
-rwxr-xr-x  1 user group 5242880 Jan 15 14:30 libcrypto64.so.1.1
lrwxrwxrwx  1 user group      18 Jan 15 14:30 libcrypto.so.1.1 -> libcrypto64.so.1.1
```

---

## Actual Inspection Results (from libssl_inspection.txt)

### Extraction and Linking

**Commands Executed:**
```bash
h.benabdelaziz@znbs21:/software/home/benabdelaziz/basar_regression/pharmos.base.basar/dev/src/basar/regression/libbasarlogin_01/authentication_flow_test # ar -X64 -x ../../../../../ext/openssl/lib/libssl.a libssl64.so.1.1
h.benabdelaziz@znbs21:/software/home/benabdelaziz/basar_regression/pharmos.base.basar/dev/src/basar/regression/libbasarlogin_01/authentication_flow_test # ar -X64 -x ../../../../../ext/openssl/lib/libcrypto.a libcrypto64.so.1.1
h.benabdelaziz@znbs21:/software/home/benabdelaziz/basar_regression/pharmos.base.basar/dev/src/basar/regression/libbasarlogin_01/authentication_flow_test # ln -sf libssl64.so.1.1 libssl.so.1.1
h.benabdelaziz@znbs21:/software/home/benabdelaziz/basar_regression/pharmos.base.basar/dev/src/basar/registration/libbasarlogin_01/authentication_flow_test # ln -sf libcrypto64.so.1.1 libcrypto.so.1.1
```

**Verification:**
```bash
h.benabdelaziz@znbs21:/software/home/benabdelaziz/basar_regression/pharmos.base.basar/dev/src/basar/regression/libbasarlogin_01/authentication_flow_test # ls -la libssl* libcrypto*
-rwxr-xr-x    1 benabdel system      5144576 Jan 15 14:27 libcrypto64.so.1.1
lrwxrwxrwx    1 benabdel system           18 Jan 15 14:27 libcrypto.so.1.1 -> libcrypto64.so.1.1
-rwxr-xr-x    1 benabdel system      1572864 Jan 15 14:27 libssl64.so.1.1
lrwxrwxrwx    1 benabdel system           15 Jan 15 14:27 libssl.so.1.1 -> libssl64.so.1.1
```

**Status:** ✅ **SUCCESS** - Files extracted and symbolic links created successfully

### Build Test

**Command:**
```bash
./go.sh
```

**Compilation Output:**
```
ld: 0711-224 WARNING: Duplicate symbol: .operator new[](unsigned long)
ld: 0711-224 WARNING: Duplicate symbol: .operator new(unsigned long)
ld: 0711-345 Use the -bloadmap or -bnoquiet option to obtain more information.
```

**Status:** ✅ **SUCCESS** - Build completed (warnings are acceptable)

### Runtime Test

**Error Encountered:**
```
exec(): 0509-036 Cannot load program ./authentication_flow_test because of the following errors:
        0509-022 Cannot load module ./ldap_libs/libldap.so.
        0509-150   Dependent module ../../../../../ext/openssl/lib/libssl.a(libssl.so.1.1) could not be loaded.
        0509-152   Member libssl.so.1.1 is not found in archive
```

**Status:** ❌ **FAILED** - Runtime linker still looking in archive, not finding standalone files

---

## Root Cause Analysis

### Why Symbolic Links Alone Don't Work

1. **Archive Reference in Dependency Chain:**
   - LDAP library was linked with: `../../../../../ext/openssl/lib/libssl.a`
   - Dependency recorded as: `libssl.a(libssl.so.1.1)` - expecting member inside archive
   - Linker looks for the archive file, then searches for the member inside it

2. **Standalone Files Not Searched:**
   - The symbolic links were created as standalone files in the current directory
   - AIX runtime linker doesn't search for standalone `.so` files when dependency is recorded as archive member
   - The LIBPATH mechanism searches for the archive, not for standalone shared objects

3. **Location Issue:**
   - Files were extracted to the main test directory instead of `ldap_libs/`
   - LIBPATH has `./ldap_libs` first, but that's only relevant if files are actually there

---

## Corrected Solution

### Move Libraries to Correct Location

**Commands:**
```bash
# Move extracted files and symbolic links to ldap_libs directory
cd authentication_flow_test
mv libssl64.so.1.1 libcrypto64.so.1.1 libssl.so.1.1 libcrypto.so.1.1 ldap_libs/
```

### Expected Directory Structure

```
authentication_flow_test/
├── ldap_libs/
│   ├── libldap-2.5.so.0          (extracted from /opt/freeware/lib/libldap.a)
│   ├── libldap.so -> libldap-2.5.so.0
│   ├── liblber-2.5.so.0          (extracted from /opt/freeware/lib/liblber.a)
│   ├── liblber.so -> liblber-2.5.so.0
│   ├── libssl64.so.1.1           (extracted from project OpenSSL)
│   ├── libssl.so.1.1 -> libssl64.so.1.1
│   ├── libcrypto64.so.1.1        (extracted from project OpenSSL)
│   └── libcrypto.so.1.1 -> libcrypto64.so.1.1
├── go.sh
└── authentication_flow_test
```

### LIBPATH Configuration

The `go.sh` already sets LIBPATH correctly:
```bash
export LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:..."
```

With `./ldap_libs` first, all dependencies should be found:
- `libldap.so` → found in `./ldap_libs/`
- `libssl.so.1.1` → found in `./ldap_libs/` (symbolic link)
- `libcrypto.so.1.1` → found in `./ldap_libs/` (symbolic link)

---

## Key Learnings

### AIX Archive vs. Standalone Shared Objects

**Problem:** AIX uses a hybrid model where shared objects can be:
1. **Members inside archives** (`.a` files containing `.so` members)
2. **Standalone files** (`.so` files in directories)

**Dependency Recording:** When linking, the recorded dependency format determines search behavior:
- `libssl.a(libssl.so.1.1)` → searches for archive, then member inside
- `libssl.so.1.1` → searches for standalone file

**Resolution:** Extract shared objects and ensure they're in a directory that's early in LIBPATH.

### Member Name Conventions

- **32-bit members:** `libssl.so.1.1`, `libcrypto.so.1.1`
- **64-bit members:** `libssl64.so.1.1`, `libcrypto64.so.1.1`
- **Suffix convention:** AIX adds "64" suffix for 64-bit objects

### Symbolic Links on AIX

- ✅ Work for resolving member name mismatches
- ✅ Allow one file to satisfy multiple expected names
- ⚠️ Must be in correct location (directory in LIBPATH)
- ⚠️ Must be standalone files, not inside archives

---

## Verification Commands

### After Proper Setup

```bash
# Check library locations
ls -la ldap_libs/

# Check dependencies
ldd ./authentication_flow_test | grep -E "libssl|libcrypto|libldap|liblber"

# Run the test
./go.sh
```

### Expected Success Indicators

1. **Library listing shows all files in ldap_libs:**
   ```
   ldap_libs/libssl.so.1.1 -> libssl64.so.1.1
   ldap_libs/libcrypto.so.1.1 -> libcrypto64.so.1.1
   ```

2. **ldd shows libraries found (not "NOT found"):**
   ```
   ./ldap_libs/libldap.so
   ./ldap_libs/libssl.so.1.1
   ./ldap_libs/libcrypto.so.1.1
   ```

3. **Test runs without loader errors**

---

## Related Documentation

- [OPENSSL_DEPENDENCY_RESOLUTION.md](./OPENSSL_DEPENDENCY_RESOLUTION.md) - Complete solution documentation
- [LDAP_AUTHENTICATION_DOCUMENTATION.md](./LDAP_AUTHENTICATION_DOCUMENTATION.md) - LDAP setup guide
- [LIBPATH_FIX_DOCUMENTATION.md](./LIBPATH_FIX_DOCUMENTATION.md) - Original LIBPATH fixes

---

**Document Version:** 1.0  
**Last Updated:** January 2025  
**Status:** Complete inspection record with corrected solution
