#!/bin/bash

resourcesFile="resources.qrc"
rm $resourcesFile
echo "Creating $resourcesFile"

touch $resourcesFile
echo "<RCC>" >>$resourcesFile
echo -e "\t<qresource prefix=\"/\">" >>$resourcesFile

echo -e "\t\t<file>DefaultStyle.json</file>" >>$resourcesFile


echo "Adding shader files"
for file in shaders/*; do
  if [[ -f "$file" ]]; then
    echo -e "\t\t<file>$file</file>" >> $resourcesFile
  fi
done

echo "Adding icons"
for file in icons/*; do
  if [[ -f "$file" ]]; then
    echo -e "\t\t<file>$file</file>" >> $resourcesFile
  elif [[ -d "$file" ]]; then
    for icon in $file/*; do
      if [[ -f "$icon" ]]; then
        echo -e "\t\t<file>$icon</file>" >> $resourcesFile
      fi
    done
  fi
done

echo -e "\t</qresource>" >>$resourcesFile
echo "</RCC>" >>$resourcesFile
echo "Done"
