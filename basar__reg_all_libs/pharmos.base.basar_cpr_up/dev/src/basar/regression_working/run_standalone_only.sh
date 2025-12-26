
#!/bin/bash

# Run only standalone tests that don't require Basar libraries

regression_dir=$(cd "$(dirname "$0")" && pwd)

echo "========================================="
echo "  Standalone Tests Only (No Libraries)"
echo "========================================="
echo ""

# Run infrastructure test
cd "${regression_dir}/standalone/infrastructure_test"
echo "Running infrastructure_test..."
./go.sh

echo ""
echo "Checking results..."
"${regression_dir}/check_patterns.sh" go.out patterns.txt

echo ""
echo "========================================="
echo "Test output saved to:"
echo "  ${regression_dir}/standalone/infrastructure_test/go.out"
echo "========================================="
