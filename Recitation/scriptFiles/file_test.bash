#!/bin/bash

echo "Enter File"
read datafile
if [ -e $datafile ]
then
        echo "We found $datafile"
else
        echo "$datafile not found"
fi
