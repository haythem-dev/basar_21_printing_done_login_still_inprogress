# libopensslwrap

## Role in Basar Package

**libopensslwrap** is the OpenSSL wrapper library for Basar. It provides a simplified C++ interface to OpenSSL functionality:

- SSL/TLS context management
- SSL connection methods and protocols
- Error handling and reporting
- OpenSSL initialization and cleanup
- Version compatibility handling

This library abstracts OpenSSL complexity and provides a consistent interface for secure communications used by `libbasarlogin` and other security-sensitive components.

## Dependencies

- OpenSSL libraries (libssl, libcrypto)

## Regression Tests

| Test | Description |
|------|-------------|
| `ssl_context_test` | Tests SSL context creation and configuration |
| `ssl_error_test` | Validates SSL error handling and reporting |
| `ssl_init_test` | Tests OpenSSL initialization sequence |
| `ssl_method_test` | Validates SSL/TLS method selection |
| `ssl_version_test` | Tests SSL version compatibility |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## AIX Notes

OpenSSL libraries must be in LIBPATH. Tests configure this automatically via `go.sh`.
