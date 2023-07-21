#!/bin/bash

# Setup the correct working directory
initial_loc=$(pwd)
cd "$(dirname "${BASH_SOURCE[0]}")"

# Removes the original file
resourcesFile="resources.qrc"
rm $resourcesFile

# All the files to add
file_list=$(find . -type f -not -name "*.sh" -not -name "*.bash" -not -name "*.qrc" -print)
echo "Creating $resourcesFile"

# Create resources.qrc file
touch $resourcesFile
echo "<RCC>" >>$resourcesFile
echo -e "\t<qresource prefix=\"/\">" >>$resourcesFile

for file in $file_list; do
  echo -e "\t\t<file>$file</file>" >>$resourcesFile
  echo "Added ${file}"
done

echo -e "\t</qresource>" >>$resourcesFile
echo "</RCC>" >>$resourcesFile

# Done and navigate back
echo "Done"
cd "$initial_loc"
