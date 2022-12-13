#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

static int num_of_threads = 3;
static int itemsOnTable = 0;
static int targetItems = 6;
static char *items[] = {
        "tobacco",
        "paper",
        "matches"
    };
FILE *input, *output;
pthread_mutex_t mutex;

typedef struct smokersArgs {
    int id;
    int check_s;
    int smoke_s;
} smokersArgs_t;


typedef struct brokerArgs {
    int put_s;
} brokerArgs_t;

void* check(void *args) {
    smokersArgs_t *arg = (smokersArgs_t*) args;
    while(1) {
        sleep(arg->check_s);
        pthread_mutex_lock(&mutex);
            if (itemsOnTable + arg->id != targetItems) {
                fprintf(output, "Thread %d: skipped with %s\n", arg->id, items[arg->id - 1]);
            } else {
                fprintf(output, "Thread %d: smoke with %s\n", arg->id, items[arg->id - 1]);
                sleep(arg->smoke_s);
                itemsOnTable = 0;
            }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* put(void *args) {
    brokerArgs_t *arg = (brokerArgs_t*) args;
    while(1) {
        pthread_mutex_lock(&mutex);
            if (itemsOnTable == 0) {
                int first = rand() % num_of_threads;
                int second = (first  + rand() % (num_of_threads - 1) + 1) % num_of_threads;
                itemsOnTable = first + second + 2;
                fprintf(output, "Broker put %s and %s\n", items[first], items[second]);
                sleep(arg->put_s);
            }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char** argv) {
    int num_of_threads = 3;
    int check_s, smoke_s, put_s;
    pthread_t smokers_threads[num_of_threads];
    pthread_t broker_thread;
    size_t i, j;
    smokersArgs_t args[num_of_threads];
    brokerArgs_t args_b;
    if (atoi(argv[1]) == 0) {
        check_s = atoi(argv[2]);
        smoke_s = atoi(argv[3]);
        put_s = atoi(argv[4]);
        output = stdout;
    } else if (atoi(argv[1]) == 1) {
        input = fopen(argv[2], "r");
        output = fopen(argv[3], "a");
        fscanf(input, "%d", &check_s);
        fscanf(input, "%d", &smoke_s);
        fscanf(input, "%d", &put_s);
        printf("check_s: %d\n", check_s);
        printf("smoke_s: %d\n", smoke_s);
        printf("put_s: %d\n", put_s);
    } else {
        check_s = rand() % 20;
        smoke_s = rand() % 20;
        put_s = rand() % 20;
        printf("check_s: %d\n", check_s);
        printf("smoke_s: %d\n", smoke_s);
        printf("put_s: %d\n", put_s);
        output = stdout;
    }

    pthread_mutex_init(&mutex, NULL);
        args_b.put_s = put_s;
        pthread_create(&broker_thread, NULL, put, &args_b);
        for(i = 0; i < num_of_threads; i++) {
            args[i].id = i + 1;
            args[i].check_s = check_s;
            args[i].smoke_s = smoke_s;
            pthread_create(&smokers_threads[i], NULL, check, &args[i]);
        }
        pthread_join(broker_thread, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}