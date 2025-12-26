 # Qt 5.15.16 FTP Download Regression Test

## Overview
This regression test validates Qt 5.15.16 FTP download functionality on AIX 64-bit systems.

## Files
- `main.cpp`: Main application source using QProcess approach
- `ftp_download.pro`: Qt project file with AIX-specific configuration
- `go.sh`: Build and execution script (organizational standard compliant)
- `pattern.txt`: Success patterns for automated evaluation
- `README.md`: This documentation file

## Execution
```bash
# Make script executable
chmod +x go.sh

# Run test and capture output
./go.sh > go.out 2>&1

# Evaluate results against patterns
grep -f pattern.txt go.out | wc -l
```

## Success Criteria
The test passes when:
1. Build completes without errors
2. Application executes successfully (exit code 0)
3. File is downloaded with expected size (891 bytes)
4. All success patterns are matched in go.out

## Expected Results
- Exit code: 0
- Downloaded file: dummy.txt (891 bytes)
- All patterns from pattern.txt found in go.out

## Organizational Compliance
This test follows the standard regression pattern:
- go.sh: Compilation and execution
- go.out: Captured output
- pattern.txt: Success criteria
- External evaluation: Pattern matching analysis