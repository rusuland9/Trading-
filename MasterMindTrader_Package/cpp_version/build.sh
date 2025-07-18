#!/bin/bash

echo "MasterMind Trader - Build Script"
echo "================================"

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Navigate to build directory
cd build

# Configure with CMake
echo "Configuring project with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Check if CMake configuration was successful
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build the project
echo "Building MasterMind Trader..."
cmake --build . --config Release

# Check if build was successful
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build completed successfully!"
echo "You can now run ./MasterMindTrader" 