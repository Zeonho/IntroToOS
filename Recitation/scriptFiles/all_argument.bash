#!/bin/bash
echo "Argument are $*"
echo "Number of arguments is $#"
num=0
for x  in  $*
do
        #num=`expr $num + 1`;
        let num=$num+1
        echo "Arg $num is $x."
done
