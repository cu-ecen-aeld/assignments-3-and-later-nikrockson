#!/bin/sh

# Number of arguments are provided
if [ $# -lt 2 ]; 
then
  echo "Error: Two arguments required."
  exit 1
fi

filesdir=$1 # Directory to search in (First argument)
searchstr=$2 # Search string (Second argument)

# Check if the provided directory exists
if [ ! -d "$filesdir" ]; 
then
  echo "Error: '$filesdir' is not a valid directory."
  exit 1
fi

# Counting the number of files
file_count=$(find "$filesdir" -type f | wc -l)

# Counting the number of matching lines in those files
match_count=$(grep -r "$searchstr" "$filesdir" | wc -l)

# Output the result
echo "The number of files are $file_count and the number of matching lines are $match_count"

# Exit with success status
exit 0