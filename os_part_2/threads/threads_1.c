/* 
 Scrivere un programma che accetta un intero n da riga di comando,
 crea n thread e poi aspetta la loro terminazione.
 Ciascun thread aspetta un numero di secondi casuale tra 1 e 5, 
 poi incrementa una variabile globale intera ed infine ne stampa il valore 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int glob = 0;

void *thread_fun(void *arg)
{
    int random = rand() % 5 + 1;
    sleep(random);
    glob++;
    printf("GLOB: %d\n", glob);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    pthread_t threads[n];
    srand(time(NULL));

    for(int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, thread_fun, NULL);
    }

    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("GLOB: %d\n", glob);
}
