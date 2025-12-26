
#!/bin/bash

# Basar Regression Test Suite Runner
# Adapted from Qt5 regression test approach

# Set up environment
script_dir=$(cd "$(dirname "$0")" && pwd)
regression_dir="${script_dir}"

# Validate we're in the right place
if [ ! -d "$regression_dir" ]; then
    echo "Error: Must run from basar/regression directory"
    exit 1
fi

# Clean old outputs (but keep source files)
echo "Cleaning old test outputs..."
find "${regression_dir}/" -type f -name "go.out" -exec rm -f {} \;
find "${regression_dir}/" -type f -name "*.o" -exec rm -f {} \;

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0

# Create summary output file
summary_file="${regression_dir}/regression.summary"
> "$summary_file"

echo "=========================================" | tee -a "$summary_file"
echo "  Basar Regression Test Suite" | tee -a "$summary_file"
echo "=========================================" | tee -a "$summary_file"
echo "" | tee -a "$summary_file"
echo "Regression directory: $regression_dir" | tee -a "$summary_file"
echo "Start time: $(date)" | tee -a "$summary_file"
echo "" | tee -a "$summary_file"

# Find and execute all regression tests
test_scripts=$(find "${regression_dir}" -type f -name "go.sh" | sort)

if [ -z "$test_scripts" ]; then
    echo "ERROR: No regression tests found (no go.sh scripts)" | tee -a "$summary_file"
    echo "" | tee -a "$summary_file"
    echo "To create a test, see QUICK_START_GUIDE.md" | tee -a "$summary_file"
    exit 1
fi

for go_script in $test_scripts; do
    test_dir=$(dirname "$go_script")
    test_name=$(echo "$test_dir" | sed "s#${regression_dir}/##")
    patterns_file="$test_dir/patterns.txt"
    
    # Skip if no patterns file
    if [ ! -f "$patterns_file" ]; then
        echo "WARNING: Skipping $test_name (no patterns.txt)" | tee -a "$summary_file"
        continue
    fi
    
    total_tests=$((total_tests + 1))
    
    echo "=========================================" | tee -a "$summary_file"
    echo "Test $total_tests: $test_name" | tee -a "$summary_file"
    echo "=========================================" | tee -a "$summary_file"
    
    # Execute the test
    chmod +x "$go_script"
    cd "$test_dir" || continue
    
    echo "Running: $go_script" | tee -a "$summary_file"
    echo "Working directory: $(pwd)" | tee -a "$summary_file"
    
    # Run the test script
    bash ./go.sh > go.out 2>&1
    test_exit_code=$?
    echo "Exit code: $test_exit_code" | tee -a "$summary_file"
    
    # Check patterns
    go_out="$test_dir/go.out"
    if [ -f "$go_out" ]; then
        echo "" | tee -a "$summary_file"
        
        # Run pattern checker
        if [ -x "${regression_dir}/check_patterns.sh" ]; then
            pattern_output=$("${regression_dir}/check_patterns.sh" "$go_out" "$patterns_file" 2>&1)
            echo "$pattern_output" | tee -a "$summary_file"
            
            # Check if all patterns matched (100% success rate)
            if echo "$pattern_output" | grep -q "Success rate: 100%"; then
                echo "✓ PASSED" | tee -a "$summary_file"
                passed_tests=$((passed_tests + 1))
            else
                echo "✗ FAILED" | tee -a "$summary_file"
                failed_tests=$((failed_tests + 1))
                
                # Show first few lines of actual output for debugging
                echo "" | tee -a "$summary_file"
                echo "First 10 lines of test output:" | tee -a "$summary_file"
                head -10 "$go_out" | sed 's/^/  /' | tee -a "$summary_file"
            fi
        else
            echo "ERROR: check_patterns.sh not found or not executable" | tee -a "$summary_file"
            echo "✗ FAILED" | tee -a "$summary_file"
            failed_tests=$((failed_tests + 1))
        fi
    else
        echo "ERROR: No output file generated (go.out missing)" | tee -a "$summary_file"
        echo "✗ FAILED" | tee -a "$summary_file"
        failed_tests=$((failed_tests + 1))
    fi
    
    echo "" | tee -a "$summary_file"
    cd "$regression_dir" || exit 1
done

# Print final summary
echo "=========================================" | tee -a "$summary_file"
echo "  Regression Test Results" | tee -a "$summary_file"
echo "=========================================" | tee -a "$summary_file"
echo "" | tee -a "$summary_file"
echo "Total tests:  $total_tests" | tee -a "$summary_file"
echo "Passed:       $passed_tests" | tee -a "$summary_file"
echo "Failed:       $failed_tests" | tee -a "$summary_file"
echo "" | tee -a "$summary_file"
echo "End time: $(date)" | tee -a "$summary_file"
echo "" | tee -a "$summary_file"

if [ $total_tests -eq $passed_tests ]; then
    echo "SUCCESS: All regression tests passed! ✓" | tee -a "$summary_file"
    exit 0
else
    echo "FAILURE: $failed_tests of $total_tests tests failed ✗" | tee -a "$summary_file"
    echo "" | tee -a "$summary_file"
    echo "See $summary_file for details" | tee -a "$summary_file"
    exit 1
fi
