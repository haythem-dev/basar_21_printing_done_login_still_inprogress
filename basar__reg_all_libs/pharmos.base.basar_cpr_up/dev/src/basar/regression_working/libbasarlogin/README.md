# libbasarlogin

## Role in Basar Package

**libbasarlogin** is the authentication and authorization library for Basar. It provides security infrastructure:

- User authentication workflows
- LDAP directory integration
- Session management and tracking
- Permission checking and access control
- Credential validation and storage

This library handles all security-related operations for Basar applications, supporting both local and LDAP-based authentication.

## Dependencies

- `libopensslwrap` - SSL/TLS for secure connections
- `libbasarcmnutil` - Common utilities
- OpenLDAP client libraries

## Regression Tests

| Test | Description |
|------|-------------|
| `authentication_flow_test` | Tests complete authentication workflows |
| `configuration_test` | Validates login configuration handling |
| `ldap_config_test` | Tests LDAP connection configuration |
| `permission_check_test` | Validates permission verification logic |
| `session_management_test` | Tests session creation and lifecycle |
| `user_credentials_test` | Validates credential handling and validation |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## Additional Documentation

- `LDAP_AUTHENTICATION_DOCUMENTATION_see14.md` - LDAP setup details
- `LIBPATH_FIX_DOCUMENTATION.md` - AIX library path configuration
- `OPENSSL_DEPENDENCY_RESOLUTION.md` - OpenSSL linking notes
