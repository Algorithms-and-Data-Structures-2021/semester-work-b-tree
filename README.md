# Сбалансированное дерево поиска B-Tree

[![CMake](https://github.com/Algorithms-and-Data-Structures-2021/semester-work-b-tree/actions/workflows/cmake.yml/badge.svg)](https://github.com/Algorithms-and-Data-Structures-2021/semester-work-b-tree/actions/workflows/cmake.yml)

- В проекте реализуется структура данных B-Tree
- Структура данных представляет из себя сбалансированное дерево поиска
- B-Tree широко используется в базах данных и файловых системах (например: NTFS, HFS+, ext4)
- Структура данных поддерживает такие операции, как: поиск, вставка, удаление, Sequential access, Initial construction
- Сложность операций (в худшем случае): Space - `O(n)`, поиск, вставка, удаление - `O(log n)`

## Команда "Название команды"

_Заполните таблицу с указанием вклада каждого из участников в проект._

**Примечание**. Преподаватель может определить вклад любого из участников команды по истории коммитов.

| Фамилия Имя   | Вклад (%) | Прозвище              |
| :---          |   ---:    |  ---:                 |
| Андрей Неманов   |      33,(3)   |                 |
| Рустем Давлетшин   |      33,(3)   |   |
| Кямаль Хайдаров   |       33,(3)  |    камаль       |

**Девиз команды**
> _Наши цели не ясны. Задачи не определены. Забиваем, товарищи!_

## Структура проекта

Проект состоит из следующих частей:

- [`src`](src)/[`include`](include) - реализация структуры данных (исходный код и заголовочные файлы);
- [`benchmark`](benchmark) - контрольные тесты производительности структуры данных (операции добавления, удаления,
  поиска и пр.);
- [`examples`](examples) - примеры работы со структурой данных;
- [`dataset`](dataset) - наборы данных для запуска контрольных тестов и их генерация;

## Требования (Prerequisites)

Рекомендуемые требования:

1. С++ компилятор c поддержкой стандарта C++17 (например, _GNU GCC 8.1.x_ и выше).
2. Система автоматизации сборки _CMake_ (версия _3.12.x_ и выше).
3. Интерпретатор _Python_ (версия _3.7.x_ и выше).
4. Рекомендуемый объем оперативной памяти - не менее 4 ГБ.
5. Свободное дисковое пространство объемом ~ 3 ГБ (набор данных для контрольных тестов).

## Сборка и запуск



### Windows

#### Сборка проекта


Склонируйте проект к себе на устройство через [Git for Windows](https://gitforwindows.org/) (либо используйте
возможности IDE):

```shell
git clone https://github.com/Algorithms-and-Data-Structures-2021/semester-work-b-tree.git
```

Для ручной сборки проекта в терминале введите:

```shell
# переход в папку с проектом
cd C:\Users\username\asd-projects\semester-work-b-tree

# создание папки для файлов сборки (чтобы не засорять папку с проектом) 
mkdir -p build && cd build 

# сборка проекта
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --config RelWithDebInfo --build . 
```

#### Генерация тестовых данных

Генерация тестовых данных проходит с помощью файла generator.py, в нём указывается количество значений и путь до файла,
куда тестовые данные сохраняются. Файлы хранятся в формате .csv

Генерация тестового набора данных в
формате [comma-seperated values (CSV)](https://en.wikipedia.org/wiki/Comma-separated_values):

```shell

# запуск Python-скрипта
python generator.py [args ...]
```

Тестовые данные представлены в CSV формате (см.
[`dataset/data/dataset-example.csv`](dataset/data/dataset-example.csv)):

```csv
id, full_name
0, "Ramil Safin"
1, "Bulat Abbyasov"
...
```

**Примечание**. Для удобства запуска контрольных тестов рекомендуется организовывать данные в директориях, например:

```shell
dataset/data/
  add/
    1/
      100.csv
      ...
      5000000.csv
    2/ ...
    3/ ...
    ...
    10/ ...
  search/
    1/
      100.csv
      ...
      5000000.csv
    ...
    10/ ...
  ...
```

По названию директории `/dataset/data/add` можно понять, что здесь хранятся наборы данных для контрольных тестов по
**добавлению** элементов в структуру данных. Названия файлов `100.csv`. `5000000.csv` и т.д. хранят информацию о размере
набора данных (т.е. количество элементов).

#### Контрольные тесты (benchmarks)


Для запуска контрольных тестов запустите main.cpp, затем введите абсолютный путь к файлу с тестовыми данными, абсолютный
путь к файлу, в который хотите сохранить результат, и количество повторов запуска. Тесты замеряют время работы трёх
основных операций B-дерева (insert, search, remove)

В файле с результатом замеры времени будут записаны следующим образом:

insert search remove
100   100     100
100   100     100
100   100     100
insert: 100
search: 100
remove: 100


Наборы данных находятся в папке семестровой
работы на [Google Drive](https://drive.google.com/drive/folders/17-qridbMXFnz3E-6UjOj0WD1H0jWtpz3?usp=sharing).

## Источники

https://en.wikipedia.org/wiki/B-tree

https://habr.com/ru/post/337594/

https://habr.com/ru/post/114154/