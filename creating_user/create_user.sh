#!/bin/bash


function input_error()
{
    echo "INVALID INPUT"
    help_message
    exit 100
}


function help_message()
{
    printf "\nScript for creating user"
    echo "Otions:"
    printf "  -p set password for user\n"
    printf "  -r set root privileges\n"
}


function get_password()
{
    hash_seed=$(openssl rand -base64 10)
    check_password=("0" "1")

    read -p "Enter password: " -s check_password[0]
    check_password[0]=$(openssl passwd -6 -salt $hash_seed ${check_password[0]})

    read -p "Repeat password: " -s check_password[1]
    check_password[1]=$(openssl passwd -6 -salt $hash_seed ${check_password[1]})

    if [ "${check_password[0]}" != "${check_password[1]}" ]; then
        echo "Passwords are not equal"
        exit 300
    fi
    echo "${check_password[0]}"
}

password_flag=0
root_flag=0


case $# in
    1)  ;;
    2)
        if [ $2 == "-p" ]; then
            password_flag=1
        elif [ $2 == "-r" ]; then
            root_flag=1
        else
            input_error
        fi ;;

    3) if [ $2 == "-p" ] && [ $3 == "-r" ] && [ $2 != $3 ]; then
            password_flag=1
            root_flag=1
        elif [ $2 == "-r" ] && [ $3 == "-p" ] && [ $2 != $3 ]; then
            password_flag=1
            root_flag=1
        else
            input_error
        fi ;;
    *) input_error
esac

user_name=$1

if [ $password_flag == 1 ]; then
    password=$(get_password)
else
    password="*"
fi

if [ $root_flag == 1 ]; then
    user_id=0
    group_id=0
else
    user_id=$(($(awk -F ":" '$3 > 1000 { uid = $3 } END { print uid }' /etc/passwd)+1))
    group_id=$(($(awk -F ":" '$3 > 1000 { uid = $3 } END { print uid }' /etc/passwd)+1))
fi


date=$((`date +%s` / (3600 * 24)))

echo "$user_name:x:$user_id:$group_id::/home/$user_name:/bin/bash" >> /etc/passwd
echo "$user_name:$password:$date:0:99999:7:::" >> /etc/shadow

mkdir "/home/$user_name"
chmod 755 "/home/$user_name"
chown "$user_name" "/home/$user_name"

if [ $password_flag != 1 ]; then
    echo "$user_name:U6aMy0wojraho" | chpasswd -e
fi

echo "$user_name:x:$group_id:" >> /etc/group

printf "\n"
