#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define CREATE_PROCESS 5

sem_t semaphore;

void* routine(void* args) {
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from process %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[CREATE_PROCESS];
    sem_init(&semaphore, 1, 1);
    int i;
    for (i = 1; i < CREATE_PROCESS; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create process");
        }
    }

    for (i = 1; i < CREATE_PROCESS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join process");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}
