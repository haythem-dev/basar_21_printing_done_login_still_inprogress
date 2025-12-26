# Basar Framework Regression Test Documentation

## Overview

This document provides comprehensive documentation for the Basar C++ framework regression test package. All tests are designed to be compatible with **AIX 7.2** and use the appropriate compiler flags for that platform.

The Basar framework is a comprehensive C++ application development framework designed for enterprise pharmaceutical applications. It provides a layered architecture covering everything from low-level utilities to high-level GUI components, with strong support for database operations, security, and printing capabilities.

## Table of Contents

1. [Directory Structure](#directory-structure)
2. [Test Framework Components](#test-framework-components)
3. [AIX 7.2 Compatibility](#aix-72-compatibility)
4. [Library Test Documentation](#library-test-documentation)
5. [Running Tests](#running-tests)
6. [Adding New Tests](#adding-new-tests)

---

## Directory Structure

```
regression/
├── do_regression.sh           # Main regression test runner script
├── new_test_aix.sh           # Script for creating new AIX-compatible tests
├── new_test.sh               # Generic new test creation script
├── check_patterns.sh         # Pattern validation script
├── check_dependencies.sh     # Dependency checker
├── run_standalone_only.sh    # Run standalone tests only
├── regression.summary        # Test execution summary
├── ADDING_3RD_PARTY_LIBRARY_TESTS.md  # Guide for adding 3rd party tests
├── REGRESSION_TEST_DOCUMENTATION.md   # This documentation
│
├── libbasarcmnutil/          # Common Utilities Library Tests
│   ├── date_operations/      # Date manipulation tests (10 sub-tests)
│   ├── decimal_arithmetic/   # Decimal number operations (12 sub-tests)
│   ├── exception_test/       # Exception handling tests (10 sub-tests)
│   ├── hello_basar/          # Basic library verification (5 sub-tests)
│   └── string_operations/    # String manipulation tests (10 sub-tests)
│
├── libbasarappl/             # Application Framework Library Tests
│   ├── event_executor_test/  # Event execution tests (10 sub-tests)
│   ├── event_macros_test/    # Event macro tests (10 sub-tests)
│   ├── functions_test/       # Utility function tests (10 sub-tests)
│   ├── init_test/            # Initialization tests (10 sub-tests)
│   ├── logger_pool_test/     # Logger pool tests (10 sub-tests)
│   └── system_event_test/    # System event tests (10 sub-tests)
│
├── libbasardbaspect/         # Database Aspect Library Tests
│   ├── accessor_test/        # Accessor pattern tests (10 sub-tests)
│   ├── caching_policy_test/  # Caching policy tests (10 sub-tests)
│   ├── connection_test/      # Connection management tests (10 sub-tests)
│   ├── manager_test/         # Manager pattern tests (10 sub-tests)
│   └── sql_builder_test/     # SQL builder tests (10 sub-tests)
│
├── libbasardbsql/            # Database SQL Library Tests
│   ├── connection_basic/     # Basic connection tests (5 sub-tests)
│   ├── database_info_test/   # Database info retrieval (5 sub-tests)
│   ├── exception_handling/   # SQL exception tests (5 sub-tests)
│   ├── resultset_test/       # Result set operations (10 sub-tests)
│   └── statement_test/       # SQL statement tests (10 sub-tests)
│
├── libbasardebug/            # Debug Library Tests
│   ├── build_server_test/    # Build server tests (10 sub-tests)
│   ├── debug_output_test/    # Debug output tests (10 sub-tests)
│   ├── leak_check_test/      # Memory leak detection (10 sub-tests)
│   ├── memory_ignore_test/   # Memory ignore tests (10 sub-tests)
│   └── print_error_test/     # Error printing tests (10 sub-tests)
│
├── libbasarguiqt/            # Qt GUI Library Tests
│   ├── definition_struct_test/  # DefinitionStruct tests (5 sub-tests)
│   ├── dll_struct_test/      # DllStruct tests (5 sub-tests)
│   ├── init_struct_test/     # InitStruct tests (5 sub-tests)
│   ├── manager_init_test/    # Manager initialization (5 sub-tests)
│   └── version_info_test/    # Version info retrieval (5 sub-tests)
│
├── libbasarguitie/           # GUI TIE Library Tests
│   ├── event_policy_test/    # Event policy tests (10 sub-tests)
│   ├── exception_handler_test/ # Exception handler tests (10 sub-tests)
│   ├── matcher_policy_test/  # Matcher policy tests (10 sub-tests)
│   ├── transformer_test/     # Transformer tests (10 sub-tests)
│   └── widget_types_test/    # Widget type tests (10 sub-tests)
│
├── libbasarlogin/            # Login Library Tests
│   ├── authentication_flow_test/ # Auth flow tests (10 sub-tests)
│   ├── configuration_test/   # Configuration tests (5 sub-tests)
│   ├── permission_check_test/   # Permission tests (10 sub-tests)
│   ├── session_management_test/ # Session tests (10 sub-tests)
│   └── user_credentials_test/   # Credentials tests (10 sub-tests)
│
├── libbasarprinting/         # Printing Library Tests
│   ├── file_extension_test/  # File extension tests (10 sub-tests)
│   ├── interpreter_test/     # Interpreter tests (10 sub-tests)
│   ├── pdf_conversion_test/  # PDF conversion tests (10 sub-tests)
│   ├── print_utils_test/     # Print utility tests (10 sub-tests)
│   └── raw_print_test/       # Raw print tests (10 sub-tests)
│
├── libbasarproperty/         # Property Library Tests
│   ├── column_operations_test/  # Column tests (10 sub-tests)
│   ├── property_description_test/ # Description tests (10 sub-tests)
│   ├── property_state_test/  # State tests (10 sub-tests)
│   ├── property_table_test/  # Table tests (10 sub-tests)
│   └── property_type_test/   # Type tests (10 sub-tests)
│
├── libbasarqtwidget/         # Qt Widget Library Tests
│   ├── combo_box_test/       # ComboBox tests (10 sub-tests)
│   ├── date_edit_test/       # DateEdit tests (10 sub-tests)
│   ├── table_widget_test/    # TableWidget tests (10 sub-tests)
│   ├── time_edit_test/       # TimeEdit tests (10 sub-tests)
│   └── widget_types_test/    # Widget types tests (10 sub-tests)
│
├── libopensslwrap/           # OpenSSL Wrapper Library Tests
│   ├── ssl_context_test/     # SSL context tests (10 sub-tests)
│   ├── ssl_error_test/       # SSL error tests (10 sub-tests)
│   ├── ssl_init_test/        # SSL initialization (10 sub-tests)
│   ├── ssl_method_test/      # SSL method tests (10 sub-tests)
│   └── ssl_version_test/     # SSL version tests (10 sub-tests)
│
└── standalone/               # Standalone Tests (No library dependencies)
    ├── cpp_basics/           # C++ basics verification (5 sub-tests)
    ├── filesystem_test/      # Filesystem operations (10 sub-tests)
    ├── infrastructure_test/  # Build infrastructure (5 sub-tests)
    ├── memory_test/          # Memory operations (10 sub-tests)
    └── threading_test/       # Threading tests (10 sub-tests)
```

---

## Test Framework Components

### Test Directory Structure

Each test directory contains the following files:

| File | Description |
|------|-------------|
| `main.cpp` | Test source code with 5+ sub-tests |
| `go.sh` | Build and run script |
| `makefile` | AIX-compatible makefile |
| `patterns.txt` | Expected output patterns for validation |

### go.sh Script

The `go.sh` script performs the following operations:
1. Compiles the test using the makefile
2. Executes the test binary
3. Captures and reports the exit code

### makefile Structure

Each makefile uses AIX 7.2 compatible compiler flags:

```makefile
CXX = xlC_r
CXXFLAGS = -maix64 -pthread -I$(BASAR_ROOT)/include
LDFLAGS = -Wl,-brtl -L$(BASAR_ROOT)/lib -l<library_name>
```

### patterns.txt Format

The `patterns.txt` file contains expected output patterns that the `check_patterns.sh` script validates against actual test output.

---

## AIX 7.2 Compatibility

### Required Compiler Flags

| Flag | Purpose |
|------|---------|
| `-maix64` | 64-bit AIX compilation |
| `-pthread` | POSIX thread support |
| `-Wl,-brtl` | Runtime linking |

### Platform Requirements

- **Compiler**: IBM XL C/C++ (xlC_r)
- **OS**: AIX 7.2
- **Architecture**: PowerPC 64-bit

---

## Library Test Documentation

### libbasarcmnutil (Common Utilities)

#### Library Introduction

**libbasarcmnutil** is the foundational utility library of the Basar framework. It provides essential low-level functionality that all other Basar libraries depend upon. This library serves as the building block for the entire framework, offering robust implementations of commonly needed data types and operations.

#### Role in the Framework

- **Foundation Layer**: Provides core utilities used by all other Basar components
- **Data Type Support**: Implements specialized data types like `Date` and `Decimal` with precision handling
- **Exception Framework**: Establishes the base exception hierarchy for consistent error handling across the framework
- **String Utilities**: Offers advanced string manipulation functions beyond standard C++ capabilities
- **Cross-Platform Abstraction**: Encapsulates platform-specific functionality for portability

#### Key Features

- High-precision decimal arithmetic for financial calculations
- Comprehensive date/time manipulation with timezone support
- Unified exception handling with detailed error information
- Thread-safe string operations
- Encoding conversion utilities (UTF-8, Latin-1, etc.)

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `date_operations` | `Date::getCurrent()`, `Date::toString()`, `Date::fromString()`, `Date::addDays()`, `Date::getDayOfWeek()` |
| `decimal_arithmetic` | `Decimal` constructor, arithmetic operators, comparison operators, precision handling, conversion methods |
| `exception_test` | `Exception` class, `what()` method, exception inheritance, throw/catch semantics |
| `hello_basar` | `getVersion()`, `Date` basic usage, library loading verification |
| `string_operations` | String utilities, encoding conversion, trim operations, case conversion |

**Namespace:** `basar::cmnutil`

---

### libbasarappl (Application Framework)

#### Library Introduction

**libbasarappl** is the application lifecycle management library that provides the infrastructure for building robust Basar applications. It handles application initialization, event processing, logging, and graceful shutdown procedures.

#### Role in the Framework

- **Application Bootstrap**: Manages the complete application startup and initialization sequence
- **Event System**: Provides a powerful event-driven architecture for loose coupling between components
- **Logging Infrastructure**: Implements a flexible, pooled logging system with multiple output targets
- **Configuration Management**: Handles application configuration loading and management
- **Lifecycle Control**: Manages application states from startup through shutdown

#### Key Features

- Macro-based event definition for compile-time type safety
- Asynchronous event execution with queuing support
- Hierarchical logger pools with configurable log levels
- System signal handling for graceful shutdown
- Plugin architecture support

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `event_executor_test` | Event execution patterns, async event handling, event queuing |
| `event_macros_test` | Event definition macros, event registration, event dispatching |
| `functions_test` | Utility functions, application helpers, system utilities |
| `init_test` | Application initialization, startup sequence, configuration loading |
| `logger_pool_test` | Logger creation, log pool management, log level configuration |
| `system_event_test` | System event handling, signal processing, shutdown events |

**Namespace:** `basar::appl`

---

### libbasardbaspect (Database Aspect)

#### Library Introduction

**libbasardbaspect** implements aspect-oriented programming patterns for database operations. It provides a high-level abstraction layer over raw database access, implementing patterns like accessor, caching, and connection pooling.

#### Role in the Framework

- **Data Access Layer**: Provides structured patterns for database interactions
- **Caching Strategy**: Implements configurable caching policies to optimize performance
- **Connection Management**: Handles database connection pooling and lifecycle
- **Query Abstraction**: Offers SQL query builders for database-agnostic operations
- **Transaction Support**: Manages database transactions with proper isolation

#### Key Features

- Accessor pattern for type-safe CRUD operations
- Configurable caching policies (LRU, TTL, write-through)
- Connection pool with automatic health checking
- Fluent SQL builder API
- Aspect-oriented transaction management

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `accessor_test` | Data accessor patterns, CRUD operations, field access |
| `caching_policy_test` | Cache configuration, cache invalidation, cache strategies |
| `connection_test` | Connection pooling, connection lifecycle, connection parameters |
| `manager_test` | Database manager singleton, transaction management |
| `sql_builder_test` | SQL query building, parameterized queries, query validation |

**Namespace:** `basar::dbaspect`

---

### libbasardbsql (Database SQL)

#### Library Introduction

**libbasardbsql** is the low-level database connectivity library that provides direct SQL database access through ODBC. It handles the raw database communication, result set processing, and SQL exception management.

#### Role in the Framework

- **Database Connectivity**: Establishes and manages ODBC database connections
- **SQL Execution**: Executes SQL statements with parameter binding support
- **Result Processing**: Handles result set iteration and data type conversion
- **Error Handling**: Provides detailed SQL exception information
- **Metadata Access**: Retrieves database and driver metadata

#### Key Features

- ODBC-based database connectivity for broad database support
- Prepared statement support with parameter binding
- Efficient result set iteration with column-based access
- Comprehensive SQL exception hierarchy
- Database metadata and version information retrieval

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `connection_basic` | `DatabaseInfo` creation, ODBC configuration, connection state management |
| `database_info_test` | `DatabaseInfo::getVersion()`, database metadata, info struct population |
| `exception_handling` | `SQLException`, error codes, exception messages |
| `resultset_test` | `ResultSet` iteration, column access, null handling, data conversion |
| `statement_test` | `Statement` creation, parameter binding, execution, batch operations |

**Namespace:** `basar::dbsql`

---

### libbasardebug (Debug Support)

#### Library Introduction

**libbasardebug** provides comprehensive debugging and diagnostic capabilities for Basar applications. It includes memory leak detection, debug output management, and integration with build servers for automated testing.

#### Role in the Framework

- **Memory Diagnostics**: Detects memory leaks and tracks allocations
- **Debug Output**: Manages debug logging with configurable verbosity
- **Build Integration**: Supports CI/CD pipelines with structured test output
- **Error Reporting**: Provides detailed error messages with stack traces
- **Development Tools**: Offers utilities to aid in development and debugging

#### Key Features

- Runtime memory leak detection with allocation tracking
- Configurable memory ignore lists for third-party allocations
- Structured debug output with log level filtering
- Stack trace generation for error diagnostics
- Build server integration for automated testing

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `build_server_test` | Build server integration, debug info generation |
| `debug_output_test` | Debug output streams, log formatting, output redirection |
| `leak_check_test` | Memory leak detection, allocation tracking, leak reporting |
| `memory_ignore_test` | Memory ignore lists, false positive handling |
| `print_error_test` | Error printing utilities, stack traces, error formatting |

**Namespace:** `basar::debug`

---

### libbasarguiqt (Qt GUI Core)

#### Library Introduction

**libbasarguiqt** is the core Qt-based GUI library that provides the fundamental structures and management classes for building graphical user interfaces in Basar applications. It defines the essential data structures for application definition and initialization.

#### Role in the Framework

- **GUI Foundation**: Provides base structures for Qt-based GUI applications
- **Application Definition**: Defines program metadata (version, copyright, description)
- **Initialization Management**: Manages GUI component initialization sequences
- **DLL Management**: Tracks loaded dynamic libraries and their versions
- **Manager Pattern**: Implements singleton managers for GUI resource coordination

#### Key Features

- DefinitionStruct for application metadata management
- InitStruct for database and connection configuration
- DllStruct for dynamic library version tracking
- Singleton Manager pattern for centralized GUI control
- Version information APIs for runtime version queries

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `definition_struct_test` | `DefinitionStruct` fields: copyright, programDescription, version |
| `dll_struct_test` | `DllStruct` fields: name, pkgVersion |
| `init_struct_test` | `InitStruct` fields: database, host, dokulink |
| `manager_init_test` | Manager singleton pattern, initialization sequence |
| `version_info_test` | `getVersion()`, `getFullVersion()`, version string format |

**Namespace:** `basar::guiqt`, `basar` (struct types)

---

### libbasarguitie (GUI TIE Framework)

#### Library Introduction

**libbasarguitie** implements the TIE (Transform, Interpret, Execute) framework for GUI operations. It provides a policy-based approach to GUI event handling, widget matching, and data transformation.

#### Role in the Framework

- **Event Policy System**: Configurable policies for GUI event processing
- **Widget Matching**: Pattern-based widget identification and selection
- **Data Transformation**: Converts data between GUI and business layer formats
- **Exception Handling**: Specialized GUI exception handling with user feedback
- **Widget Type System**: Extensible widget type registration and identification

#### Key Features

- Policy-based event filtering and processing
- Regex-based widget matching for flexible UI automation
- Bidirectional data transformers for model-view binding
- GUI-aware exception handling with dialog support
- Dynamic widget type registration system

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `event_policy_test` | Event policy configuration, event filtering, policy application |
| `exception_handler_test` | GUI exception handling, error dialogs, recovery mechanisms |
| `matcher_policy_test` | Widget matching policies, pattern matching, selector policies |
| `transformer_test` | Data transformers, value conversion, format transformation |
| `widget_types_test` | Widget type registration, type identification, type mapping |

**Namespace:** `basar::guitie`

---

### libbasarlogin (Authentication)

#### Library Introduction

**libbasarlogin** provides comprehensive authentication and authorization functionality for Basar applications. It handles user login flows, session management, permission checking, and credential storage.

#### Role in the Framework

- **Authentication**: Manages user login and credential validation
- **Authorization**: Implements role-based access control (RBAC)
- **Session Management**: Handles user sessions with timeout and invalidation
- **Credential Security**: Secure storage and validation of user credentials
- **Configuration**: Flexible authentication configuration options

#### Key Features

- Complete authentication flow with multi-factor support capability
- Role and permission-based access control
- Session management with configurable timeouts
- Secure password hashing and credential storage
- Pluggable authentication backends

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `authentication_flow_test` | Login flow, authentication sequence, credential validation |
| `configuration_test` | Login configuration, namespace availability, type definitions |
| `permission_check_test` | Permission verification, role checking, access control |
| `session_management_test` | Session creation, session timeout, session invalidation |
| `user_credentials_test` | Credential storage, password hashing, credential validation |

**Namespace:** `basar::login`

---

### libbasarprinting (Printing)

#### Library Introduction

**libbasarprinting** handles all printing functionality in Basar applications, including PDF generation, print job management, and direct printer communication. It supports various output formats and printing protocols.

#### Role in the Framework

- **Print Management**: Coordinates print jobs across different printers
- **PDF Generation**: Creates PDF documents from application data
- **Format Handling**: Processes various document formats for printing
- **Raw Printing**: Supports direct printer communication for specialized hardware
- **Print Utilities**: Provides helper functions for common printing tasks

#### Key Features

- PDF generation with customizable templates
- Support for multiple printer types and protocols
- File extension-based format detection
- Print interpreter for custom print languages
- Raw print output for label printers and POS devices

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `file_extension_test` | File extension handling, MIME type detection |
| `interpreter_test` | Print interpreter, format processing |
| `pdf_conversion_test` | PDF generation, document conversion |
| `print_utils_test` | Print utilities, printer selection, job management |
| `raw_print_test` | Raw print output, direct printer communication |

**Namespace:** `basar::printing`

---

### libbasarproperty (Properties)

#### Library Introduction

**libbasarproperty** implements a comprehensive property system for managing application and component properties. It provides structured property definitions, state management, and property tables for organized data handling.

#### Role in the Framework

- **Property Definition**: Defines typed properties with metadata and validation
- **State Management**: Tracks property state changes and transitions
- **Property Tables**: Organizes properties into structured tables
- **Column Operations**: Manages property columns with type information
- **Serialization**: Supports property serialization and deserialization

#### Key Features

- Strongly-typed property system with validation
- Property state tracking (clean, dirty, new, deleted)
- Hierarchical property tables with column definitions
- Property description metadata for UI generation
- Type conversion and validation utilities

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `column_operations_test` | Column definitions, column metadata, column operations |
| `property_description_test` | Property descriptions, property metadata |
| `property_state_test` | Property state management, state transitions |
| `property_table_test` | Property tables, table operations, table queries |
| `property_type_test` | Property types, type conversion, type validation |

**Namespace:** `basar::property`

---

### libbasarqtwidget (Qt Widgets)

#### Library Introduction

**libbasarqtwidget** provides enhanced Qt widget implementations tailored for Basar applications. It extends standard Qt widgets with additional functionality specific to pharmaceutical and enterprise application requirements.

#### Role in the Framework

- **Enhanced Widgets**: Provides specialized versions of common Qt widgets
- **Data Binding**: Supports data binding between widgets and the property system
- **Input Validation**: Implements domain-specific input validation
- **Widget Types**: Registers custom widget types for the framework
- **User Experience**: Improves user interaction with specialized behaviors

#### Key Features

- Enhanced ComboBox with search and filtering capabilities
- DateEdit and TimeEdit with locale-aware formatting
- TableWidget with sorting, filtering, and selection features
- Custom widget type registration system
- Consistent look and feel across all widgets

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `combo_box_test` | ComboBox creation, item management, selection handling |
| `date_edit_test` | DateEdit creation, date formatting, date validation |
| `table_widget_test` | TableWidget creation, cell operations, selection modes |
| `time_edit_test` | TimeEdit creation, time formatting, time validation |
| `widget_types_test` | Widget type registration, type enumeration |

**Namespace:** `basar::qtwidget`

---

### libopensslwrap (OpenSSL Wrapper)

#### Library Introduction

**libopensslwrap** provides a C++ wrapper around the OpenSSL library, offering secure communication capabilities for Basar applications. It simplifies SSL/TLS usage while maintaining full access to OpenSSL's security features.

#### Role in the Framework

- **Secure Communication**: Enables encrypted network communication
- **SSL/TLS Support**: Provides modern TLS protocol support
- **Certificate Handling**: Manages SSL certificates and chains
- **Error Management**: Wraps OpenSSL errors in C++ exceptions
- **Thread Safety**: Ensures thread-safe SSL operations

#### Key Features

- Easy-to-use SSL context creation and configuration
- Support for multiple TLS versions and cipher suites
- Comprehensive SSL error handling with detailed messages
- Thread-safe initialization and cleanup
- Version compatibility checking

#### Tested APIs

| Test | APIs Covered |
|------|--------------|
| `ssl_context_test` | SSL context creation, context configuration, context options |
| `ssl_error_test` | SSL error handling, error codes, error messages |
| `ssl_init_test` | SSL library initialization, cleanup, thread safety |
| `ssl_method_test` | SSL method selection, TLS versions, protocol negotiation |
| `ssl_version_test` | OpenSSL version retrieval, version compatibility |

**Namespace:** `basar::opensslwrap`

---

### standalone (Infrastructure Tests)

#### Introduction

The **standalone** test suite verifies the build infrastructure and basic C++ functionality without any Basar library dependencies. These tests ensure the development environment is correctly configured before running library-dependent tests.

#### Role in Testing

- **Build Verification**: Confirms the build system functions correctly
- **Compiler Validation**: Verifies C++ compiler and standard library availability
- **Platform Testing**: Tests platform-specific functionality (filesystem, threading)
- **Baseline Establishment**: Establishes a working baseline for library tests
- **Environment Validation**: Ensures the AIX 7.2 environment is properly configured

#### Key Features

- Pure C++ tests with no external dependencies
- Filesystem operation verification
- Memory management testing
- POSIX threading validation
- Build infrastructure verification

#### Tested Functionality

| Test | Purpose |
|------|---------|
| `cpp_basics` | C++ language features: strings, vectors, smart pointers, exceptions |
| `filesystem_test` | Filesystem operations: file creation, directory operations |
| `infrastructure_test` | Build system validation, standard library availability |
| `memory_test` | Memory allocation, deallocation, memory management |
| `threading_test` | POSIX threads, mutex operations, thread synchronization |

---

## Running Tests

### Run All Tests

```bash
./do_regression.sh
```

### Run Specific Library Tests

```bash
cd libbasarcmnutil
for test in */; do
    cd "$test"
    ./go.sh
    cd ..
done
```

### Run Standalone Tests Only

```bash
./run_standalone_only.sh
```

### Check Pattern Validation

```bash
./check_patterns.sh <test_directory>
```

---

## Adding New Tests

### Using new_test_aix.sh

```bash
./new_test_aix.sh <library_name> <test_name>
```

This creates a new test directory with:
- AIX-compatible `main.cpp` template
- Configured `makefile` with proper flags
- `go.sh` build/run script
- Empty `patterns.txt` for expected output

### Test Requirements

1. **Minimum 5 sub-tests per test file**
2. **Consistent output format:**
   ```
   =========================================
     Basar Regression Test: <test_name>
   =========================================
   
   Test 1: <Test Description>
     <Test output>
     <Verification message>
   
   ...
   
   =========================================
     Test completed successfully
   =========================================
   ```
3. **Proper exception handling**
4. **Pattern validation support**

---

## Test Statistics Summary

| Library | Test Directories | Total Sub-Tests |
|---------|------------------|-----------------|
| libbasarcmnutil | 5 | 47 |
| libbasarappl | 6 | 60 |
| libbasardbaspect | 5 | 50 |
| libbasardbsql | 5 | 35 |
| libbasardebug | 5 | 50 |
| libbasarguiqt | 5 | 25 |
| libbasarguitie | 5 | 50 |
| libbasarlogin | 5 | 45 |
| libbasarprinting | 5 | 50 |
| libbasarproperty | 5 | 50 |
| libbasarqtwidget | 5 | 50 |
| libopensslwrap | 5 | 50 |
| standalone | 5 | 40 |
| **TOTAL** | **66** | **602** |

---

## Framework Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        Application Layer                                 │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │ libbasarappl│  │libbasarlogin│  │libbasarprint│  │libbasarprop │    │
│  │ (App Frame) │  │   (Auth)    │  │ (Printing)  │  │ (Properties)│    │
│  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
┌─────────────────────────────────────────────────────────────────────────┐
│                          GUI Layer                                       │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐                      │
│  │libbasarguiqt│  │libbasarguitie│ │libbasarqtwid│                      │
│  │ (Qt Core)   │  │ (TIE Frame) │  │ (Widgets)   │                      │
│  └─────────────┘  └─────────────┘  └─────────────┘                      │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
┌─────────────────────────────────────────────────────────────────────────┐
│                        Database Layer                                    │
│  ┌─────────────┐  ┌─────────────┐                                       │
│  │libbasardbsql│  │libbasardb   │                                       │
│  │ (SQL/ODBC)  │  │aspect (ORM) │                                       │
│  └─────────────┘  └─────────────┘                                       │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
┌─────────────────────────────────────────────────────────────────────────┐
│                       Foundation Layer                                   │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐                      │
│  │libbasarcmn  │  │libbasardebug│  │libopensslwrp│                      │
│  │util (Core)  │  │  (Debug)    │  │   (SSL)     │                      │
│  └─────────────┘  └─────────────┘  └─────────────┘                      │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## Version History

| Date | Version | Description |
|------|---------|-------------|
| 2024-12 | 1.1 | Added library introductions and explanations |
| 2024-12 | 1.0 | Initial comprehensive documentation |

---

## Contact

For questions about this regression test suite, please contact the Basar development team.
