# libbasardebug

## Role in Basar Package

**libbasardebug** is the debugging and diagnostics library for Basar. It provides development and troubleshooting tools:

- Debug output formatting and routing
- Memory leak detection utilities
- Build server integration for CI/CD
- Error printing and stack trace utilities
- Memory allocation tracking with ignore patterns

This library is primarily used during development and testing to identify issues and validate code quality.

## Regression Tests

| Test | Description |
|------|-------------|
| `build_server_test` | Tests CI/CD build server integration |
| `debug_output_test` | Validates debug message formatting and output |
| `leak_check_test` | Tests memory leak detection mechanisms |
| `memory_ignore_test` | Validates memory tracking ignore patterns |
| `print_error_test` | Tests error printing and formatting |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
