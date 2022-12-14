# Архитектура вычислительных систем ИДЗ 4

0. [**Задание**](#задание)
0. [**Отчёт**](#отчёт)
    + [*4 балла*](#4-балла)
    + [*5 баллов*](#5-баллов)
    + [*6 баллов*](#6-баллов)
    + [*7 баллов*](#7-баллов)
    + [*8 баллов*](#8-баллов)

## Задание

*Изучить работу с потоками. Научиться разбивать задачу на части, для последующего их выполнения различными потоками.*

*Вариант 7. Задача о курильщиках. Есть три процесса-курильщика и один процесспосредник. Курильщик непрерывно скручивает сигареты и курит их. Чтобы скрутить сигарету, нужны табак, бумага и спички. У одного процессакурильщика есть табак, у второго – бумага, а у третьего – спички. Посредник кладет на стол по два разных случайных компонента. Тот процесс-курильщик, у которого есть третий компонент, забирает компоненты со стола, скручивает сигарету и курит. Посредник дожидается, пока курильщик закончит, затем процесс повторяется. Создать многопоточное приложение, моделирующее поведение курильщиков и посредника. При решении задачи использовать семафоры.*

## Отчёт

**Делала на оценку 8 баллов**

### 4 балла

+ *Приведено условие задачи.*

    Условие задачи приведено выше

+ *Описана модель параллельных вычислений, используемая при разработке многопоточной программы.*

    Для данной задачи подойдёт модель "Производители и потребители", потому что процесс-посредник как раз является "производителем" предметов на столе, а процессы-курильщики их "потребляют"

+ *Описаны входные данные программы, включающие вариативные диапазоны, возможные при многократных запусках.*

    Вариант ввода данных выбирается с помощью первого аргумента командной строки:
    + `0` - с помощью аргументов командной строки, далее 3 аргумента, соответствующие входным данным
    + `1`- с помощью файла, далее названия входного и выходного файлов, во входном файле находятся входные данные
    + `2`- с помощью генератора случайных чисел, больше ничего вводить не надо, данные сгенерируются сами

    Входные данные программы:
    + `check_s` - время проверки предметов на столе курильщиком (в секундах)
    + `smoke_s` - время курения (в секундах)
    + `put_s` - время добавления на стол предметов брокером (в секундах)
    
    Как именно запускается исполняемый файл, можно посмотреть в bash-скриптах в директории `scripts/`

+ *Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта синхропримитивов.*

    Реализованное приложение находится в `main.c`. Использованы мьютексы, так как они по определению является семафорами, которые требуются в задании

    Оно уже скомпилировано и запускается с помощью следующих команд:
    
    +   для ввода с помощью аргументов командой строки:
        ```bash
        make run.cl
        ```
    
    +   для ввода с помощью файла:
        ```bash
        make run.file
        ```
    
    +   для ввода с помощью генератора случайных чисел:
        ```bash
        make run.rand
        ```
    
    По просьбе лектора приложение работает бесконечно, для завершения работы - `Ctrl+C`
    
    Пример работы приложения:
    ```bash
    $ make run.cl
    bash ./scripts/run-cl.sh
    Broker put paper and matches
    Thread 2: skipped with paper
    Thread 3: skipped with matches
    Thread 1: smoke with tobacco
    Broker put tobacco and matches
    Thread 2: smoke with paper
    Thread 3: skipped with matches
    Thread 1: skipped with tobacco
    Broker put matches and paper
    Thread 3: skipped with matches
    Thread 1: smoke with tobacco
    Thread 2: skipped with paper
    Broker put paper and matches
    Thread 3: skipped with matches
    Thread 1: smoke with tobacco
    Thread 2: skipped with paper
    ...
    ```

+ *Ввод данных в приложение реализован с консоли.*

    Реализовано 3 варианта ввода данных

+ *Результаты работы приведены в отчете*

    Отчёт создан

### 5 баллов

+ *В программу добавлены комментарии, поясняющие выполняемые действия и описание используемых переменных.*

    Комментарии добавлены в `main.c`

+ *В отчете должен быть приведен сценарий, описывающий одновременное поведение представленных в условии задания сущностей в терминах предметной области. То есть, описано поведение объектов разрабатываемой программы как взаимодействующих субъектов, а не то, как это будет реализовано в программе.*

    Пример сценария: пока курильщик курит, посредник выбирает, какие компоненты положить на стол (случайный выбор тоже надо сделать)

### 6 баллов

+ *В отчете подробно описан обобщенный алгоритм, используемый при реализации программы исходного словесного сценария. В котором показано, как на программу отображается каждый из субъектов предметной области.*

    TL;DR: счётчик (стол), один поток (посредник) его увеличивает, три других (курильщики) уменьшают, доступ к нему с помощью мьютекса
    
    Какие у нас есть субъекты предметной области?
    + Стол -> `int itemsOnTable`
    + Процесс-посредник -> `pthread_t broker_thread`
    + Процессы-курильщики -> `pthread_t smokers_threads[num_of_threads]`
    
    После ввода данных всеми возможными способами создаются указанные потоки и каждый из них начинает бесконечно выполнять свою задачу:
    + Процесс-посредник -> `put` - добавление компонентов на стол
    + Процессы-курильщики -> `check` - просмотр и использование компонентов на столе
    
    Для доступа к столу процессы используют мьютекс
    
    Процесс-посредник выбирает следующий компонент, пока стол занят другим процессом. Когда стол свободен и на нём ничего нет (процесс-курильщик докурил), этот процесс добавляет два случайных компонента
    
    Каждый из процессов-курильщиков блокирует стол и оценивает компоненты на нём. Если с его собственным компонентом получается необходимый набор, то он курит и убирает все компоненты со стола

+ *Реализован ввод данных из командной строки.*

    Ввод данных из командной строки описан в оценке на 4 балла

+ *Результаты изменений отражены в отчете.*

    Отчёт обновлён
    

### 7 баллов

+ *В программу добавлены ввод данных из файла и вывод результатов в файл.*

    Ввод данных из файла:
    ```c
    isFile = 1;
    input = fopen(argv[2], "r");
    filename = argv[3];
    fscanf(input, "%d", &check_s);
    fscanf(input, "%d", &smoke_s);
    fscanf(input, "%d", &put_s);
    fclose(input);
    ```

+ *Приведены входные и выходные файлы с различными результатами выполнения программы.*

    Программа запускалась с помощью команды:
    ```bash
    make run.file
    ```
    
    Первый запуск:
    ```bash
    Broker put paper and matches
    Thread 3: skipped with matches
    Thread 2: skipped with paper
    Thread 1: smoke with tobacco
    Broker put paper and tobacco
    Thread 3: smoke with matches
    Thread 2: skipped with paper
    Thread 1: skipped with tobacco
    Broker put matches and paper
    Thread 1: smoke with tobacco
    Thread 2: skipped with paper
    Thread 3: skipped with matches
    Broker put tobacco and matches
    Thread 2: smoke with paper
    Thread 3: skipped with matches
    Thread 1: skipped with tobacco
    ...
    ```
    
    Второй запуск:
    ```bash
    Broker put paper and matches
    Thread 2: skipped with paper
    Thread 3: skipped with matches
    Thread 1: smoke with tobacco
    Broker put paper and matches
    Thread 2: skipped with paper
    Thread 3: skipped with matches
    Thread 1: smoke with tobacco
    Broker put tobacco and matches
    Thread 3: skipped with matches
    Thread 2: smoke with paper
    Thread 1: skipped with tobacco
    Broker put paper and matches
    Thread 3: skipped with matches
    Thread 2: skipped with paper
    Thread 1: smoke with tobacco
    ...
    ```

+ *Результаты работы программы должны выводиться на экран и записываться в файл.*

    Для вывода на экран можно запустить с помощью:
    ```bash
    make run.cl
    ```
    
    Для вывод в файл:
    ```bash
    make run.file
    ```
    
    Дублирование вывода в рамках одного запуска автору показалось нецелесообразным

+ *Ввод данных из командной строки расширен с учетом введенных изменений.*

    Итоговый формат ввода описан в оценке на 4 балла

+ *Результаты изменений отражены в отчете.*

    Отчёт обновлён
    

### 8 баллов

+ *В программу добавлена генерация случайных данных в допустимых диапазонах.*

    Генерация случайных данных:
    ```c
    srand(time(NULL));
    check_s = rand() % 20;
    smoke_s = rand() % 20;
    put_s = rand() % 20;
    printf("check_s: %d\n", check_s);
    printf("smoke_s: %d\n", smoke_s);
    printf("put_s: %d\n", put_s);
    output = stdout;
    ```

+ *Приведены входные и выходные файлы с различными результатами выполнения программы.*

    Программа запускалась с помощью команды:
    ```bash
    make run.rand
    ```
    
    Первый запуск:
    ```bash
    check_s: 3
    smoke_s: 6
    put_s: 17
    Broker put paper and tobacco
    Thread 3: smoke with matches
    Thread 2: skipped with paper
    Thread 1: skipped with tobacco
    Broker put matches and paper
    Thread 1: smoke with tobacco
    Thread 2: skipped with paper
    Thread 3: skipped with matches
    Broker put paper and matches
    Thread 3: skipped with matches
    Thread 2: skipped with paper
    Thread 1: smoke with tobacco
    Broker put tobacco and matches
    Thread 2: smoke with paper
    Thread 3: skipped with matches
    Thread 1: skipped with tobacco
    ...
    ```
    
    Второй запуск:
    ```bash
    check_s: 18
    smoke_s: 11
    put_s: 7
    Broker put matches and tobacco
    Thread 3: skipped with matches
    Thread 2: smoke with paper
    Thread 1: skipped with tobacco
    Broker put paper and tobacco
    Thread 3: smoke with matches
    Broker put tobacco and paper
    Thread 2: skipped with paper
    Thread 1: skipped with tobacco
    Thread 3: smoke with matches
    Broker put matches and tobacco
    Thread 2: smoke with paper
    Thread 1: skipped with tobacco
    ...
    ```

+ *Ввод данных из командной строки расширен с учетом введенных изменений.*

    Итоговый формат ввода описан в оценке на 4 балла

+ *Результаты изменений отражены в отчете.*

    Отчёт обновлён
