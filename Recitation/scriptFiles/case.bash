#!/bin/bash
echo -n "Enter the name of an animal: "
read ANIMAL

echo -n "The $ANIMAL has "
case $ANIMAL in
        'horse') echo -n "four";;
        'dog' ) echo -n "four";;
        'cat') echo -n "four";;
        'man' ) echo -n "two";;
        'kangaroo' ) echo -n "two";;
        *) echo -n "an unknown number of";;
esac
echo " legs."
