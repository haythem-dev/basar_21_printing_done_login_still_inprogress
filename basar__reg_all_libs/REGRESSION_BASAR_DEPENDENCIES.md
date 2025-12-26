
# Regression Tests, Basar Library, and Dependencies

## Overview

This document explains the architecture and relationships between the **Basar regression test suite**, the **Basar library ecosystem**, and their **external dependencies**.

---

## 1. The Basar Library Ecosystem

### 1.1 Library Hierarchy

The Basar library follows a **layered architecture** where higher-level libraries depend on lower-level ones:

```
┌─────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                        │
│                  (libbasarappl, etc.)                       │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    PRESENTATION LAYER                       │
│         libbasarguiqt, libbasarguitie, libbasarqtwidget    │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    BUSINESS LOGIC LAYER                     │
│          libbasarlogin, libbasarproperty, libbasarprinting │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    DATA ACCESS LAYER                        │
│              libbasardbaspect, libbasardbsql               │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    FOUNDATION LAYER                         │
│        libbasarcmnutil, libbasardebug, libopensslwrap      │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Core Libraries

| Library | Purpose | Key Components |
|---------|---------|----------------|
| **libbasarcmnutil** | Common utilities (Foundation) | Date, DateTime, TimeSpan, Decimal, I18nString, Logging, Email, Printing |
| **libbasardebug** | Debug utilities | Assertions, checks, debug helpers |
| **libbasardbsql** | Database SQL layer | Connection, Statement, ResultSet, PreparedStatement |
| **libbasardbaspect** | Database ORM/aspect layer | Accessors, caching policies, transaction management |
| **libbasarproperty** | Property management | Property tables, descriptors, state management |
| **libbasarlogin** | Authentication | User management, login validation, credentials |
| **libbasarguiqt** | Qt GUI framework | Main background, print preview, status line, about dialogs |
| **libbasarguitie** | GUI-to-data binding | View connectors, transformers, control behaviors |
| **libbasarqtwidget** | Custom Qt widgets | BasarDateEdit, BasarTimeEdit, MultiComboBox, TableWidget |
| **libbasarappl** | Application framework | System events, logger pool, initialization |
| **libbasarprinting** | Printing utilities | PostScript interpretation, print utilities |
| **libopensslwrap** | OpenSSL wrapper | SSL/TLS utilities |

---

## 2. External Dependencies

### 2.1 Third-Party Libraries

The Basar ecosystem depends on several external libraries:

```
pharmos.base.basar_cpr_up/dev/
│
├── ext/                           # External dependencies (symlinks)
│   ├── boost/                     # Boost C++ Libraries
│   │   ├── include/               # Smart pointers, filesystem, system
│   │   └── lib/
│   │
│   ├── log4cplus/                 # Logging framework
│   │   ├── include/               # log4cplus headers
│   │   └── lib/
│   │
│   ├── qt5/                       # Qt5 framework
│   │   ├── include/               # Qt headers (QtCore, QtGui, QtWidgets, etc.)
│   │   └── lib/
│   │
│   ├── openssl/                   # SSL/TLS library
│   │   ├── include/
│   │   └── lib/
│   │
│   ├── libssh2/                   # SSH2 protocol library
│   │   ├── include/
│   │   └── lib/
│   │
│   ├── zlib/                      # Compression library
│   │   ├── include/
│   │   └── lib/
│   │
│   └── ghostscript/               # PostScript/PDF processing
│       ├── include/
│       └── lib/
```

### 2.2 Dependency Matrix

| Basar Library | Boost | log4cplus | Qt5 | OpenSSL | libssh2 | zlib | Ghostscript |
|---------------|-------|-----------|-----|---------|---------|------|-------------|
| libbasarcmnutil | ✓ | ✓ | ✗ | ✓ | ✓ | ✓ | ✓ |
| libbasardebug | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| libbasardbsql | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| libbasardbaspect | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| libbasarproperty | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| libbasarlogin | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| libbasarguiqt | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| libbasarguitie | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| libbasarqtwidget | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |

---

## 3. Regression Test Architecture

### 3.1 Test Strategy

The regression tests follow a **bottom-up testing strategy**, validating the foundation before testing higher layers:

```
Testing Order (Priority):
1. Foundation Layer  → libbasarcmnutil (TESTED)
2. Data Layer        → libbasardbsql, libbasardbaspect (PLANNED)
3. Business Layer    → libbasarproperty, libbasarlogin (PLANNED)
4. Presentation Layer→ libbasarguiqt, libbasarguitie (PLANNED)
```

### 3.2 Current Test Coverage

#### **Phase 1: Foundation Tests** ✅ (Current)

Located in: `pharmos.base.basar_cpr_up/dev/src/basar/regression/`

```
regression/
├── libbasarcmnutil/           # Foundation library tests
│   ├── hello_basar/           # Library integration test
│   │   ├── main.cpp           # Tests: version info, basic initialization
│   │   ├── makefile           # Build configuration
│   │   ├── go.sh              # Test runner
│   │   ├── patterns.txt       # Expected output patterns
│   │   └── go.out             # Generated test output
│   │
│   ├── date_operations/       # Date/DateTime/TimeSpan test
│   │   ├── main.cpp           # Tests: Date arithmetic, formatting, parsing
│   │   └── ...
│   │
│   └── decimal_arithmetic/    # Decimal precision test
│       ├── main.cpp           # Tests: Decimal math, rounding, precision
│       └── ...
│
└── standalone/                # Infrastructure tests
    └── infrastructure_test/   # Validates test framework itself
        └── ...
```

### 3.3 Why These Tests Were Chosen

#### **1. infrastructure_test** (Standalone)
- **Purpose**: Validates the regression framework works
- **Dependencies**: NONE (pure C++)
- **Strategic Value**: Baseline - proves test execution works

#### **2. hello_basar** (libbasarcmnutil)
- **Purpose**: Library integration validation
- **Tests**: 
  - Library loading
  - Header accessibility
  - Version information
  - Logging initialization
- **Strategic Value**: Proves entire build chain works (headers → libraries → runtime)

#### **3. date_operations** (libbasarcmnutil)
- **Purpose**: Core data type validation
- **Tests**:
  - `Date` class operations
  - `DateTime` class operations
  - `TimeSpan` calculations
- **Why Critical**: 
  - Used throughout Pharmos (prescriptions, expiry dates, scheduling)
  - If dates break, most business logic breaks
  - Foundation for all time-based operations

#### **4. decimal_arithmetic** (libbasarcmnutil)
- **Purpose**: Financial precision validation
- **Tests**:
  - `Decimal` class arithmetic
  - Rounding operations
  - Precision preservation
- **Why Essential**:
  - Pharmaceutical pricing requires exact decimal math
  - Regulatory compliance (no floating-point errors)
  - Foundation for all monetary calculations

---

## 4. Dependency Flow in Tests

### 4.1 Compilation Dependencies

Each regression test has a specific dependency chain:

```
Test Program (main.cpp)
    ↓
    ├─→ Basar Headers (libbasarcmnutil.h)
    │       ↓
    │       ├─→ log4cplus Headers (logger.h, etc.)
    │       └─→ Boost Headers (scoped_ptr.hpp, etc.)
    │
    ├─→ Basar Libraries (.so files)
    │       ├─→ libbasarcmnutil.so
    │       └─→ libbasardebug.so
    │
    └─→ External Libraries (.so files)
            ├─→ liblog4cplus.so
            ├─→ libboost_system.so
            └─→ libboost_filesystem.so
```

### 4.2 Include Path Resolution

From test directory: `regression/libbasarcmnutil/hello_basar/`

```makefile
# Relative paths from test directory
BASAR_INC_DIR     = ../../../library/include           # Installed Basar headers
LOG4CPLUS_INC     = ../../../../../ext/log4cplus/include
BOOST_INC         = ../../../../../ext/boost/include

# Path resolution:
hello_basar/ → ../ → libbasarcmnutil/
             → ../ → regression/
             → ../ → basar/
             → library/include/  ← Basar headers
```

### 4.3 Library Path Resolution

```makefile
# Library directories
BASAR_LIB_DIR = ../../../library/lib/debug
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_LIB     = ../../../../../ext/boost/lib

# Runtime library path (in go.sh)
export LD_LIBRARY_PATH="$BASAR_LIB_DIR:$LOG4CPLUS_LIB:$LD_LIBRARY_PATH"
```

---

## 5. Test Execution Flow

### 5.1 Build and Run Process

```
┌────────────────────────────────────────────────────────────┐
│ 1. User executes: ./do_regression.sh                      │
└───────────────────────┬────────────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────────────┐
│ 2. For each test directory:                               │
│    - Find go.sh script                                     │
│    - Execute: bash ./go.sh                                 │
└───────────────────────┬────────────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────────────┐
│ 3. go.sh performs:                                         │
│    a) Set LD_LIBRARY_PATH for runtime                     │
│    b) Clean previous build (make clean)                   │
│    c) Compile test (make)                                 │
│    d) Run test executable                                 │
│    e) Capture all output to go.out                        │
└───────────────────────┬────────────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────────────┐
│ 4. make (makefile) performs:                              │
│    a) Resolve include paths                               │
│    b) Compile main.cpp with g++                           │
│    c) Link Basar libraries                                │
│    d) Link external libraries                             │
│    e) Create test executable                              │
└───────────────────────┬────────────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────────────┐
│ 5. Test executable runs:                                  │
│    a) Load shared libraries (.so)                         │
│    b) Initialize logging (log4cplus)                      │
│    c) Execute test logic                                  │
│    d) Print results to stdout                             │
│    e) Exit with code (0=success)                          │
└───────────────────────┬────────────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────────────┐
│ 6. Pattern validation:                                    │
│    - check_patterns.sh compares go.out vs patterns.txt   │
│    - Calculates success rate (% patterns found)          │
│    - PASS if 100%, FAIL otherwise                        │
└────────────────────────────────────────────────────────────┘
```

---

## 6. Relationship Diagram

### 6.1 Complete Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        PHARMOS ECOSYSTEM                            │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌───────────────────────────────────────────────────────────┐    │
│  │              REGRESSION TEST SUITE                        │    │
│  │                                                            │    │
│  │  ┌─────────────────┐  ┌─────────────────┐                │    │
│  │  │ infrastructure_ │  │ libbasarcmnutil │                │    │
│  │  │ test            │  │ tests           │                │    │
│  │  │                 │  │                 │                │    │
│  │  │ • Validates     │  │ • hello_basar   │                │    │
│  │  │   framework     │  │ • date_ops      │                │    │
│  │  │ • No deps       │  │ • decimal_arith │                │    │
│  │  └─────────────────┘  └────────┬────────┘                │    │
│  │                                 │                         │    │
│  │                                 │ Tests ↓                 │    │
│  └─────────────────────────────────┼─────────────────────────┘    │
│                                    │                              │
│  ┌─────────────────────────────────┼─────────────────────────┐    │
│  │         BASAR LIBRARY ECOSYSTEM │                         │    │
│  │                                 ▼                         │    │
│  │  ┌───────────────────────────────────────────────────┐   │    │
│  │  │ libbasarcmnutil (FOUNDATION)                      │   │    │
│  │  │                                                    │   │    │
│  │  │  Classes:                                         │   │    │
│  │  │  • Date, DateTime, TimeSpan                       │   │    │
│  │  │  • Decimal                                        │   │    │
│  │  │  • I18nString                                     │   │    │
│  │  │  • Exception hierarchy                            │   │    │
│  │  │  • Logging (log4cplus wrapper)                    │   │    │
│  │  │  • Email, Printing (PostScript)                   │   │    │
│  │  └───────────────┬───────────────────────────────────┘   │    │
│  │                  │                                       │    │
│  │                  │ Used by ↓                             │    │
│  │                  │                                       │    │
│  │  ┌───────────────┴───────────────────────────────────┐   │    │
│  │  │ Higher-Level Libraries                            │   │    │
│  │  │                                                    │   │    │
│  │  │  • libbasardbsql      (Database SQL)             │   │    │
│  │  │  • libbasardbaspect   (ORM/Accessors)            │   │    │
│  │  │  • libbasarlogin      (Authentication)           │   │    │
│  │  │  • libbasarproperty   (Property management)      │   │    │
│  │  │  • libbasarguiqt      (Qt GUI)                   │   │    │
│  │  │  • libbasarguitie     (GUI-to-data binding)      │   │    │
│  │  └───────────────────────────────────────────────────┘   │    │
│  └─────────────────────────────────────────────────────────┘    │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │         EXTERNAL DEPENDENCIES                            │   │
│  │                                                           │   │
│  │  • Boost          (smart pointers, filesystem)          │   │
│  │  • log4cplus      (logging framework)                   │   │
│  │  • Qt5            (GUI framework)                       │   │
│  │  • OpenSSL        (SSL/TLS)                             │   │
│  │  • libssh2        (SSH protocol)                        │   │
│  │  • zlib           (compression)                         │   │
│  │  • Ghostscript    (PostScript/PDF)                      │   │
│  └──────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 7. Future Test Expansion

### 7.1 Planned Test Coverage

Following the bottom-up strategy, future tests will cover:

#### **Phase 2: Data Layer Tests**
```
regression/
└── libbasardbsql/
    ├── connection_test/       # Database connection
    ├── statement_test/        # SQL statements
    └── resultset_test/        # Query results

└── libbasardbaspect/
    ├── accessor_test/         # ORM accessors
    ├── caching_test/          # Cache policies
    └── transaction_test/      # Transaction management
```

#### **Phase 3: Business Layer Tests**
```
regression/
└── libbasarlogin/
    ├── authentication_test/   # User login
    └── credentials_test/      # Credential management

└── libbasarproperty/
    ├── propertytable_test/    # Property tables
    └── descriptor_test/       # Property descriptors
```

#### **Phase 4: Presentation Layer Tests**
```
regression/
└── libbasarguiqt/
    ├── mainwindow_test/       # Main background
    └── printpreview_test/     # Print preview

└── libbasarguitie/
    ├── viewconnector_test/    # GUI-to-data binding
    └── transformer_test/      # Data transformers
```

---

## 8. Key Insights

### 8.1 Why Test libbasarcmnutil First?

1. **Foundation Dependency**: All other libraries depend on it
2. **Critical Data Types**: Date/Decimal used everywhere
3. **Risk Mitigation**: If foundation breaks, everything breaks
4. **Domain-Specific**: Pharmos needs precise dates (prescriptions) and decimals (pricing)

### 8.2 Dependency Pyramid

```
     ┌─────────────┐
     │ Applications│  ← Build on everything below
     └──────┬──────┘
            │
     ┌──────┴──────┐
     │ GUI Layers  │  ← Depend on business logic
     └──────┬──────┘
            │
     ┌──────┴──────┐
     │ Business    │  ← Depend on data access
     │ Logic       │
     └──────┬──────┘
            │
     ┌──────┴──────┐
     │ Data Access │  ← Depend on foundation
     └──────┬──────┘
            │
     ┌──────┴──────┐
     │ Foundation  │  ← TEST THIS FIRST ✓
     │(cmnutil)    │
     └─────────────┘
```

### 8.3 Test Philosophy

- ✅ **Test foundations first** - Bottom-up approach
- ✅ **Test most-used components** - Date/Decimal everywhere
- ✅ **Test domain-critical features** - Pharmos-specific needs
- ✅ **Validate build system** - Infrastructure before features

---

## 9. Summary

| Aspect | Details |
|--------|---------|
| **Test Count** | 4 (1 infrastructure + 3 libbasarcmnutil) |
| **Current Focus** | Foundation layer (libbasarcmnutil) |
| **Strategy** | Bottom-up testing (foundation → higher layers) |
| **External Deps** | Boost, log4cplus, Qt5, OpenSSL, libssh2, zlib, Ghostscript |
| **Test Success** | 100% (all 4 tests passing after build 10 fixes) |
| **Next Phase** | Data layer (libbasardbsql, libbasardbaspect) |

---

**Document Version**: 1.0  
**Last Updated**: Build 10 analysis  
**Status**: Foundation tests complete ✅  
**Next Goal**: Expand to data access layer tests
