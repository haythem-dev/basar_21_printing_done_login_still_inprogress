# libbasarlogin Test Suite

## Structure
- `01_library_loading/` - Verify library loads successfully
- `02_login_types/` - Test login type enums and constants
- `03_configuration_basic/` - Test configuration object creation (no XML)
- `04_authentication_states/` - Test state machine without actual auth
- `05_credential_validation/` - Test input validation logic
- `06_session_management/` - Test session data structures

## Each directory contains:
- patterns.txt - Test patterns
- main.cpp - C++ test code
- go.sh - Build script
- makefile - Build configuration

## Usage
cd "01_library_loading - Verify library loads successfully"
make run
