#!/bin/bash

# Define the build directory
BUILD_DIR="build"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
fi

# Navigate into the build directory
cd $BUILD_DIR

# Run CMake and Make
cmake ..
make

# Check if build was successful
if [ $? -eq 0 ]; then
    # Run the program (replace 'YourProjectName' with your actual project name)
    ./Opengl
else
    echo "Build failed."
fi

# Return to the original directory
cd ..