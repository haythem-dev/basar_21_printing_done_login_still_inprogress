# libbasardbaspect

## Role in Basar Package

**libbasardbaspect** is the database aspect library for Basar. It provides a high-level abstraction layer over database operations:

- Database accessor patterns for simplified data access
- Connection pooling and management
- Caching policies for query optimization
- SQL query building utilities
- Transaction management aspects

This library depends on `libbasardbsql` for low-level database connectivity and adds aspect-oriented programming patterns for cleaner database code.

## Dependencies

- `libbasardbsql` - Low-level SQL operations
- `libbasarproperty` - Property management
- `libbasarcmnutil` - Common utilities
- CICS libraries (AIX) - Required for Informix BML

## Regression Tests

| Test | Description |
|------|-------------|
| `accessor_test` | Tests database accessor pattern implementation |
| `caching_policy_test` | Validates query caching strategies |
| `connection_test` | Tests connection pool management |
| `manager_test` | Validates database manager operations |
| `sql_builder_test` | Tests dynamic SQL query construction |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## AIX Notes

Tests require CICS library path in LIBPATH for `libBml.a` (Informix BML library). The `go.sh` scripts automatically configure this.
