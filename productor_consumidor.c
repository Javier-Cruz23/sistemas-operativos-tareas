#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM_BUFFER 5
#define NUM_PRODUCTORES 2
#define NUM_CONSUMIDORES 2
#define ITEMS_POR_HILO 10

int buffer[TAM_BUFFER];
int entrada = 0;
int salida = 0;

sem_t vacios;   /* espacios libres en el buffer */
sem_t llenos;   /* espacios ocupados en el buffer */
pthread_mutex_t mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

void *productor(void *arg) {
    int id = *(int *)arg;
    int i, dato;

    for (i = 0; i < ITEMS_POR_HILO; i++) {
        dato = id * 100 + i;
        usleep(50000 + rand() % 100000);

        sem_wait(&vacios);
        pthread_mutex_lock(&mutex_buffer);

        buffer[entrada] = dato;
        printf("Productor %d produjo %d en posicion %d\n", id, dato, entrada);
        entrada = (entrada + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&llenos);
    }
    return NULL;
}

void *consumidor(void *arg) {
    int id = *(int *)arg;
    int i, dato;

    for (i = 0; i < ITEMS_POR_HILO; i++) {
        sem_wait(&llenos);
        pthread_mutex_lock(&mutex_buffer);

        dato = buffer[salida];
        printf("Consumidor %d consumio %d de posicion %d\n", id, dato, salida);
        salida = (salida + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&vacios);

        usleep(80000 + rand() % 150000);
    }
    return NULL;
}

int main(void) {
    pthread_t productores[NUM_PRODUCTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];
    int ids_prod[NUM_PRODUCTORES];
    int ids_cons[NUM_CONSUMIDORES];
    int i;

    srand(7);
    sem_init(&vacios, 0, TAM_BUFFER);
    sem_init(&llenos, 0, 0);

    for (i = 0; i < NUM_PRODUCTORES; i++) {
        ids_prod[i] = i + 1;
        pthread_create(&productores[i], NULL, productor, &ids_prod[i]);
    }
    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        ids_cons[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, &ids_cons[i]);
    }

    for (i = 0; i < NUM_PRODUCTORES; i++) {
        pthread_join(productores[i], NULL);
    }
    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    sem_destroy(&vacios);
    sem_destroy(&llenos);
    pthread_mutex_destroy(&mutex_buffer);

    printf("Fin: se produjeron y consumieron %d elementos\n",
           NUM_PRODUCTORES * ITEMS_POR_HILO);
    return 0;
}
