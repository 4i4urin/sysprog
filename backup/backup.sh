#!/bin/bash

# Check if arguments are not empty
if [ $# -eq 0 ]; then
    echo "Missing arguments"
    echo "Requireв -f [config_file_name]"
    exit 0
fi


# Parse options using getopts
while getopts ":f:" OPTION; do
    case $OPTION in
        f)
            config_path=$OPTARG
            ;;
        \?)
            echo "Invalid option: -$OPTARG"
            exit 0
            ;;
        :)
            echo "Option -$OPTARG requires an argument"
            exit 0
            ;;
    esac
done

shift $((OPTIND-1)) # Shift script arguments


find_res=$(find . -name $config_path)

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
lines_count=$(wc -l < $find_res)
read storage_dir <<< $(tail -n $(($lines_count-${#path_backup_data[@]}-1)) $find_res)

if [[ -z "$storage_dir" ]]; then
    echo "Bad settings file. No storage_dir"
    exit 200
fi

# reading max number of backups in storage_dir
read max_backups <<< $(tail -n $(($lines_count-${#path_backup_data[@]}-3)) $find_res)

if [[ $max_backups -eq 0 ]]; then
    echo "Bad settings file. No max number of backups"
    exit 200
fi


echo $storage_dir


#create dir if require
if [[ ! -e $storage_dir ]]; then
    mkdir $storage_dir
fi


# delete backup file if require
backup_count=$(ls $storage_dir | grep "backup-" | wc -l)
if [[ "$backup_count" -ge "$max_backups" ]]; then
    delete_file=$(ls $storage_dir | grep "backup-" | head -n1)
    delete_file="${storage_dir}${delete_file}"
    echo "Delete file $delete_file"
    rm $delete_file
fi


#create backup file name
file_name="backup"
file_name="${file_name}-$(date +'%Y-%m-%d-%H-%M').tar.gz"


#build command for archive
cmd_archive_data="tar -zcvf $storage_dir"
cmd_archive_data="${cmd_archive_data}${file_name}"

for path in "${path_backup_data[@]}"; do
    if [[ ! -e $path ]]; then
        echo "$path doesn't exist"
    fi
    cmd_archive_data+=" $path"
done

cmd_archive_data="$(${cmd_archive_data})"


#execute archive command
echo "$cmd_archive_data"
