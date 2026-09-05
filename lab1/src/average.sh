#!/bin/bash

# Проверка, что есть аргументы
if [ $# -eq 0 ]; then
    echo "Usage: $0 number1 number2 ..."
    exit 1
fi

# Сумма и количество аргументов
sum=0
count=0

for num in "$@"; do
    # Проверка, что аргумент - число
    if [[ "$num" =~ ^-?[0-9]+$ ]] || [[ "$num" =~ ^-?[0-9]+\.[0-9]+$ ]]; then
        sum=$(echo "$sum + $num" | bc -l)
        count=$((count + 1))
    else
        echo "Warning: '$num' is not a number, skipping"
    fi
done

# Вывод результата
if [ $count -eq 0 ]; then
    echo "No valid numbers provided"
else
    average=$(echo "$sum / $count" | bc -l)
    echo "Count: $count"
    echo "Average: $average"
fi
