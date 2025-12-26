#!/bin/bash

# Usage: ./check_patterns.sh filename patterns_file
# filename: file to check
# patterns_file: file containing patterns (one per line)

FILE="$1"
PATTERNS_FILE="$2"

if [ -z "$FILE" ] || [ -z "$PATTERNS_FILE" ]; then
    echo "Usage: $0 <file_to_check> <patterns_file>"
    exit 1
fi

printf "\n==> Checking ]%s[ for patterns from ]%s[\n" \
$(echo "${FILE}" | sed 's#.*dev/src/##') \
$(echo "${PATTERNS_FILE}" | sed 's#.*dev/src/##') 

# Read patterns into an array
mapfile -t patterns < "$PATTERNS_FILE"

total_patterns=${#patterns[@]}
found_count=0

# Loop through patterns
for pattern in "${patterns[@]}"; do
    if grep -q "$pattern" "$FILE"; then
	echo "Pattern '$pattern' found."
	((found_count++))
    else
	echo "Pattern '$pattern' NOT found."
    fi
done

# Calculate percentage
percentage=0
if (( total_patterns > 0 )); then
    percentage=$((found_count * 100 / total_patterns))
fi

echo "==> $found_count out of $total_patterns patterns found. Success rate: $percentage%"
