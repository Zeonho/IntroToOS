#!/bin/bash

# Read the name of the user

echo "What is your name?"
read PERSON
# if name is undefine, then use John Doe
echo "Hello, Your name is : ${PERSON:=John Doe}"
