#!/bin/bash

# check for the number of arguments
if [ $# -ne 2 ]
then 
    echo "Error: The number of arguments is incorrect."
    exit 1
fi

# load in the parameters
sz_write_file=$1
sz_write_str=$2

# create a new file if it doesn't exist
mkdir -p "$(dirname "$sz_write_file")" && touch "$sz_write_file"

# check whether the file could be created or already legally exist
if [ $? -ne 0 ]
then
    echo "Error: the file could not be created.\n"
    exit 1
fi

touch $sz_write_file

# write content to the file specified, overwriting any contents
echo "$sz_write_str" > $sz_write_file


