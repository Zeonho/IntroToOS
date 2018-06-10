#!/bin/bash

echo "Enter the name of the folder"
read folder

list=$(find $folder -type f)
for file in $list;
do
    fileSize=$(stat -c%s "$file")
    echo "Size of $file = $fileSize bytes."
done
