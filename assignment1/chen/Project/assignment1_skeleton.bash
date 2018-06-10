#!/bin/bash

#---------------------------------------------------------------------
# CSci 4061 Fall 2017
# Assignment# 1
# This work must be done individually.
# Student name:  
# Student: 
# x500 id:  
# Operating system on which you tested your code: Ubuntu 16.04
# CSELABS machine: <machine you tested on eg: xyz.cselabs.umn.edu>
#---------------------------------------------------------------------


echo " "

######################
# Directives for usage

 if [ $argv == 0 ]
   then
    output="Process argument missing \n"
    echo -e $output
   else
    output="SELECT THE FUNCTION YOU WANT TO EXECUTE: \n"
    echo -e $output
    output="1. Search for files"
    echo -e $output
    output="2. Calculate total length"
    echo -e $output
    output="3. Find zero length files"
    echo -e $output
    output="4. Create backup files"
    echo -e $output
    output="5. Exit"
    echo -e $output

 fi

  echo -e "\nEnter option "
  read option

#  echo $option

  case "$option" in

	1)  echo "Searchng for files"
	    #here you should implement for search files
	    # Begin Code
      echo -e "\nEXECUTING FILE SEARCH FUNCTION"
      echo -n "ENTER DIRECTORY NAME: "
      read Directory
      echo -n "ENTER FILE NAME: "
      read FileName

      echo

      # finding all dirs with name that user input(Start From home Directory)
      DIRS=`find $HOME -name $Directory`
      # checking if this dir exist
      if [[ -z ${DIRS} ]]
      then
        echo "Directory Not Exist, Now Exiting the Program..."
        exit
      fi
      # itering through each dir and search for the file
      for DIR in $DIRS;
      do
        # print full path
        echo "PATH:$DIR"

        FILES=`find $DIR -name $FileName`

        # Exit if no file is Found
        if [[ -z ${FILES} ]]
        then
          echo "Sorry, I can't found any file name $FileName, Now Exiting the Program..."
          exit
        fi


        # Destect if it is a directory or file
        for FILE in $FILES;
        do
          if [[ -f $FILE ]]
          then
            echo "Found Ordinary File: "
            ls -l $FILE
          else
            echo "Found Directory File: "
            ls -ld $FILE
          fi
        done

      echo
      done
	    #End Code
	    ;;
	2)  echo "Calculating total of the size of all files in the directory tree"
	    #here you should implement the code to calculate the size of all files in a folder
	    # Begin Code
      # filelist=*
      filelist=`find -type f $PWD`
      total=0
      for filename in $filelist;
      do
        size=`stat -c%s $filename`
        total=`expr $total + $size`
      done
      echo "Total size of files in bytes is $total"
	    #End Code
	    ;;

	3) echo "Finding zero length files"
	    #here you should implement the code to find empty files
	    # Begin Code
      echo -n "ENTER DIRECTORY NAME:"
      read Directory
      # echo -e "\nI found these Zero files for you:"
      # find $Directory -size 0

      DIRS=`find $HOME -name $Directory`
      # checking if this dir exist
      if [[ -z ${DIRS} ]]
      then
        echo "Directory Not Exist, Now Exiting the Program..."
        exit
      fi
      # itering through each dir and search for the file
      for DIR in $DIRS;
      do
        # print full path
        echo "PATH:$DIR"
        echo -e "\nI found these Zero files for you:"
        # set current files
        find $DIR -maxdepth 1 -size 0

      done

	    #End Code
	    ;;

	 4) echo  "Creating backup files"
	    #here you should implement the backup code
	    # Begin Code


      echo -e "\nBACKUP FUNCTION"

      # Asking user for input
      echo -n "ENTER DIRECTORY NAME: "
      read Directory
      echo -n "ENTER FILE NAME: "
      read FileName
      echo


      # finding all dirs with name that user input(Start From home/user Directory)
      DIRS=`find $HOME -name $Directory`
      # checking if this dir exist
      if [[ -z ${DIRS} ]]
      then
        echo "Directory Not Exist, Now Exiting the Program..."
        exit
      fi
      # itering through each dir and search for the file
      for DIR in $DIRS;
      do
        # print full path
        echo "PATH:$DIR"

        FILES=`find $DIR -name $FileName`

        # Exit if no file is Found
        if [[ -z ${FILES} ]]
        then
          echo "Sorry, I can't found any file name $FileName, Now Exiting the Program..."
          exit
        fi


        # Destect if it is a directory or file
        for FILE in $FILES;
        do
          if [[ -f $FILE ]]
          then
            # BackupFile Program
            echo "Found Ordinary File: "

            BackupFile=`find $DIR -name $FileName.bak | head -1`

            if [[ ${BackupFile} ]]; then
              #BackupFile Exist
              echo "BackupFile Exist, Now Comparing the differences..."
              DIFF=`diff $FILE $BackupFile`
              if [[ $DIFF ]]; then
                #different

                date=`date +%m-%d-%Y`
                mv $BackupFile $BackupFile-$date
                echo "BackupFile is Different From Original File"
                echo "Creating New BackupFile..."
                cp $FILE $FILE.bak
                echo "$FILE.bak Created..."
              else
                echo "BackupFile is up-to-date..."
              fi
            else
              #BackupFile Not Exist
              echo "BackupFile Not Exist, Now Creating a New One..."
              cp $FILE $FILE.bak
              echo "$FILE.bak Created..."
            fi

          else
            # BackupDir Program
            echo "Found Directory File: "


            BackupDir=`find $DIR -name $FileName.bak | head -1`

            if [[ ${BackupDir} ]]; then
              #BackupDir Exist
              echo "BackupDir Exist..."
              date=`date +%m-%d-%Y`
              mv $BackupDir $BackupDir-$date
              echo "Creating New BackupDir..."
              cp -ra $FILE $BackupDir
              echo "$FILE.bak Created..."

            else
              #BackupFile Not Exist
              echo "BackupDir Not Exist, Now Creating a New One..."
              cp -ra $FILE $FILE.bak
              echo "$FILE.bak Created..."
            fi


          fi
        done

      echo
      done





	    #End Code
	    ;;

	5) echo "Exiting the program. Thanks"
	  exit
	  ;;
   esac
