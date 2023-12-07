#!/bin/bash

echo "Hello"

dir=$1 # store directory from script parameter in variable
option=$2 # store the option (week/month) from script parameter

if [ ! -d "$dir" ]; then
    echo "Please provide a valid directory"
    exit 1
fi

# Function to calculate week number from date
get_week_number() {
    date -d "$1" +%V
}

# Function to calculate month from date
get_month() {
    date -d "$1" +%m
}

# run through dir
for file in "$dir"/*
do
    if [ -f "$file" ]; then
        # get creation date, for example:
        creationdate=$(stat -c %y "$file" | cut -d ' ' -f1)

        if [ "$option" == "week" ]; then
            week_number=$(get_week_number "$creationdate")
            destination="$dir/week_$week_number"
        elif [ "$option" == "month" ]; then
            month=$(get_month "$creationdate")
            destination="$dir/month_$month"
        else
            echo "Invalid option. Please choose 'week' or 'month'."
            exit 1
        fi

        # make directory if not exists
        mkdir -p "$destination"

        # copy file to destination
        cp "$file" "$destination"

        # get md5sum of original and copied file
        originalhash=$(md5sum "$file" | cut -d " " -f1)
        copiedhash=$(md5sum "$destination/$(basename "$file")" | cut -d " " -f1)

        # check if md5sum matches
        if [ "$originalhash" == "$copiedhash" ]; then
            # remove original file
            rm "$file"
            echo "File moved successfully."
        else
            echo "Error in copying file. MD5sum does not match."
        fi
    fi
done
