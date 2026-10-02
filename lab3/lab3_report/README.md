# Лабораторная работа №3

**Тема:** Параллельный поиск минимума и максимума в массиве.
Аргументы командной строки, fork, pipe, работа с файлами, exec, Makefile.

---

## Структура проекта

| Файл | Назначение |
|------|------------|
| find_max_min.h / find_max_min.c | Функция GetMinMax - поиск min/max на промежутке [start, end). |
| sequiential_min_max.c | Последовательная версия. |
| parallel_min_max.c | Параллельная версия (pipe и by_files). |
| run_sequential.c | Запуск sequential_min_max через fork + exec. |
| makefile | Сборка всех программ (make all). |
| .gitignore | Исключение бинарников и временных файлов. |

---

## Задание 1. GetMinMax

Функция перебирает элементы массива в полуинтервале [start, end)
и возвращает минимальный и максимальный элементы.

    void GetMinMax(const int *array, size_t start, size_t end,
                   int *min, int *max);

Сборка:

    make sequential_min_max
    ./sequential_min_max 100 0 100 42

---

## Задания 2-3. parallel_min_max

Массив делится на две части: [start, mid) и [mid, end).
Родитель обрабатывает первую, потомок - вторую.
Результат потомка передаётся родителю одним из двух способов:

* by_files - через временные файлы parent_result.txt и child_result.txt (задание 2).
* pipe (по умолчанию) - через pipe() и write/read структуры MinMax (задание 3).

Запуск:

    ./parallel_min_max 100 0 100 42            # pipe
    ./parallel_min_max 100 0 100 42 by_files   # files

Оба режима дают одинаковый результат с sequential_min_max.

---

## Задание 4. Makefile

Targets:

* all - собирает sequential_min_max, parallel_min_max, run_sequential.
* sequential_min_max, parallel_min_max, run_sequential - сборка отдельных программ.
* clean - удаляет бинарники и временные файлы.

    make all
    make clean

---

## Задание 5. run_sequential

Программа делает fork, в дочернем процессе через execv
запускает ./sequential_min_max, родитель ждёт завершения через waitpid
и печатает код возврата.

    ./run_sequential 100 0 100 42

---

## Результаты запусков

Все три программы на одинаковых входных данных (n=100, start=0,
end=100, seed=42) дают одинаковый результат. Точные значения зависят
от реализации rand() на платформе - важно, что все три версии
совпадают между собой.

В output.txt полный лог.

---

## Как воспроизвести

    make all
    ./sequential_min_max 100 0 100 42
    ./parallel_min_max   100 0 100 42
    ./parallel_min_max   100 0 100 42 by_files
    ./run_sequential     100 0 100 42
