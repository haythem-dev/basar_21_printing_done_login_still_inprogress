
# LDAP Authentication in Basar Framework - Complete Documentation

## Table of Contents
1. [Overview](#overview)
2. [Real Source Package Analysis](#real-source-package-analysis)
3. [LDAP Implementation Architecture](#ldap-implementation-architecture)
4. [Platform-Specific Details](#platform-specific-details)
5. [AIX LDAP Library Dependencies](#aix-ldap-library-dependencies)
6. [Regression Testing Strategy](#regression-testing-strategy)
7. [Complete File Reference](#complete-file-reference)

---

## Overview

The Basar framework implements **Active Directory (AD) authentication** using the LDAP (Lightweight Directory Access Protocol) protocol. This enables enterprise-wide user authentication against Microsoft Active Directory servers.

### Key Features
- TLS-encrypted LDAP communication
- Active Directory user validation
- Account status checking (enabled/disabled)
- Service account-based directory searches
- Cross-platform support (Windows/AIX/Unix)

---

## Real Source Package Analysis

### Package Location
```
basar__reg_all_libs/pharmos.base.basar_cpr_up/dev/src/basar/libbasarlogin/
```

### Core Files

#### 1. **Configuration Management**

**File:** `configuration.cpp` / `configuration.h`

**Purpose:** Manages LDAP server configuration and authentication settings

**Key Configuration Parameters:**
```cpp
// Default LDAP settings
LDAP Server IP:     10.130.247.202
LDAP Port:          389 (standard LDAP port)
Service Account DN: CN=svc - UADM Lookup,OU=Services,OU=Global,DC=phoenix,DC=loc
Service Password:   Ago2Tod6
```

**Configuration Keys (from basar.config.xml):**
```
basar.login.logintype           -> "AD" or non-AD
basar.login.adlogin.ldapserver  -> LDAP server IP address
basar.login.adlogin.tcpport     -> LDAP server port
basar.login.adlogin.commonname  -> Service account common name
basar.login.adlogin.password    -> Service account password
basar.login.adlogin.dn_prefix   -> Distinguished name prefix
basar.login.adlogin.dn_postfix  -> Distinguished name postfix
```

**Key Methods:**
- `getLdapServer()` - Returns LDAP server IP
- `getTcpPort()` - Returns LDAP port
- `getDistingushedName()` - Returns service account DN
- `getPassword()` - Returns service account password
- `isADLogin()` - Checks if AD authentication is enabled

---

#### 2. **Authentication Flow Controller**

**File:** `checkadloginuc.cpp` / `checkadloginuc.h`

**Purpose:** Orchestrates the AD authentication process

**Authentication Steps:**
1. **User Lookup:** Search for user in database by username
2. **AD Connection:** Establish LDAP connection to AD server
3. **Account Search:** Find user's LDAP entry in Active Directory
4. **Status Check:** Verify account is enabled (userAccountControl check)
5. **Authentication:** Bind to AD using user's credentials
6. **Authorization:** Check user permissions for selected application area

**Key Methods:**
```cpp
bool run()                          // Main authentication flow
void setLoginMode(bool sso)         // Enable/disable single sign-on
bool hasUserRight()                 // Check application permissions
VarString getUserLogonName()        // Get Windows logon name (SSO)
```

**Authentication Modes:**
- **Standard Login:** Username + password validation
- **Single Sign-On (SSO):** Uses current Windows credentials

---

#### 3. **Unix/AIX LDAP Implementation**

**File:** `adusercheckerunix.cpp` / `adusercheckerunix.h`

**Purpose:** Platform-specific LDAP operations for Unix/AIX systems

**LDAP Operations Flow:**

```
1. Initialize LDAP Connection
   ↓
2. Configure TLS Settings
   ↓
3. Start TLS Encryption
   ↓
4. Bind with Service Account
   ↓
5. Search for User in AD
   ↓
6. Check Account Status
   ↓
7. Authenticate User Credentials
   ↓
8. Close Connection
```

**Key Methods:**
```cpp
bool createLdapConnection()                    // Establishes LDAP connection
bool doLogin(username, password)               // Authenticates user
bool isAccountActive(accountName)              // Checks if account enabled
LDAPMessage* findLdapUserEntry(accountName)    // Searches AD for user
bool getDistinguishedName(username, dn)        // Gets user's DN from AD
void shutdownLdapConnection()                  // Closes LDAP connection
```

**LDAP Library Calls (OpenLDAP):**
```cpp
ldap_initialize()        // Create LDAP connection handle
ldap_set_option()        // Configure LDAP options (TLS, version)
ldap_start_tls_s()       // Start TLS encryption
ldap_simple_bind_s()     // Bind with service account
ldap_search_ext_s()      // Search directory for user
ldap_get_values()        // Retrieve user attributes
ldap_sasl_bind_s()       // Authenticate user credentials
ldap_unbind_ext()        // Close connection
```

---

#### 4. **Windows LDAP Implementation**

**File:** `adusercheckerwindows.cpp` / `adusercheckerwindows.h`

**Purpose:** Platform-specific LDAP operations for Windows systems

**Windows-Specific Features:**
- Uses `winldap.h` (Windows LDAP API)
- Integrated with Windows security (`security.h`)
- Single Sign-On support via `GetUserNameEx()`

---

## LDAP Implementation Architecture

### Class Hierarchy

```
IADUserChecker (Interface)
    ↓
    ├── ADUserCheckerUnix    (Unix/AIX implementation)
    └── ADUserCheckerWindows (Windows implementation)
```

### Authentication Sequence Diagram

```
User Application
    ↓
CheckADLogin::run()
    ↓
    ├─→ Database lookup (find user by username)
    ↓
ADUserChecker::createLdapConnection()
    ↓
    ├─→ ldap_initialize() - Create connection
    ├─→ ldap_set_option() - Configure TLS
    └─→ ldap_start_tls_s() - Enable encryption
    ↓
ADUserChecker::findLdapUserEntry()
    ↓
    ├─→ ldap_simple_bind_s() - Bind as service account
    └─→ ldap_search_ext_s() - Search for user
         Filter: (&(objectClass=user)(sAMAccountName=<username>))
         Base DN: DC=phoenix,DC=loc
    ↓
ADUserChecker::isAccountActive()
    ↓
    └─→ ldap_get_values() - Get userAccountControl attribute
         Check bit 0x0200 (ACCOUNTDISABLE flag)
    ↓
ADUserChecker::doLogin()
    ↓
    └─→ ldap_sasl_bind_s() - Authenticate with user credentials
    ↓
CheckADLogin::hasUserRight()
    ↓
    └─→ Database query for user permissions
    ↓
Return authentication result
```

---

## Platform-Specific Details

### AIX Implementation

**Compiler:** IBM XL C/C++ (xlC_r)

**LDAP Library:** OpenLDAP 2.5.16 (from `/opt/freeware`)

**Required Headers:**
```cpp
#define LDAP_DEPRECATED 1  // Enable deprecated LDAP functions
#include <ldap.h>          // OpenLDAP C API
```

**Key LDAP Structures:**
```cpp
LDAP*        m_pldapConnection;  // LDAP connection handle
LDAPMessage* userDetails;        // Search result message
BerValue     cred;               // Credential structure for binding
```

**TLS Configuration:**
```cpp
// Do not require certificate verification
ldap_set_option(NULL, LDAP_OPT_X_TLS_REQUIRE_CERT, LDAP_OPT_X_TLS_NEVER);

// Do not check certificate revocation
ldap_set_option(NULL, LDAP_OPT_X_TLS_CRLCHECK, LDAP_OPT_X_TLS_CRL_NONE);

// Use LDAP v3 protocol
ldap_set_option(m_pldapConnection, LDAP_OPT_PROTOCOL_VERSION, LDAP_VERSION3);
```

**Active Directory Search:**
```cpp
Search Base:    "DC=phoenix,DC=loc"
Search Scope:   LDAP_SCOPE_SUBTREE (recursive search)
Search Filter:  "(&(objectClass=user)(sAMAccountName=%s))"
Attributes:     NULL (return all attributes)
```

**Account Status Detection:**
```cpp
// userAccountControl attribute values
// Bit 0x0200 = ACCOUNTDISABLE
int uacValue = atoi(ldap_get_values()[0]);
bool enabled = ((0x0200 & uacValue) == 0x0200);
```

---

## AIX LDAP Library Dependencies

### The Problem

On AIX, OpenLDAP libraries are packaged as **archive files** (`.a`) containing shared objects, not as standalone `.so` files:

```bash
# Library location
/opt/freeware/lib/libldap.a

# Contains (inside archive)
libldap-2.5.so.0
libldap-2.4.so.2
```

**Issue:** Runtime linker cannot find `libldap.so` because it doesn't exist as a standalone file.

### Solution: Extract LDAP Libraries Locally

**Step 1:** Create local library directory
```bash
mkdir -p ./ldap_libs
cd ./ldap_libs
```

**Step 2:** Extract shared objects from archives
```bash
# Extract LDAP library
ar -X64 -x /opt/freeware/lib/libldap.a

# Extract LBER library (LDAP dependency)
ar -X64 -x /opt/freeware/lib/liblber.a
```

**Step 3:** Create symbolic links
```bash
ln -sf libldap-2.5.so.0 libldap.so
ln -sf liblber-2.5.so.0 liblber.so
```

**Step 4:** Update LIBPATH
```bash
export LIBPATH="./ldap_libs:$LIBPATH"
```

### Library Dependencies Chain

```
authentication_flow_test
    ↓
libbasarlogin.so
    ↓
    ├─→ libldap.so (OpenLDAP client)
    │       ↓
    │       └─→ liblber.so (LDAP BER encoding)
    │               ↓
    │               └─→ libssl.so, libcrypto.so (TLS/SSL)
    ↓
    ├─→ libbasardbsql.so (Database SQL layer)
    │       ↓
    │       └─→ libBml.a (Informix BML library) ← REQUIRES INFORMIXDIR!
    │       └─→ Other Informix ESQL libraries
    ↓
    ├─→ libbasarcmnutil.so (Basar utilities)
    ├─→ liblog4cplus.so (Logging)
    └─→ libboost_*.so (Boost libraries)
```

---

## Informix Database Library Dependencies

### The Problem

`libbasarlogin.so` has a dependency on `libbasardbsql.so` which requires Informix database libraries:

```
Cannot load module libBml.a.
0509-026 System error: A file or directory in the path name does not exist.
```

### Solution: Set INFORMIXDIR

Before running any `libbasarlogin` tests, you **MUST** set the `INFORMIXDIR` environment variable:

```bash
# Set Informix directory (adjust path for your system)
export INFORMIXDIR=/opt/informix

# Run the test
./go.sh
```

### How This Differs from libbasarcmnutil Tests

The working `libbasarcmnutil____working` tests **do not** have this dependency because:

| Library | Depends on libbasardbsql.so | Requires INFORMIXDIR |
|---------|----------------------------|---------------------|
| libbasarcmnutil | No | No |
| libbasarlogin | Yes (indirect) | **Yes** |

### Updated go.sh Scripts

All `libbasarlogin` test scripts now:

1. **Check for INFORMIXDIR** before running
2. **Fail gracefully** with a clear error message if not set
3. **Include Informix library paths** when INFORMIXDIR is available:
   - `$INFORMIXDIR/lib/esql` (ESQL libraries including libBml.a)
   - `$INFORMIXDIR/lib` (Core Informix libraries)

### Example Usage

```bash
# On AIX with Informix installed
export INFORMIXDIR=/opt/informix

# Navigate to test directory
cd libbasarlogin/authentication_flow_test

# Run the test
./go.sh
```

If INFORMIXDIR is not set, you'll see:
```
=========================================
  ERROR: INFORMIXDIR not set!
=========================================

libbasarlogin depends on libbasardbsql.so which requires:
  - libBml.a (Informix BML library)
  - Other Informix ESQL libraries

Please set INFORMIXDIR before running this test:
  export INFORMIXDIR=/path/to/informix

SKIPPED: Informix libraries not available
```

---

## Regression Testing Strategy

### Test Categories

#### 1. **Configuration Tests** (`ldap_config_test`)
**Purpose:** Validate LDAP configuration without network connectivity

**What It Tests:**
- Configuration object creation
- Default LDAP settings
- Configuration parameter access
- Login type detection
- Namespace availability

**Why No Network Required:**
- Tests only read configuration
- No actual LDAP connection
- Validates code compilation and linking

---

#### 2. **Authentication Flow Tests** (`authentication_flow_test`)
**Purpose:** Test authentication logic flow (mock/stub)

**Approach:**
- Use mock LDAP responses
- Test error handling paths
- Validate state transitions
- Check permission logic

---

### Test Implementation Example

**File:** `ldap_config_test/main.cpp`

```cpp
// Test 1: Verify configuration object creation
Configurationptr config(new Configuration());
// ✓ Ensures libbasarlogin.so loads correctly

// Test 2: Access LDAP server setting
const VarString& ldapServer = config->getLdapServer();
// ✓ Validates configuration reading mechanism

// Test 3: Check login type
bool isAD = config->isADLogin();
// ✓ Tests enum and type system
```

---

## Complete File Reference

### Source Files

| File | Purpose | Key Classes/Functions |
|------|---------|----------------------|
| `configuration.cpp` | LDAP config management | `Configuration`, `readConfiguration()` |
| `checkadloginuc.cpp` | Auth flow orchestration | `CheckADLogin::run()`, `hasUserRight()` |
| `adusercheckerunix.cpp` | Unix LDAP operations | `doLogin()`, `isAccountActive()` |
| `adusercheckerwindows.cpp` | Windows LDAP operations | Windows-specific LDAP calls |
| `iaduserchecker.h` | LDAP interface | `IADUserChecker` abstract class |
| `loginappl.cpp` | Application context | `LoginAppl` state management |

### Header Files

| File | Purpose |
|------|---------|
| `configuration.h` | Config class declaration |
| `checkadloginuc.h` | CheckADLogin class |
| `adusercheckerunix.h` | Unix checker declaration |
| `iaduserchecker.h` | LDAP checker interface |
| `logintype.h` | Login type enums |
| `libbasarlogin_definitions.h` | Common definitions |

### Configuration Files

| File | Purpose |
|------|---------|
| `basar.config.xml` | Application configuration |
| `*.cfg` | Log4cplus logging config |

### Regression Test Files

| File | Purpose |
|------|---------|
| `ldap_config_test/main.cpp` | Configuration validation test |
| `ldap_config_test/makefile` | AIX build configuration |
| `ldap_config_test/go.sh` | Test runner script |
| `ldap_config_test/patterns.txt` | Expected output patterns |

---

## Summary

### Real Source Package
- **Location:** `libbasarlogin/`
- **Purpose:** Enterprise AD authentication
- **Platform:** Cross-platform (Windows/AIX/Unix)
- **Protocol:** LDAP over TLS
- **Dependencies:** OpenLDAP, OpenSSL, Boost

### Regression Tests
- **Strategy:** Configuration validation without network
- **Coverage:** Namespace, types, configuration access
- **AIX Focus:** LDAP library dependency resolution
- **No Network:** Tests don't require AD server connectivity

### Key Takeaway
The regression tests validate that the LDAP authentication **code compiles and links correctly** on AIX, and that all configuration mechanisms work, without requiring actual Active Directory connectivity.

