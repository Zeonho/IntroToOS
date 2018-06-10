echo "File Name: $0"
echo "sourceFile  equals to $1 "
echo "destinationFile  equals to $2 "
sourceFile=$1
destinationFile=$2


if [ -e $destinationFile ]
then
        echo "$destinationFile is exist, do you need to overwrite it?(y/n)"
        read answer

else
        echo "$datafile not found"
fi
