#!/bin/bash

# Formats all cpp files

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"
cd ..

SOURCE_EXTENSIONS=".cpp .hpp .h"

# Check if clang-format is installed
if ! command -v clang-format &>/dev/null; then
  echo "clang-format not found. Please install clang-format."
  exit 1
fi


for ext in $SOURCE_EXTENSIONS; do
  # Find all source files with the specified extension
  files=$(find src include modules -name "*$ext")

  # Run clang-format and check for changes
  for file in $files; do
    echo "Formatting: $file"
    clang-format -style=file:.clang-format "$file" -i
  done
done

echo ""
echo "Formatting complete."
echo ""

cd "$initial_loc"
