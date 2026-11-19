#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_FILOSOFOS 5
#define VECES_COMER 3

pthread_mutex_t tenedores[NUM_FILOSOFOS];
pthread_mutex_t mutex_pantalla = PTHREAD_MUTEX_INITIALIZER;

void imprimir(int id, const char *accion) {
    pthread_mutex_lock(&mutex_pantalla);
    printf("Filosofo %d %s\n", id, accion);
    pthread_mutex_unlock(&mutex_pantalla);
}

void *filosofo(void *arg) {
    int id = *(int *)arg;
    int izquierdo = id;
    int derecho = (id + 1) % NUM_FILOSOFOS;
    int primero, segundo;
    int i;

    /* El ultimo filosofo toma los tenedores en orden inverso para evitar interbloqueo */
    if (id == NUM_FILOSOFOS - 1) {
        primero = derecho;
        segundo = izquierdo;
    } else {
        primero = izquierdo;
        segundo = derecho;
    }

    for (i = 0; i < VECES_COMER; i++) {
        imprimir(id, "esta pensando");
        usleep(100000 + rand() % 200000);

        imprimir(id, "tiene hambre");
        pthread_mutex_lock(&tenedores[primero]);
        imprimir(id, "tomo el primer tenedor");
        pthread_mutex_lock(&tenedores[segundo]);
        imprimir(id, "tomo el segundo tenedor");

        imprimir(id, "esta comiendo");
        usleep(100000 + rand() % 200000);

        pthread_mutex_unlock(&tenedores[segundo]);
        pthread_mutex_unlock(&tenedores[primero]);
        imprimir(id, "solto los tenedores");
    }

    imprimir(id, "termino de cenar");
    return NULL;
}

int main(void) {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];
    int i;

    srand(42);

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
    }

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, filosofo, &ids[i]);
    }

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    printf("Todos los filosofos terminaron de cenar\n");
    return 0;
}
