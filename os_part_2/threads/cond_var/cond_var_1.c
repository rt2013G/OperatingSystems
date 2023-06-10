/*
 Realizzare un programma che, con l'ausilio della libreria Pthread, lancia n (n<10)
 thread che cercano di leggere una variabile il cui contenuto viene impostato
 ogni secondo generando casualmente un numero intero compreso tra 1 e 100.
 Il thread che riesce a leggere la variabile ne scrive il contenuto in una matrice
 5x5 allocata precedentemente. Una volta riempita la matrice, il programma termina
 facendo in modo che ciascun thread stampi in output il proprio identificativo 
 seguito dal numero di volte in cui ha letto la variabile ed uno solo di essi
 provvede a stampare la matrice. Utilizzare le variabili di condizione
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_once_t once;
    int changed;
    int finished;
    int num;
    int **matrix;
    int i_matrix, j_matrix;
} shared = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_ONCE_INIT, 0, 0};
int n_threads;

void alloc_matrix();
void *read_thread();
void *var_thread();
void print_matrix();

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <number of threads < 10>\n", argv[0]);
        return 0;
    }
    n_threads = atoi(argv[1]);
    if(n_threads >= 10) {
        printf("Error.\n");
        return 0;
    }
    pthread_t threads[n_threads];
    pthread_t thread_v;
    alloc_matrix();
    for(int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, read_thread, NULL);
    }
    pthread_create(&thread_v, NULL, var_thread, NULL);
    for(int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void *var_thread(void *arg)
{
    while(1) {
        sleep(1);
        srand(time(NULL));
        pthread_mutex_lock(&shared.lock);
        shared.num = (rand() % 100) + 1;
        shared.changed = 1;
        pthread_mutex_unlock(&shared.lock);
        pthread_cond_signal(&shared.cond);
    }
}

void *read_thread(void *arg)
{
    int my_count = 0;
    unsigned int my_tid = pthread_self();
    while(1) {
        pthread_mutex_lock(&shared.lock);
        if(shared.finished) {
            pthread_mutex_unlock(&shared.lock);
            printf("TID: %u, VAR LETTE: %d\n", my_tid, my_count);
            pthread_once(&shared.once, print_matrix);
            pthread_exit(NULL);
        }
        if(shared.changed) {
            shared.matrix[shared.i_matrix][shared.j_matrix++] = shared.num;
            my_count++;
            shared.changed = 0;
            if(shared.j_matrix >= 5) {
                if(shared.i_matrix >= 4) {
                    shared.finished = 1;
                } else {
                    shared.j_matrix = 0;
                    shared.i_matrix++;
                }
            }
        } else {
            pthread_cond_wait(&shared.cond, &shared.lock);
        }
        pthread_mutex_unlock(&shared.lock);
    }
}

void print_matrix(void *arg)
{
    printf("MATRIX:\n");
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            printf("%d, %d: %d\n", i, j, shared.matrix[i][j]);
        }
    }
}

void alloc_matrix()
{
    shared.matrix = calloc(5, sizeof(int*));
    for(int i = 0; i < 5; i++) {
        shared.matrix[i] = calloc(5, sizeof(int));
    }
    shared.i_matrix = 0;
    shared.j_matrix = 0;
}
