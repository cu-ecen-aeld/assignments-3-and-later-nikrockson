#!/bin/bash

#1 Check if the correct number of arguments is provided
if [ "$#" -ne 2 ];
then
  echo "Error: Two arguments required."
  echo "Usage: $0 <writefile> <writestr>"
  exit 1
fi

writefile=$1
writestr=$2

#2 Directory to write the file
write_dir=$(dirname "$writefile")

#3 Check if the directory exists, if not create it
if [ ! -d "$write_dir" ];
then
  mkdir -p "$write_dir"
fi

#4 Write the string to the file
echo "$writestr" > "$writefile"

#5 Check if the write operation was successful
if [ $? -eq 0 ];
then
  echo "Successfully wrote to '$writefile'."
else
  echo "Error: Could not write to '$writefile'."
  exit 1
fi

#6 Exit with success status
exit 0
# End of script