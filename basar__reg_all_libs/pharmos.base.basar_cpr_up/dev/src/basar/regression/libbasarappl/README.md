# libbasarappl

## Role in Basar Package

**libbasarappl** is the application framework library for Basar. It provides core application infrastructure including:

- Event handling and dispatch mechanisms
- Application initialization and lifecycle management
- Logging pool management for centralized log handling
- System event processing
- Utility functions for common application operations

This library serves as the foundation for building Basar-based applications, providing the essential plumbing that other libraries depend on.

## Regression Tests

| Test | Description |
|------|-------------|
| `event_executor_test` | Validates event execution and callback mechanisms |
| `event_macros_test` | Tests event macro definitions and expansions |
| `functions_test` | Verifies core utility functions |
| `init_test` | Tests application initialization sequence |
| `logger_pool_test` | Validates centralized logging pool operations |
| `system_event_test` | Tests system-level event handling |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
