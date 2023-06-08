/*
 Realizzare un programma che, mediante l'ausilio della libreria pthread,
 decomponga la somma degli elementi di un vettore di interi di dimensione 2500
 in cinque somme locali effettuate concorrentemente da altrettanti thread.
 Si contempli l'uso di una variabile mutex per regolare l'accesso alla variabile 
 globale che conterrà la somma degli elementi del vettore.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void rand_vec();
int *vec;
int size = 2500;
int sum = 0;
int n = 5;
pthread_mutex_t lock;

void *thread_fun(void *arg)
{
    int *tid = (int *)arg;
    int mysum = 0;
    int offset = size / n * *tid;
    for(int i = 0; i < size / n; i++) {
        mysum += vec[offset + i];
    }
    pthread_mutex_lock(&lock);
    sum += mysum;
    pthread_mutex_unlock(&lock);
    printf("Sum %d is: %d\n", *tid, mysum);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[n];
    int tids[n];
    pthread_mutex_init(&lock, NULL);
    rand_vec();
    for(int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_fun, (void *)&tids[i]);
    }
    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("The total sum is: %d\n", sum);
    free(vec);
    pthread_mutex_destroy(&lock);
    return 0;
}

void rand_vec()
{
    srand(time(NULL));
    vec = calloc(size, sizeof(int));
    for(int i = 0; i < size; i++) {
        vec[i] = rand() % 10;
    }
}
