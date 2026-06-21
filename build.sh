#!/bin/bash

# Build script for Restaurant Management System

echo "Building Restaurant Management System..."
echo "========================================="

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ is not installed"
    exit 1
fi

# Compile
echo "Compiling..."
g++ -std=c++17 -I. main.cpp -o restaurant_app

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo ""
    echo "To run the application:"
    echo "  ./restaurant_app"
    echo ""
    echo "Make sure data directory exists:"
    mkdir -p data
    echo "✓ Data directory ready"
else
    echo "✗ Build failed!"
    exit 1
fi
