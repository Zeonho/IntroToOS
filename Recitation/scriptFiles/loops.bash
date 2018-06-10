#!/bin/bash

# Usage of for loop
for i in $( ls ); do
    echo item: $i
done

# Usage of while loop
COUNT=0
while [ $COUNT -lt 10 ]; do
    echo The counter value is $COUNT
    let COUNT+=1
done

# Usage of until loop
COUNT=20
until [ $COUNT -lt 10 ]; do
    echo The counter value is $COUNT
    let COUNT-=1
done
