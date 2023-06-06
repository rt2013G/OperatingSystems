/*
 Scrivere un programma che, utilizzando la libreria pthread e i mutex allocati dinamicamente,
 crea 2 thread i quali eseguono una funzione che accede alle variabili a e b 
 di una struttura condivisa glob e ne incrementa il loro valore di 2 per 10 volte.
 Al termine del programma il thread principale stampa a video i valori di glob.a e glob.b.
*/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

struct {
    int a;
    int b;
    pthread_mutex_t mutex;
} glob;

void func(void *arg)
{
    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(&glob.mutex);
        glob.a += 2;
        pthread_mutex_unlock(&glob.mutex);

        pthread_mutex_lock(&glob.mutex);
        glob.b += 2;
        pthread_mutex_unlock(&glob.mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    pthread_mutex_init(&glob.mutex, NULL);
    pthread_create(&thread1, NULL, (void *)func, NULL);
    pthread_create(&thread2, NULL, (void *)func, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&glob.mutex);
    printf("VALUE OF a: %d, VALUE OF b: %d\n", glob.a, glob.b);
    return 0;
}
