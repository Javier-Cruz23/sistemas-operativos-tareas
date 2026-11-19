#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LIMITE 10000
#define INICIO_CRITICA 4000
#define FIN_CRITICA 6000

sem_t semaforo;

void *contar(void *arg) {
    int id = *(int *)arg;
    int i;

    for (i = 1; i <= LIMITE; i++) {
        if (i == INICIO_CRITICA) {
            sem_wait(&semaforo);   /* entra a la seccion critica */
        }

        printf("Hilo %d: %d\n", id, i);

        if (i == FIN_CRITICA) {
            sem_post(&semaforo);   /* sale de la seccion critica */
        }
    }
    return NULL;
}

int main(void) {
    pthread_t hilo1, hilo2;
    int id1 = 1, id2 = 2;

    setvbuf(stdout, NULL, _IOLBF, 0);
    sem_init(&semaforo, 0, 1);

    pthread_create(&hilo1, NULL, contar, &id1);
    pthread_create(&hilo2, NULL, contar, &id2);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    sem_destroy(&semaforo);
    return 0;
}
