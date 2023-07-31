#!/bin/bash
# Builds the documentation and pushes it to the gh-pages branch

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"

cd ..

project_version=$(head -n 1 version.txt)
( cat docs/Doxyfile ; echo "PROJECT_NUMBER = $project_version" ) | doxygen -

# Exit if any command fails
set -e

target_branch="gh-pages"

tmp_dir=$(mktemp -d)
# Copy the generated documentation to the temporary directory
cp -r docs/html/* "$tmp_dir/"
# Switch to the target branch
git checkout "$target_branch"
# Remove existing documentation from the target branch
rm -rf ./*
# Copy the new documentation to the target branch
cp -r "$tmp_dir/"/* .
# Add the changes and commit
git add .
git commit -m "Updated documentation"
# Push the changes to the remote repository
git push origin "$target_branch"
# Switch back to the previous branch
git checkout -
# Cleanup the temporary directory
rm -rf "$tmp_dir"

echo "Documentation generated and pushed to $target_branch branch."

git submodule init
git submodule update

cd "$initial_loc"
