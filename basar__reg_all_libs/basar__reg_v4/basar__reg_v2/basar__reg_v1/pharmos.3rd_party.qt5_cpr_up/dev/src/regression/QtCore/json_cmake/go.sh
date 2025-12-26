#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -rf build CMakeLists.txt

cat > CMakeLists.txt <<EOF
cmake_minimum_required(VERSION 3.10)
project(json_cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMake prefix path to locate Qt5Config.cmake
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}/lib/cmake" ${CMAKE_PREFIX_PATH})

# Find Qt5 Core and Threads
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Threads REQUIRED)

# Add executable target
add_executable(json_cmake main.cpp)

target_compile_options(json_cmake PRIVATE
    -D_LARGE_FILES -D_LARGEFILE_SOURCE -D_REENTRANT -D_THREAD_SAFE -D_GNU_SOURCE
    -Wall -W -maix64 -fPIC -ftls-model=initial-exec -pthread -fno-strict-aliasing
)

target_link_options(json_cmake PRIVATE
    -Wl,-brtl,-G,-bbigtoc -maix64
    -L/usr/lib/threads -pthread
    -L/opt/freeware/lib/gcc/powerpc-ibm-aix7.2.0.0/12/pthread
)

target_include_directories(json_cmake PRIVATE
    "$ENV{QTDIR}/include"
    "$ENV{QTDIR}/include/QtCore"
)

# Try linking with pthread version of libstdc++
target_link_libraries(json_cmake 
    Qt5::Core
    /opt/freeware/lib/gcc/powerpc-ibm-aix7.2.0.0/12/pthread/libstdc++.a
    pthread
    pthreads
    c_r
)
EOF

mkdir build && cd build && cmake ..

# Patch make instructions -isystem -> -I
find . -type f -exec fgrep -l isystem {} \; | while read p; do
  echo "Patching make include in ${p}..."
  sed -e 's# -isystem # -I#g' "${p}" > temp && mv temp "${p}"
done

gmake VERBOSE=1 2>&1             | tee ../go.out
cd .. && ./build/json_cmake 2>&1 | tee -a go.out
