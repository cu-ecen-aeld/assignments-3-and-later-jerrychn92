#!/bin/bash

# check that there are 2 arguments passed in
if [ $# -ne 2 ]
then 
    echo "ERROR: Incorrect number of arguments.\n"
    exit 1
fi

# load in the args
sz_filesdir=$1
sz_searchstr=$2

# Check the files directory argument is legal
if [ -d $sz_filesdir ]
then
    echo "Directory $sz_filesdir exists.\n"
else
    echo "Error: Directory $sz_filesdir does not exist.\n"
    exit 1
fi

# find number of files in the specified files directory
uc_num_files=`find $sz_filesdir -type f | wc -l`

# read content into a string and print it
sz_content=`grep -r $sz_searchstr *`


echo "The number of files are $uc_num_files and the number of matching lines are $sz_content"
