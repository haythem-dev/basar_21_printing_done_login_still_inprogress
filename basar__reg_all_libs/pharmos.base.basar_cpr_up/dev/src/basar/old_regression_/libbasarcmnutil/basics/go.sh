#!/bin/bash
# SPDX-FileCopyrightText: 2022-2023 Technology Solutions Inc.
# SPDX-License-Identifier: Apache-2.0

set -x

export BASEDIR=../../../../..
# Print the current working directory to stdout.
pwd
# Export the MARATHON_LOG_CONFIGDIR environment variable.
# This variable is used to specify the directory for log configuration files.
export MARATHON_LOG_CONFIGDIR=$(pwd)
# Print the current working directory to stdout again.
pwd
# Export the MARATHON_LOG_LOGSDIR environment variable.
# This variable is used to specify the directory for log files.
export MARATHON_LOG_LOGSDIR=$(pwd)

# Read the contents of the test_basar.cfg file from standard input.
# Redirect standard input from a temporary file.
cat 1> test_basar.cfg 0< /tmp/sh18416114.2

# Compile the test_basar.cpp file using g++-12.
# -I flags specify include directories for header files.
# -L flags specify library directories for linking.
# -l flags specify libraries to link against.
/opt/freeware/bin/g++-12 \
  -I../../../../libbasarcmnutil \
  -I../../../../../ext/unittestpp/include \
  -I../../../../../ext/log4cplus/include \
  -I../../../../../ext/boost/include \
  -fPIC -std=c++17 -Wl,-brtl -maix64 -lpthread -lpthreads -pthread \
  -L../../../../../ext/unittestpp/lib/Release \
  -L../../../../../ext/zlib/lib/Release \
  -L/informix/informix11.70FC5/lib \
  -L/informix/informix11.70FC5/lib/esql \
  -o test_basar test_basar.cpp \
  -lbasarcmnutil -lbasardebug -lUnitTest++ 2>&1 | tee go.out

# List the dynamic dependencies of the test_basar executable.
# tee -a appends the output to go.out.
ldd ./test_basar | tee -a go.out

# Execute the test_basar program.
# Redirect standard error to standard output.
./test_basar 2>& 1 | tee go.out