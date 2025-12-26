# libbasarproperty

## Role in Basar Package

**libbasarproperty** is the property management library for Basar. It provides a flexible property system for data modeling:

- Property type definitions and validation
- Property state management
- Column-based property operations
- Property table structures
- Property description metadata

This library enables dynamic data modeling where object properties can be defined and manipulated at runtime, supporting the aspect-oriented approach in `libbasardbaspect`.

## Dependencies

- `libbasarcmnutil` - Common utilities

## Regression Tests

| Test | Description |
|------|-------------|
| `column_operations_test` | Tests column-based property operations |
| `property_description_test` | Validates property metadata handling |
| `property_state_test` | Tests property state transitions |
| `property_table_test` | Validates property table structures |
| `property_type_test` | Tests property type definitions |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
