#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 20            // cantidad de números a producir
#define BUFFER_SIZE 5   // tamaño del buffer

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < N; i++) {
        int item = i + id * 100; // número producido
        // espera si el buffer está lleno
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex); 
        }

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("- Productor #%d produjo: %d (buffer=%d)\n", id, item, count);

        pthread_cond_signal(&not_empty); // despierta a consumidores
        pthread_mutex_unlock(&mutex);

        usleep(100000); // simula trabajo
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < N; i++) {
        // espera si el buffer está vacío
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex); 
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("° Consumidor #%d consumió: %d (buffer=%d)\n", id, item, count);

        pthread_cond_signal(&not_full); // despierta a productores
        pthread_mutex_unlock(&mutex);

        usleep(150000); // simula procesamiento
    }
    return NULL;
}

int main() {
    pthread_t prod[2], cons[2];
    int ids[2] = {1, 2};

    // hilos productores y consumidores
    pthread_create(&prod[0], NULL, producer, &ids[0]);
    pthread_create(&prod[1], NULL, producer, &ids[1]);
    pthread_create(&cons[0], NULL, consumer, &ids[0]);
    pthread_create(&cons[1], NULL, consumer, &ids[1]);

    for (int i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    return 0;
}
