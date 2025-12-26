#!/usr/bin/ksh

find "${PWD}/regression/" -type f | egrep -v '(patterns\.txt|\.sh|\.pro|\.cpp|\.h|\.md|\.py)$' | xargs rm
regressions=0

# First pass: count how many go.sh scripts will be run
find "${PWD}/regression/" -type f -name "patterns.txt" | while IFS= read -r pattern_file; do
    dir=$(dirname "$pattern_file")
    if [ -r "$dir/go.sh" ]; then
	regressions=$((regressions + 1))
    fi
done
countdown=${regressions}

# Report how many scripts will be executed
echo "Number of regression scripts to execute: ${countdown}"

find "${PWD}/regression" -type f -name go.sh |\
    while read go; do
	goout=$(echo "${go}" | sed -e 's#sh$#out#')
	gopath=$(dirname "${go}")

	if [ -r "${gopath}/patterns.txt" ]; then
	    printf "==>\n==> %02d regressions to do. Regression test in %s:\n==>\n" \
		   "${countdown}" $(echo "${gopath}" | sed 's#.*dev/src/##')
	    chmod a+x "$go" && "$go"
	    "${PWD}/regression/check_patterns.sh" "${goout}" "${gopath}/patterns.txt" 2>&1 |\
		tee -a "${PWD}/regression/regression.out"
            countdown=$((countdown - 1))
	fi
    done

printf "===\n=== Regression Tests Summary: ===\n===\n"
fgrep ==\> "${PWD}/regression/regression.out"
successes=$(fgrep "Success rate: 100%" "${PWD}/regression/regression.out" | wc -l)
printf "===\n=== %d%% (%d of %d regressions) succeeded completely.\n\n" \
       $((successes * 100 / regressions)) \
       ${successes} ${regressions}

# This regression will test some features of Qt in a very generic way.
# The tests are expected to reside in the regression/ directory hierarchy.
#
# A particular regression will include these components:
# - the go.sh shell script to kick the regression off
# -- this script will be searched for and is mandatory for the particular
#    regression as of now
# -- it should contain the compilation instructions of the source code
# -- furthermore, it should output all important messages of the compilation
#    and test case to the go.out file in the same directory
# - patterns.txt; this file will contain all regular expressions that
#   should match the go.out output to indicate one successful regression check
# - the source code
# - compilation instructions, e.g. a Makefile, or a Qt .pro file
#
# You are very much invited to use the existing examples as a template to
# create new ones, and to enhance the approach, e.g. by checking for
# "anti-patterns" as well!
