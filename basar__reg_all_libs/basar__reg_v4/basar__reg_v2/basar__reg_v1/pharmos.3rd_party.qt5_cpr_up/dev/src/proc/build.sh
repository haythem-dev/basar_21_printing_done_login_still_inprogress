#!/usr/bin/bash -e

proc_dir=$(pwd)/proc

$proc_dir/build-qtbase.sh $*
$proc_dir/build-qttools.sh $*
$proc_dir/build-qttranslations.sh $*

$proc_dir/do_regression.sh

echo "Cleaning up temporary build files..." >&2
# Remove Git log file if it exists at package root
if [ -f "../../q" ]; then
    echo "Removing Git log file: ../../q" >&2
    rm -f "../../q"
fi
# Remove any other temporary log files that might be created
find ../../ -maxdepth 1 -name "q" -type f -delete 2>/dev/null || true
echo "Cleanup completed." >&2