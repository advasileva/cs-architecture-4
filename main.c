#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int num_of_threads = 3;
static int itemsOnTable = 0;
static int targetItems = 6;
static char *items[] = {
        "tobacco",
        "paper",
        "matches"
    };
pthread_mutex_t mutex;

typedef struct smokersArgs {
    int id;
} smokersArgs_t;

void* check(void *args) {
    smokersArgs_t *arg = (smokersArgs_t*) args;
    while(1) {
        for(int t = 0; t < 1000000000; t++) {
        }
        pthread_mutex_lock(&mutex);
            if (itemsOnTable + arg->id != targetItems) {
                printf("Thread %d: skipped with %s\n", arg->id, items[arg->id - 1]);
            } else {
                printf("Thread %d: smoke with %s\n", arg->id, items[arg->id - 1]);
                itemsOnTable = 0;
            }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* put(void *args) {
    while(1) {
        for(int t = 0; t < 1000000000; t++) {
        }
        pthread_mutex_lock(&mutex);
            if (itemsOnTable == 0) {
                int first = rand() % num_of_threads;
                int second = (first  + rand() % (num_of_threads - 1) + 1) % num_of_threads;
                itemsOnTable = first + second + 2;
                printf("Broker put %s and %s\n", items[first], items[second]);
            }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char** argv) {
    int num_of_threads = 3;
    pthread_t smokers_threads[num_of_threads];
    pthread_t broker_thread;
    size_t i, j;
    smokersArgs_t args[num_of_threads];

    pthread_mutex_init(&mutex, NULL);
        pthread_create(&broker_thread, NULL, put, NULL);
        for(i = 0; i < num_of_threads; i++) {
            args[i].id = i + 1;
            pthread_create(&smokers_threads[i], NULL, check, &args[i]);
        }

        pthread_join(broker_thread, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
