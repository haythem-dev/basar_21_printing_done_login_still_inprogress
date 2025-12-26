# libbasarcmnutil

## Role in Basar Package

**libbasarcmnutil** is the common utilities library for Basar. It provides fundamental utility classes and functions used throughout the Basar framework:

- Date and time operations
- Decimal arithmetic with precision handling
- String manipulation utilities
- Exception handling infrastructure
- Basic data type conversions

This is a foundational library with minimal dependencies, used by nearly all other Basar libraries.

## Regression Tests

| Test | Description |
|------|-------------|
| `date_operations` | Tests date parsing, formatting, and arithmetic |
| `decimal_arithmetic` | Validates decimal precision and calculations |
| `exception_test` | Tests exception creation and propagation |
| `hello_basar` | Basic smoke test for library linkage |
| `string_operations` | Tests string utilities and transformations |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
