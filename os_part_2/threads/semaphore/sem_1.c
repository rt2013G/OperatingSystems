/*
 Si progetti un programma che, che crei un thread che estrae N volte (con N passato da riga di comando) 
 un numero casuale compreso tra 0 e 10 e ne calcoli il fattoriale, ponendono in una variabile condivisa,
 e un thread che stampi a video il fattoriale calcolato N volte prelevandolo da tale variabile.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int num;
struct {
    sem_t mutex;
    sem_t sem_prod;
    sem_t sem_cons;
    int fact_n;
} shared;

int fact(int n);
void *prod(void *arg);
void *cons(void *arg);

int main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "Error. Usage: %s <integer>\n", argv[0]);
        exit(0);
    }
    num = atoi(argv[1]);
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.sem_prod, 0, 0);
    sem_init(&shared.sem_cons, 0, 0);
    pthread_t producer;
    pthread_t consumer;
    pthread_create(&producer, NULL, prod, NULL);
    pthread_create(&consumer, NULL, cons, NULL);
    pthread_join(consumer, NULL);


    sem_destroy(&shared.mutex);
    sem_destroy(&shared.sem_prod);
    sem_destroy(&shared.sem_cons);
    return 0;
}

void *prod(void *arg)
{
    for(int i = 0; i < num; i++) {
        sleep(1);
        srand(time(NULL));
        sem_wait(&shared.mutex);
        shared.fact_n = fact(rand() % 11);
        sem_post(&shared.mutex);
        sem_post(&shared.sem_cons);
        sem_wait(&shared.sem_prod);
    }
    pthread_exit(NULL);
}

void *cons(void *arg)
{
    for(int i = 0; i < num; i++) {
        sem_wait(&shared.sem_cons);
        sem_wait(&shared.mutex);
        printf("Value %d: %d\n", i+1, shared.fact_n);
        sem_post(&shared.mutex);
        sem_post(&shared.sem_prod);
    }
    pthread_exit(NULL);
}

int fact(int n)
{
    int fac = 1;
    for(int i = 1; i <= n; i++) {
        fac *= i;
    }
    return fac;
}
