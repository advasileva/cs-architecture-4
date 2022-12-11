#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int counter = 0;
pthread_mutex_t mutex;


typedef struct someArgs_tag {
    int id;
    const char *msg;
    int out;
} someArgs_t;

void* exec(void *args) {
    someArgs_t *arg = (someArgs_t*) args;
        for(int t; t < 10000000; t++) {
            
        }
    pthread_mutex_lock(&mutex);
        if (counter < 2) {
            counter++;
            printf("Thread %d: add %s\n", arg->id, arg->msg);
        } else {
            counter = 0;
            printf("Thread %d: smoke\n", arg->id);
        }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char** argv) {
    int num_of_threads = 3;
    pthread_t threads[num_of_threads];
    size_t i, j;
    someArgs_t args[num_of_threads];
    const char *messages[] = {
        "tobacco",
        "paper",
        "matches"
    };
    for(j = 0; j < 10; j++) {
        for(i = 0; i < num_of_threads; i++) {
            args[i].id = i;
            args[i].msg = messages[i];
            pthread_create(&threads[i], NULL, exec, &args[i]);
        }
        for(i = 0; i < num_of_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
