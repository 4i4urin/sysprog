#!/bin/bash

find_res=$(find . -name $1)

if [[ ! -e $find_res ]]; then
	echo "No such file"
    exit 100
fi

if [[ "$find_res" != *".csv" ]]; then
    echo "Require .csv file"
    exit 100
fi

# read files path to backup them
path_backup_data=()
i=0
while read line; do [ -z "$line" ] && break;
    path_backup_data[$i]=$line
    let i++
done <<< $(cat $find_res)

if [ ${#path_backup_data[@]} == 0 ]; then
    echo "Bad settings file. Nothing to backup"
    exit 200
fi

# reading storage dir for backups
read storage_dir <<< $(tail -n $((${#path_backup_data[@]}-2)) $find_res)

if [[ -z "$storage_dir" ]]; then
    echo "Bad settings file. No storage_dir"
    exit 200
fi

# reading max number of backups in storage_dir
read max_backups <<< $(tail -n $((${#path_backup_data[@]}-4)) $find_res)

if [[ $max_backups -eq 0 ]]; then
    echo "Bad settings file. No max number of backups"
    exit 200
fi

echo $storage_dir
echo $max_backups
echo "${path_backup_data[@]}"

