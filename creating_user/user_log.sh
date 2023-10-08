#!/bin/bash

# Путь к файлу wtmp
wtmp_file="/var/log/wtmp"

# Проверка наличия команды utmpdump
if ! command -v utmpdump &> /dev/null; then
echo "Команда utmpdump не найдена. Убедитесь, что она установлена."
exit 1
fi
# Создаем временный файл для сортировки
tmpfile=$(mktemp)
trap 'rm -f "$tmpfile"' EXIT

# Вывод времени входа и выхода для всех пользователей и сортировка по времени
info=$(utmpdump "$wtmp_file" | awk '$1 == "[7]" || $1 == "[8]" {print $1,$2, $4, $5, $12,$13}' | sort -k2)

echo "$info" > "$tmpfile"

# Фильтрация строк с одинаковыми вторыми аргументами (PID)
all_users_inf=0
all_users_inf=$(awk 'NR == 1 { prev_pid = $2; prev_line = $0; next } $2 == prev_pid { print $3 $4 ";" $5 ";" prev_field_13 ;  } { prev_pid = $2; prev_line = $0; prev_field_13 = $6 }' "$tmpfile" | sort -t ";" -k2)

if [ $# == 0 ]; then
    user_inf=$(echo "$all_users_inf" | tr -d '[]')
else
    user_inf=$(echo "$all_users_inf" | tr -d '[]' | grep "$1")
fi



array=()
readarray -t array <<< "$user_inf"

i=0
len=${#array[@]}
symbl=";"

while [[ $i -lt $len ]];
do
    string=${array[$i]}
    name="$(cut -d';' -f1 <<< "$string" )"
    login="$(cut -d';' -f2 <<< "$string" )"
    logout="$(cut -d';' -f3 <<< "$string")"
    if [ -z "$login" ]; then
        let i++
        continue
    fi
    if [ -z "$logout" ]; then
        let i++
        continue
    fi
    login=$(date -d $login '+%F %H:%M:%S')
    logout=$(date -d $logout '+%F %H:%M:%S')
    printf "$name\t$login\t$logout\n"
    let i++
done

