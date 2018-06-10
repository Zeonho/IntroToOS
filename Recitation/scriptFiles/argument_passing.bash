#!/bin/bash
echo "File Name: $0"
echo "Total Number of Parameters : $#"
echo "Quoted Values: $*"

echo "Argument 1 equals to $1 "
echo "Argument 2 equals to $2 "

let sum=$1+$2
echo "Sum = $sum"
