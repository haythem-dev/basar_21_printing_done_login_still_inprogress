
# Basar Library - Comprehensive Introduction

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Core Libraries](#core-libraries)
4. [Technical Details](#technical-details)
5. [Dependencies](#dependencies)
6. [Initialization System](#initialization-system)
7. [Development Guidelines](#development-guidelines)

---

## Overview

**Basar** is a comprehensive C++ library ecosystem developed by Pharmos for pharmaceutical software applications. It provides a layered architecture supporting database operations, GUI development, authentication, property management, and common utilities.

### Key Characteristics

- **Language**: C++17
- **Platform**: AIX (primary), Windows (secondary)
- **Build System**: GNU Make (AIX), Visual Studio (Windows)
- **Architecture**: Layered dependencies with strict initialization order
- **License**: Proprietary (Pharmos internal)

### Version Information

Current version defined in `libbasar_pkgversion.h`:
```cpp
#define LIBBASAR_VERSION_MAJOR 1
#define LIBBASAR_VERSION_MINOR 11
#define LIBBASAR_VERSION_PATCH 1
```

---

## Architecture

### Dependency Layers

Basar follows a **bottom-up layered architecture** where higher-level libraries depend on lower-level ones:

```
┌─────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                        │
│         libbasarappl, libbasarguitie (GUI binding)         │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    PRESENTATION LAYER                       │
│    libbasarguiqt (Qt5 GUI), libbasarqtwidget (Widgets)     │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    BUSINESS LOGIC LAYER                     │
│    libbasarlogin (Auth), libbasarproperty (Properties)     │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    DATA ACCESS LAYER                        │
│    libbasardbaspect (ORM), libbasardbsql (SQL Layer)       │
└─────────────────────────┬───────────────────────────────────┘
                          │
┌─────────────────────────┴───────────────────────────────────┐
│                    FOUNDATION LAYER                         │
│    libbasarcmnutil (Common Utils), libbasardebug (Debug)   │
└─────────────────────────────────────────────────────────────┘
```

### Static Initialization Pattern

Each library implements a static initialization pattern to avoid the **Static Initialization Order Fiasco**:

```cpp
namespace basar {
namespace <library> {

bool init() {
    static bool done = false;
    if (done) return true;
    
    // Initialize dependencies first
    basar::cmnutil::init();
    
    // Library-specific initialization
    
    done = true;
    return true;
}

// Automatic initialization at load time
static bool initlib = init();

}}
```

---

## Core Libraries

### 1. libbasarcmnutil (Foundation)

**Purpose**: Common utilities used throughout the Basar ecosystem

**Key Components**:

#### Date/Time Operations
- `Date` - Date handling (year, month, day)
- `DateTime` - Combined date and time
- `TimeSpan` - Time duration calculations
- **Features**: Arithmetic, formatting, parsing, validation

#### Decimal Arithmetic
- `Decimal` - Fixed-point decimal for financial calculations
- **Precision**: Avoids floating-point errors
- **Use Case**: Pharmaceutical pricing, calculations requiring exact decimal math

#### String Handling
- `I18nString` - Internationalized string support
- `VarString` - Variable-length string wrapper
- **Features**: Encoding conversion, multi-language support

#### Email & Printing
- `Email` - Email composition and sending
- `Attachment` - File and memory-based attachments
- **Printing**: PostScript/PDF generation via Ghostscript

#### Remote Task Execution
- `RemoteTask` - Execute commands on remote machines via SSH
- `RemoteTaskSubmitter` - Task submission manager
- **Protocol**: SSH2 (via libssh2)

#### Logging
- Wrapper around **log4cplus**
- Requires `MARATHON_LOG_CONFIGDIR` environment variable
- Centralized logging configuration

**File Count**: ~100+ source files

---

### 2. libbasardbsql (Database SQL Layer)

**Purpose**: Low-level database connectivity abstraction

**Supported Databases**:
- **ODBC** - Generic database access
- **Informix** - IBM Informix (primary)
- **CICS** - IBM CICS Transaction Server

**Key Classes**:

#### Connection Management
```cpp
ConnectionRef conn = Manager::instance().createConnection();
DatabaseInfo dbInfo;
dbInfo.setInfx("server_name", "database_name");
conn->connect(dbInfo);
```

#### Statement Execution
```cpp
StatementRef stmt = conn->createStatement();
stmt->execute("SELECT * FROM table");
ResultSetRef rs = stmt->getResultSet();
while (rs->next()) {
    VarString value = rs->getString(1);
}
```

#### Prepared Statements
```cpp
PreparedStatementRef pstmt = conn->prepareStatement("SELECT * FROM table WHERE id = ?");
pstmt->setInt(1, 42);
pstmt->execute();
```

**Dependencies**:
- libbasarcmnutil (foundation)
- Informix ESQL/C SDK (libBml.a)
- ODBC drivers

---

### 3. libbasardbaspect (Database ORM Layer)

**Purpose**: Object-Relational Mapping with caching policies

**Key Concepts**:

#### Accessors
Object-oriented database access with automatic query building:
```cpp
MyAccessor acc;
acc.execute();  // Fetches data
PropertyTableRef table = acc.getPropertyTable();
```

#### Caching Policies
- **NO_CACHING** - No data caching
- **SINGLE_ROW** - Cache one row at a time
- **ON_DEMAND** - Fetch rows as needed
- **FULL** - Cache entire result set

#### SQL String Builders
Macro-based query construction:
```cpp
BEGIN_QUERY_BUILDER_DECLARATION(MyQuery)
    ADD_PROPERTY(id, Int32)
    ADD_PROPERTY(name, VarString)
END_QUERY_BUILDER_DECLARATION()
```

#### Prepared Statement Support (v1.11+)
```cpp
BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(MyPreparedQuery)
    // Faster execution, SQL injection protection
END_QUERY_BUILDER_DECLARATION()
```

**Performance**: Prepared statements are ~2x faster than normal queries

---

### 4. libbasarproperty (Property Management)

**Purpose**: Generic property table abstraction

**Features**:
- Type-safe property storage
- X/Y iteration (columns/rows)
- Property state management (READONLY, MODIFIED, etc.)
- Property descriptors with validation

**Usage Example**:
```cpp
PropertyTableRef table;
table->addPropertyDescription("name", PropertyType::STRING);
table->addRow();
table->setProperty(0, "name", "John");
```

---

### 5. libbasarlogin (Authentication)

**Purpose**: User authentication and authorization

**Components**:

#### User Management
- User validation (UNIX/Windows/Active Directory)
- Password management
- Login history tracking

#### Application Rights
- Application-level permissions
- User role management
- Area-based access control

#### Configuration
```cpp
Configuration config;
config.setLoginType(LoginType::AD_LOGIN);  // Active Directory
config.setDatabaseInfo(dbInfo);
```

**Security**: Encrypted password storage, AD integration

---

### 6. libbasarguiqt (Qt5 GUI Framework)

**Purpose**: Qt5-based graphical user interface components

**Components**:

#### Main Window Management
- `Manager` - Singleton managing application lifecycle
- `MainBackground` - Application background with product type branding
- `StatusLine` - Status bar management

#### Dialogs
- `About` - About dialog with version info
- `PrintPreview` - PostScript/PDF print preview

**Qt Version**: Qt5 (Qt5Core, Qt5Gui, Qt5Widgets)

**Dependencies**:
- All lower layers
- Qt5 framework

---

### 7. libbasarguitie (GUI-to-Data Binding)

**Purpose**: Connect GUI widgets to data models

**Key Concepts**:

#### View Connectors
Automatic synchronization between GUI controls and PropertyTables:
```cpp
ViewConnPtr<QLineEdit> connector(lineEdit);
connector.connectProperty(propertyTable, "name");
// lineEdit automatically updates when property changes
```

#### Transformers
Data conversion between GUI and model:
- `DateTransformer` - Date ↔ QDate
- `DecimalTransformer` - Decimal ↔ QString
- `CheckBoxTransformer` - bool ↔ Qt::CheckState

#### Control Behaviors
- `LineEditBehavior` - Text input handling
- `ComboBoxBehavior` - Dropdown list binding
- `TableWidgetBehavior` - Table data binding

---

### 8. libbasarqtwidget (Custom Qt Widgets)

**Purpose**: Specialized Qt widgets for Basar applications

**Widgets**:
- `BasarDateEdit` - Date input with validation
- `BasarTimeEdit` - Time input
- `BasarDateTimeEdit` - Combined date/time
- `MultiComboBox` - Multi-selection dropdown
- `TableWidget` - Enhanced table with property binding

**Features**:
- Automatic format validation
- Integration with libbasarguitie
- Consistent look and feel

---

### 9. libbasarappl (Application Framework)

**Purpose**: Application-level infrastructure

**Components**:
- System event management
- Logger pool initialization
- Application lifecycle hooks

**Usage**: Initialize once at application startup

---

### 10. libbasarprinting (Printing Utilities)

**Purpose**: PostScript/PDF document handling

**Features**:
- PostScript interpretation via Ghostscript
- PDF conversion
- Print preview generation
- Raw printing to network printers

**Dependencies**: Ghostscript library

---

### 11. libbasardebug (Debug Utilities)

**Purpose**: Development and debugging support

**Features**:
- Memory leak detection (Windows)
- Assertion macros
- Debug output helpers
- Build server detection

**Platform**: Windows (memory checks), AIX (basic assertions)

---

### 12. libopensslwrap (SSL/TLS Wrapper)

**Purpose**: OpenSSL wrapper for secure communications

**Features**:
- SSL/TLS encryption
- Certificate handling
- Used by remote task execution

**Dependencies**: OpenSSL library

---

## Technical Details

### Build System

#### AIX (Primary Platform)
```makefile
CXX = g++
CXXFLAGS = -g -std=c++17 -maix64 -pthread
LDFLAGS = -maix64 -pthread -Wl,-brtl

# Library paths
LIBS = -lbasarcmnutil -lbasardebug -llog4cplus
```

#### Windows (Secondary Platform)
- Visual Studio 2019+
- Project files: `*.vcxproj`
- Solution: `basar.sln`

### Compiler Requirements
- **AIX**: GCC 12+ or IBM XL C++
- **Windows**: MSVC 2019+
- **Standard**: C++17

### Linking Order
```
-lbasarappl
-lbasarguitie
-lbasarguiqt
-lbasarqtwidget
-lbasarlogin
-lbasarproperty
-lbasardbaspect
-lbasardbsql
-lbasarcmnutil
-lbasardebug
```

---

## Dependencies

### External Libraries (3rd Party)

#### 1. Boost
**Version**: 1.70+  
**Components**:
- `boost::shared_ptr` - Smart pointers
- `boost::filesystem` - File system operations
- `boost::system` - System error handling

**Usage**: Memory management, file operations

---

#### 2. log4cplus
**Purpose**: Logging framework  
**Configuration**: XML-based log configuration  
**Environment**:
```bash
export MARATHON_LOG_CONFIGDIR=/path/to/config
export MARATHON_LOG_LOGSDIR=/path/to/logs
```

**Config Example**:
```xml
log4cplus.rootLogger=INFO, APPEND_STDOUT
log4cplus.appender.APPEND_STDOUT=log4cplus::ConsoleAppender
```

---

#### 3. Qt5
**Version**: Qt 5.12+  
**Modules**:
- Qt5Core - Core non-GUI functionality
- Qt5Gui - GUI base classes
- Qt5Widgets - Widget classes
- Qt5Network - Network operations (optional)

**Build**: qmake-based (converted to makefile for Basar)

---

#### 4. Informix ESQL/C
**Purpose**: Informix database connectivity  
**Library**: `libBml.a` (IBM CICS library)  
**Location**: `/software/ae/marathon/lib/tcaccess/8.0.20.1/lib`

**Usage**: Primary database for pharmaceutical applications

---

#### 5. libssh2
**Purpose**: SSH2 protocol implementation  
**Usage**: Remote task execution  
**Features**:
- SSH authentication (username/password)
- Remote command execution
- Secure file transfer (SFTP)

---

#### 6. OpenSSL
**Version**: 1.1.1+  
**Purpose**: SSL/TLS encryption  
**Usage**:
- Secure communications
- Certificate validation
- Cryptographic operations

---

#### 7. Ghostscript
**Purpose**: PostScript/PDF processing  
**Usage**:
- PDF generation from PostScript
- Print preview rendering
- Document conversion

---

#### 8. zlib
**Purpose**: Compression library  
**Usage**: Data compression in network operations

---

### Dependency Graph

```
Basar Libraries
    ├── Boost (shared_ptr, filesystem, system)
    ├── log4cplus (logging)
    ├── Qt5 (GUI framework)
    │   ├── Qt5Core
    │   ├── Qt5Gui
    │   └── Qt5Widgets
    ├── Informix ESQL/C (database)
    │   └── libBml.a (CICS)
    ├── libssh2 (SSH protocol)
    │   └── OpenSSL (encryption)
    ├── Ghostscript (PDF/PostScript)
    └── zlib (compression)
```

---

## Initialization System

### Initialization Order

Critical for avoiding static initialization order fiasco:

```cpp
// 1. libbasardebug - First (no dependencies)
basar::debug::init();

// 2. libbasarcmnutil - Foundation
basar::cmnutil::init();  // Calls debug::init()

// 3. libbasarproperty - Property management
basar::property::init();  // Calls debug, cmnutil

// 4. libbasardbsql - Database SQL layer
basar::db::sql::init();  // Calls cmnutil

// 5. libbasardbaspect - Database ORM
basar::db::aspect::init();  // Calls cmnutil, property, sql

// 6. libbasarlogin - Authentication
basar::login::init();  // Calls cmnutil, property, sql, aspect

// 7. libbasarappl - Application framework
basar::appl::init();  // Calls cmnutil, property, aspect

// 8. libbasarguitie - GUI binding
basar::gui::tie::init();  // Calls lower layers

// 9. libbasarguiqt - Qt GUI
basar::gui::qt::init();  // Calls ALL lower layers
```

### Environment Setup

Required environment variables:

```bash
# Logging configuration
export MARATHON_LOG_CONFIGDIR=/path/to/config
export MARATHON_LOG_LOGSDIR=/path/to/logs

# Library paths (AIX)
export LIBPATH=/path/to/basar/lib:/path/to/ext/lib:$LIBPATH

# Library paths (Linux)
export LD_LIBRARY_PATH=/path/to/basar/lib:/path/to/ext/lib:$LD_LIBRARY_PATH
```

---

## Development Guidelines

### Adding New Features

1. **Choose appropriate layer**: Place code in the correct library
2. **Follow initialization pattern**: Add to init.cpp
3. **Update dependencies**: Ensure proper library linking order
4. **Document**: Update APM supplements

### Regression Testing

Basar uses Qt5-style regression tests:

```bash
cd pharmos.base.basar_cpr_up/dev/src/basar/regression
./do_regression.sh
```

**Test Structure**:
- `main.cpp` - Test program
- `makefile` - Build configuration
- `go.sh` - Build and run script
- `patterns.txt` - Expected output validation

### Documentation

- **APM Supplements**: Feature-specific documentation in `docs/`
- **Code Comments**: Doxygen-style comments
- **Release Notes**: `changelog.txt`, `release.md`

---

## Use Cases

### Pharmaceutical Applications

Basar is designed for:
- **Prescription Management**: Date/time tracking, decimal pricing
- **Inventory Systems**: Database ORM, property tables
- **User Authentication**: AD integration, role-based access
- **Regulatory Compliance**: Audit logging, secure communications
- **Multi-language Support**: I18n strings, locale management

### Typical Application Flow

```cpp
// 1. Initialize libraries (automatic via static init)

// 2. Setup logging
// (configured via MARATHON_LOG_CONFIGDIR)

// 3. Connect to database
DatabaseInfo dbInfo;
dbInfo.setInfx("prod_server", "pharma_db");
ConnectionRef conn = Manager::instance().createConnection();
conn->connect(dbInfo);

// 4. Authenticate user
CheckLoginUC loginUC;
LoginData loginData("username", "password");
loginUC.execute(loginData);

// 5. Load data via ORM
PrescriptionAccessor acc;
acc.execute();
PropertyTableRef prescriptions = acc.getPropertyTable();

// 6. Display GUI (Qt5)
QApplication app(argc, argv);
MainWindow mainWin;
// Bind data to GUI via libbasarguitie
mainWin.show();

// 7. Process business logic
// Date arithmetic, decimal calculations, etc.

// 8. Cleanup (automatic via destructors)
```

---

## Version History

**Current**: v1.11.01.01

**Major Changes (v1.11)**:
- Added prepared statement support in libbasardbaspect
- Performance improvements (25%+ faster data transfer)
- MainBackground product type branding
- Email attachment support

**See**: `changelog.txt` for complete history

---

## Support & Resources

**Documentation**:
- APM Supplements in `dev/docs/`
- Regression test examples in `dev/src/basar/regression/`
- Release notes in `release.md`

**Build Environment**:
- AIX: `/software/ae/marathon/`
- Development: `$DEVLIB_PATH/marathon/`

**Team**: Pharmos Development (internal use only)

---

## Conclusion

Basar provides a comprehensive, layered C++ framework for pharmaceutical software development. Its modular design, strict initialization order, and extensive feature set make it suitable for enterprise-level applications requiring database integration, GUI development, secure communications, and regulatory compliance.

The library ecosystem continues to evolve, with ongoing improvements in performance, security, and functionality to meet the demanding requirements of pharmaceutical software systems.

---

*Document Version*: 1.0  
*Last Updated*: January 2025  
*Basar Version*: 1.11.01.01
