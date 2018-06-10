#!/bin/bash
echo \${variable:=word}
echo ${PERSON1:=John Doe}
echo $PERSON1

echo \{variable:-word}
echo ${PERSON2:-John Doe}
echo $PERSON2

PERSON3=hello
echo \${variable:+word}
echo ${PERSON3:+John Doe}
echo $PERSON3
