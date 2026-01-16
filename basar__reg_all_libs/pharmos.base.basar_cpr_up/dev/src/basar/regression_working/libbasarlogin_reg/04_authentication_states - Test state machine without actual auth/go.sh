#!/bin/bash
echo "Building $1..."
make clean
make

if [ $? -eq 0 ]; then
    echo "Running tests..."
    make run
else
    echo "Build failed!"
    exit 1
fi
