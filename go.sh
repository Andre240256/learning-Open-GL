# !/bin/bash

set -e

echo "Building the project..."

PROJECT_ROOT=$( cd -- "$( dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd )

echo "Project root directory: $PROJECT_ROOT"

cd "$PROJECT_ROOT"

echo "Compiling the project..."
cmake --build build

echo "Executing the program..."
./build/src/executable