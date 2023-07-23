#!/bin/bash

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

temp_file="checkstyle_temp.txt"

touch "$temp_file"

# Check for unformatted files
unformatted_files=()
for ext in $SOURCE_EXTENSIONS; do
  # Find all source files with the specified extension
  files=$(find src include modules -name "*$ext")

  # Run clang-format and check for changes
  for file in $files; do
    echo "Checking file: $file"
    echo ""
    clang-format -style=file:.clang-format "$file" > "$temp_file"
    format_diff=$(diff -u "$file" "$temp_file")
    echo "$format_diff"
    if [ "$format_diff" != "" ]; then
      unformatted_files+=("$file")
      echo "File $file is not properly formatted"
    else
      echo "File $file is properly formatted"
    fi
    echo ""
    exit
  done
done

rm "$temp_file"

# Check if there are any unformatted files
if [ ${#unformatted_files[@]} -eq 0 ]; then
  echo "Code is properly formatted."
else
  echo "The following files are not properly formatted:"
  for file in "${unformatted_files[@]}"; do
    echo " - $file"
  done
  exit 1
fi

cd "$initial_loc"
