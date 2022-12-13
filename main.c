#include "stdio.h"
#include "stdlib.h"
#include "pthread.h" // библиотечка для POSIX Threads
#include "unistd.h"

static int num_of_threads = 3; // количество потоков. Код, конечно, плохой, но захардкодить константу рука не поднялась
static int itemsOnTable = 0; // количество предметов на столе в текущий момент времени
static int targetItems = 6; // контрольная сумма для предметов - сумма id потоков (1+2+3=6)
static char *items[] = { // массив для соответствия между id потока и его компонентом
        "tobacco",
        "paper",
        "matches"
    };
static FILE *input, *output; // указатели на потоки ввода-вывода...
static int isFile = 0; // флаг вывода в файл (эх...)
static char *filename; // название этого файла
pthread_mutex_t mutex; // мьютекс для блокировки

typedef struct smokersArgs { // структурка для передачи аргументов потокам-курильщикам
    int id; // id потока
    int check_s; // время проверки стола (в секундах)
    int smoke_s; // время курения (в секундах)
} smokersArgs_t;


typedef struct brokerArgs { // структурка для передачи аргументов потоку-посреднику
    int put_s; // время добавления компонентов (в секундах)
} brokerArgs_t;

void* check(void *args) { // функция работы потока-курильщика
    smokersArgs_t *arg = (smokersArgs_t*) args; // получаем аргументы
    while(1) { // непрерывно работаем
        sleep(arg->check_s); // ждём время для проверки стола
        pthread_mutex_lock(&mutex); // блокируем стол
            if (isFile == 1) { // для вывода в файл
                output = fopen(filename, "a");
            }
            if (itemsOnTable + arg->id != targetItems) { // если компоненты на столе нам не подходят
                fprintf(output, "Thread %d: skipped with %s\n", arg->id, items[arg->id - 1]);
            } else { // если не хватает именно нашего компонента
                fprintf(output, "Thread %d: smoke with %s\n", arg->id, items[arg->id - 1]);
                sleep(arg->smoke_s); // ждём время курения
                itemsOnTable = 0; // компоненты использованы
            }
            if (isFile == 1) { // для вывода в файл
                fclose(output);
            }
        pthread_mutex_unlock(&mutex); // разблокируем стол
    }
    return NULL;
}

void* put(void *args) { // функция работы потока-посредника
    brokerArgs_t *arg = (brokerArgs_t*) args; // получаем аргументы
    while(1) { // непрерывно работаем
        int first = rand() % num_of_threads; // генерируем компоненты
        int second = (first  + rand() % (num_of_threads - 1) + 1) % num_of_threads;
        pthread_mutex_lock(&mutex); // блокируем стол
            if (itemsOnTable == 0) { // если стол пуст
                itemsOnTable = first + second + 2; // кладём компоненты на стол
                if (isFile == 1) { // для вывода в файл
                    output = fopen(filename, "a");
                }
                fprintf(output, "Broker put %s and %s\n", items[first], items[second]);
                sleep(arg->put_s); // ждём
                if (isFile == 1) { // для вывода в файл
                    fclose(output);
                }
            }
        pthread_mutex_unlock(&mutex); // разблокируем стол
    }
    return NULL;
}

int main(int argc, char** argv) {
    int check_s, smoke_s, put_s; // входные данные
    pthread_t smokers_threads[num_of_threads]; // потоки-курильщики
    pthread_t broker_thread; // поток-посредник
    size_t i; // итератор
    smokersArgs_t args[num_of_threads]; // аргументы потоков-курильщиков
    brokerArgs_t args_b; // аргументы потока-посредника
    if (atoi(argv[1]) == 0) { // ввод из аргументов командной строки
        check_s = atoi(argv[2]);
        smoke_s = atoi(argv[3]);
        put_s = atoi(argv[4]);
        output = stdout; // вывод в консоль есть вывод в файл :)
    } else if (atoi(argv[1]) == 1) { // ввод из файла
        isFile = 1;
        input = fopen(argv[2], "r");
        filename = argv[3];
        fscanf(input, "%d", &check_s);
        fscanf(input, "%d", &smoke_s);
        fscanf(input, "%d", &put_s);
        fclose(input);
    } else { // ввод с помощью генератора случайных чисел
        srand(time(NULL));
        check_s = rand() % 20;
        smoke_s = rand() % 20;
        put_s = rand() % 20;
        printf("check_s: %d\n", check_s);
        printf("smoke_s: %d\n", smoke_s);
        printf("put_s: %d\n", put_s);
        output = stdout;
    }

    pthread_mutex_init(&mutex, NULL); // инициализируем мьютекс
        args_b.put_s = put_s; // инициализируем аргументы для потока-посредника
        pthread_create(&broker_thread, NULL, put, &args_b); // создаём его
        for(i = 0; i < num_of_threads; i++) { // для потоков-курильщиков
            args[i].id = i + 1; // инициализируем аргументы
            args[i].check_s = check_s;
            args[i].smoke_s = smoke_s;
            pthread_create(&smokers_threads[i], NULL, check, &args[i]); // создаём их
        }
        pthread_join(broker_thread, NULL); // чтобы программа выполнялась бесконечно (не лучший вариант реализации, но можно и так)
    pthread_mutex_destroy(&mutex); // деактивируем мьютекс
    return 0;
}