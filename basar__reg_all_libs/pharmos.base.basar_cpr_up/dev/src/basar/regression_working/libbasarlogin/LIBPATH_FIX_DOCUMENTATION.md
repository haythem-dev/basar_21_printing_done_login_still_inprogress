# libbasarlogin AIX Library Path Fix Documentation

## Overview

This document describes the fixes applied to resolve runtime library loading issues in `libbasarlogin` regression tests on AIX. The tests were failing with `Cannot load module` errors due to missing library paths.

---

## Issues Resolved

### Issue 1: libBml.a Not Found

**Error Message:**
```
exec(): 0509-036 Cannot load program ./authentication_flow_test
        0509-022 Cannot load module ../../../library/lib/debug/libbasardbsql.so.
        0509-150   Dependent module libBml.a could not be loaded.
        0509-022 Cannot load module libBml.a.
        0509-026 System error: A file or directory in the path name does not exist.
```

**Root Cause:**
- `libbasarlogin.so` depends on `libbasardbsql.so`
- `libbasardbsql.so` requires `libBml.a` (IBM Informix BML library)
- `libBml.a` is located in the **CICS library path**, NOT in `$INFORMIXDIR`

**Solution:**
Added `CICS_LIB` to LIBPATH:
```bash
CICS_LIB="${DEVLIB_PATH:-/software/ae}/marathon/lib/tcaccess/8.0.20.1/lib"
export LIBPATH="${CICS_LIB}:$LIBPATH"
```

**Source of Fix:**
Pattern from working `libbasardbsql/connection_basic/go.sh` (line 38)

---

### Issue 2: libssl.so.1.1 Not Found

**Error Message:**
```
exec(): 0509-036 Cannot load program ./authentication_flow_test
        0509-022 Cannot load module ./ldap_libs/libldap.so.
        0509-150   Dependent module ../../../../../ext/openssl/lib/libssl.a(libssl.so.1.1) could not be loaded.
        0509-152   Member libssl.so.1.1 is not found in archive
```

**Root Cause:**
- `libldap.so` (extracted from `/opt/freeware/lib/libldap.a`) was linked against system OpenSSL
- System OpenSSL uses: `libssl.so.1.1` (32-bit style member name)
- Project OpenSSL uses: `libssl64.so.1.1` (64-bit style member name)
- The loader looks for `libssl.so.1.1` but only finds `libssl64.so.1.1`

**Solution:**
Added `/opt/freeware/lib` to LIBPATH so the system OpenSSL can be found:
```bash
export LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:../../../../../ext/openssl/lib:..."
```

**Source of Fix:**
Pattern from `ldap_config_test/go.sh` which already had `/opt/freeware/lib`

---

## Complete LIBPATH Order

The corrected LIBPATH includes all dependencies in the proper order:

```bash
LIBPATH="${CICS_LIB}:./ldap_libs:/opt/freeware/lib:../../../../../ext/openssl/lib:../../../library/lib/debug:../../../../../ext/log4cplus/lib/debug:../../../../../ext/boost/lib:../../../../../ext/libssh2/lib/debug:../../../../../ext/ghostscript/lib:../../../../../ext/zlib/lib/debug:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib:$LIBPATH"
```

| Order | Path | Purpose |
|-------|------|---------|
| 1 | `${CICS_LIB}` | libBml.a, libBmlCh.a, libBmlUtils.a (Informix) |
| 2 | `./ldap_libs` | Extracted libldap.so, liblber.so |
| 3 | `/opt/freeware/lib` | System OpenSSL (libssl.so.1.1) for LDAP |
| 4 | `ext/openssl/lib` | Project OpenSSL (libssl64.so.1.1) |
| 5 | `library/lib/debug` | Basar libraries |
| 6 | `ext/log4cplus/lib/debug` | Logging framework |
| 7 | `ext/boost/lib` | C++ utilities |
| 8 | `ext/libssh2/lib/debug` | SSH/SFTP support |
| 9 | `ext/ghostscript/lib` | PDF/PostScript |
| 10 | `ext/zlib/lib/debug` | Compression |
| 11 | `$INFORMIXDIR/lib/esql` | Informix ESQL |
| 12 | `$INFORMIXDIR/lib` | Informix core |

---

## Files Modified

All 6 `libbasarlogin` test scripts were updated:

| Script | Changes Applied |
|--------|-----------------|
| `authentication_flow_test/go.sh` | Added CICS_LIB + /opt/freeware/lib |
| `ldap_config_test/go.sh` | Added CICS_LIB (already had /opt/freeware/lib) |
| `configuration_test/go.sh` | Added CICS_LIB + /opt/freeware/lib |
| `session_management_test/go.sh` | Added CICS_LIB + /opt/freeware/lib |
| `user_credentials_test/go.sh` | Added CICS_LIB + /opt/freeware/lib |
| `permission_check_test/go.sh` | Added CICS_LIB + /opt/freeware/lib |

---

## Dependency Chain

```
authentication_flow_test
    │
    ├─→ libbasarlogin.so
    │       │
    │       ├─→ libldap.so (from ./ldap_libs)
    │       │       │
    │       │       └─→ libssl.so.1.1 (from /opt/freeware/lib) ← FIX #2
    │       │
    │       └─→ libbasardbsql.so
    │               │
    │               └─→ libBml.a (from CICS_LIB) ← FIX #1
    │
    ├─→ libbasarcmnutil.so
    ├─→ liblog4cplus.so
    └─→ libboost_*.so
```

---

## Environment Variables

### Required
```bash
export INFORMIXDIR=/informix/informix11.70FC5
```

### Optional (defaults shown)
```bash
export DEVLIB_PATH=/software/ae  # Base path for CICS libraries
```

---

## Verification

After applying fixes, `ldd` output should show all libraries resolved:

```
$ ldd authentication_flow_test | grep -E "libBml|libssl"
/software/ae/marathon/lib/tcaccess/8.0.20.1/lib/libBml.a      ← FOUND
/software/ae/marathon/lib/tcaccess/8.0.20.1/lib/libBmlCh.a    ← FOUND
/software/ae/marathon/lib/tcaccess/8.0.20.1/lib/libBmlUtils.a ← FOUND
/opt/freeware/lib/libssl.a(libssl.so.1.1)                     ← FOUND
```

---

## Running Tests

```bash
# Set required environment
export INFORMIXDIR=/informix/informix11.70FC5

# Navigate to test directory
cd libbasarlogin/authentication_flow_test

# Run the test
./go.sh
```

---

## Related Documentation

- `CHAT_SESSION_QA_SUMMARY.md` - Original Q&A session with libBml.a resolution
- `LDAP_AUTHENTICATION_DOCUMENTATION.md` - LDAP library setup and dependencies
- `libbasardbsql/connection_basic/go.sh` - Reference implementation for CICS_LIB

---

## Change History

| Date | Change |
|------|--------|
| 2025-12-04 | Added CICS_LIB to resolve libBml.a issue |
| 2025-12-04 | Added /opt/freeware/lib to resolve libssl.so.1.1 issue |
| 2025-12-04 | Created this documentation |

---

*Document created: December 4, 2025*
