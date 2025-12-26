# libbasardbsql

## Role in Basar Package

**libbasardbsql** is the low-level SQL database library for Basar. It provides direct database connectivity and SQL operations:

- Database connection management
- SQL statement preparation and execution
- Result set handling and iteration
- Database metadata inspection
- Error and exception handling for database operations

This library interfaces directly with Informix ESQL/C and provides the foundation for higher-level database abstractions in `libbasardbaspect`.

## Dependencies

- Informix Client SDK (ESQL/C)
- CICS libraries (AIX) - For BML connectivity
- `libbasarcmnutil` - Common utilities

## Regression Tests

| Test | Description |
|------|-------------|
| `connection_basic` | Tests basic database connection lifecycle |
| `database_info_test` | Validates database metadata retrieval |
| `exception_handling` | Tests SQL error handling and exceptions |
| `resultset_test` | Validates result set navigation and data extraction |
| `statement_test` | Tests SQL statement preparation and execution |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## Environment Requirements

- `INFORMIXDIR` must be set to Informix installation path
- `INFORMIXSERVER` must point to a valid database server
