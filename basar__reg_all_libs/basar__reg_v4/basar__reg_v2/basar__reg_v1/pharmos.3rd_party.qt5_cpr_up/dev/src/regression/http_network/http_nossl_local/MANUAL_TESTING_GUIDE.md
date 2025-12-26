#Author: Haythem ben abdelaziz
# HTTP Network Local Server Manual Testing Guide

## Overview

This test validates Qt5 HTTP networking functionality using a local test server instead of external services. This approach provides controlled, reliable testing that doesn't depend on external network connectivity.

**Note**: This test is excluded from automated regression testing (`ant full`) and must be executed manually due to the requirement for a separate local server process.

## Prerequisites

- Qt5 installation with development tools (qmake, gmake)
- Python 3.x for running the local test server
- Two terminal sessions (one for server, one for test client)

## Manual Execution Steps

### Step 1: Navigate to Test Directory

```bash
cd pharmos.3rd_party.qt5/dev/src/regression/http_network/http_nossl_local
```

### Step 2: Start Local Test Server (Terminal 1)

```bash
# Start the HTTP test server on port 8080
python3 ../local_test_server.py 8080

# Expected output:
# Qt AIX Test HTTP Server starting on localhost:8080
# Test endpoints available:
#   GET  http://localhost:8080/get
#   POST http://localhost:8080/post
#   PUT  http://localhost:8080/put
# Press Ctrl+C to stop server
```

**Keep this terminal running** - the server must remain active during test execution.

### Step 3: Execute Qt Test Client (Terminal 2)

Open a second terminal and navigate to the same directory:

```bash
cd pharmos.3rd_party.qt5/dev/src/regression/http_network/http_nossl_local

# Run the test script
./go.sh
```

## Expected Test Output

### Successful Test Execution

```
Qt HTTP Network Test (No SSL - Local Server)
=============================================

Qt environment:
QTDIR: /path/to/pkg-install
PATH: /path/to/pkg-install/bin:...
LD_LIBRARY_PATH: /path/to/pkg-install/lib:...

Debug: Checking Qt installation...
✓ qmake found at: /path/to/pkg-install/bin/qmake

Cleaning previous build...
Generating Makefile...
Building test...
gmake clean
gmake

Checking library dependencies...
Library dependencies:
http_nossl_local needs:
    libQt5Network.so.5 => /path/to/qt/lib/libQt5Network.so.5
    libQt5Core.so.5 => /path/to/qt/lib/libQt5Core.so.5
    ...

Checking for local test server...
✓ Local test server is available at http://localhost:8080

Running HTTP network test (local server)...
===========================================
Qt HTTP Network Regression Test for AIX (Local Server)
Version: 1.0 (SSL Disabled - Local Testing)
Platform: AIX 7.2
Qt Version: 5.15.16
Build Date: Jan 15 2024 14:30:22
========================================
Starting HTTP Network Test (SSL disabled)
Qt Network module version: 5.15.16
========================================
Test 1: HTTP GET request
HTTP GET request sent to http://localhost:8080
Test 2: HTTP POST request  
HTTP POST request sent with JSON data
Test 3: HTTP PUT request
HTTP PUT request sent
All HTTP tests initiated. Waiting for responses...
========================================

[Response details for each HTTP method...]

ALL NETWORK TESTS COMPLETED SUCCESSFULLY
Test execution completed with code: 0

Cleaning up...
```

### Server Terminal Output

The server terminal should show incoming requests:

```
[2024-01-15 14:30:25] "GET /get HTTP/1.1" 200 -
[2024-01-15 14:30:25] "POST /post HTTP/1.1" 200 -
[2024-01-15 14:30:25] "PUT /put HTTP/1.1" 200 -
```

## Test Scenarios

### Scenario 1: Basic Functionality Test

**Purpose**: Verify Qt HTTP networking works with local server

**Steps**:
1. Start server on default port 8080
2. Run `./go.sh`
3. Verify all 3 HTTP methods (GET, POST, PUT) succeed

**Success Criteria**:
- No compilation errors
- All HTTP requests return 200 status
- Test outputs "ALL NETWORK TESTS COMPLETED SUCCESSFULLY"
- Exit code is 0

### Scenario 2: Port Conflict Handling

**Purpose**: Test behavior when default port is occupied

**Steps**:
1. Start server on alternate port: `python3 ../local_test_server.py 8081`
2. Modify test to use alternate port (edit main.cpp line 20)
3. Run test

**Expected**: Test should still work with different port configuration

### Scenario 3: Server Unavailable Test

**Purpose**: Verify graceful handling when server is not running

**Steps**:
1. Ensure no server is running on port 8080
2. Run `./go.sh`

**Expected Output**:
```
⚠ Local test server not found at http://localhost:8080
Start the test server with: python3 ../local_test_server.py 8080
Continuing with test (will show connection errors)...
```

Test will show connection failures but should not crash.

## Troubleshooting

### Common Issues

#### 1. "qmake: command not found"

**Solution**: Set Qt environment variables:
```bash
export QTDIR=/path/to/qt5/installation
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
```

#### 2. "gmake: command not found"

**Solution**: Install GNU make or use system make:
```bash
# AIX: Install GNU make
# Or modify go.sh to use 'make' instead of 'gmake'
```

#### 3. Server Port Already in Use

**Error**: `OSError: [Errno 48] Address already in use`

**Solution**: 
- Use different port: `python3 ../local_test_server.py 8081`
- Kill existing process: `lsof -ti:8080 | xargs kill`

#### 4. Qt Library Not Found

**Error**: `error while loading shared libraries: libQt5Core.so.5`

**Solution**: Check library path:
```bash
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
ldd ./http_nossl_local  # Verify all libraries found
```

#### 5. Compilation Errors

**Error**: Missing headers or link errors

**Solution**: Verify Qt development packages installed:
```bash
# Check qmake configuration
qmake -query
# Verify Qt5Network module available
```

## Integration Notes

### Why Manual Testing Only?

This test requires **two concurrent processes**:
1. Python HTTP server (background process)
2. Qt test client (foreground process)

The automated regression system (`do_regression.sh`) executes tests in single-process context, making automatic server management complex "a bit" (...).

### Alternative for Automation

For automated testing, use the `http_nossl` test instead, which:
- Tests against external HTTP services
- Requires only single process execution
- Included in `ant full` regression suite

### Local vs External Testing

| Aspect | Local Server (`http_nossl_local`) | External Server (`http_nossl`) |
|--------|-----------------------------------|-------------------------------|
| Network dependency | None | Internet required |
| Reliability | High (controlled) | Variable (external services) |
| Execution | Manual only | Automated + Manual |
| Use case | Development/debugging | CI/CD validation |

## Maintenance

### Updating Test Server

To modify server behavior, edit `../local_test_server.py`:
- Add new endpoints
- Change response formats
- Add error simulation

### Modifying Test Client

To change test behavior, edit `main.cpp`:
- Update server URL (line 20)
- Modify test parameters
- Add additional test cases

### Port Configuration

Default port 8080 can be changed:
1. Update server command: `python3 ../local_test_server.py [PORT]`
2. Update client URL in `main.cpp`: `setBaseUrl("http://localhost:[PORT]")`

## Example Complete Test Session

```bash
# Terminal 1 - Start server
$ cd pharmos.3rd_party.qt5/dev/src/regression/http_network/http_nossl_local
$ python3 ../local_test_server.py 8080
Qt AIX Test HTTP Server starting on localhost:8080
...server running...

# Terminal 2 - Run test
$ cd pharmos.3rd_party.qt5/dev/src/regression/http_network/http_nossl_local  
$ ./go.sh
Qt HTTP Network Test (No SSL - Local Server)
...test execution...
ALL NETWORK TESTS COMPLETED SUCCESSFULLY
Test execution completed with code: 0

# Terminal 1 - Stop server
^C
Shutting down server...
```

This manual testing approach provides reliable, controlled validation of Qt5 HTTP networking functionality without external dependencies.