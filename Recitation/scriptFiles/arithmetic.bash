#!/bin/bash

#input two numbers
#output the sum and the product

  echo "Enter one number"
  read number1
  echo "Enter a second number"
  read number2
  let sum=$number1+$number2
  echo "Sum equals to $sum"
  let product=$number1*$number2
  echo "product equals to $product"
