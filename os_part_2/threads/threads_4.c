/*
 Si realizzi un programma che, utilizzando la libreria Pthread,
 lancia n thread per cercare un elemento in una matrice n*n di interi.
 Ognuno dei thread cerca l'elemento in una delle righe della matrice.
 Non appena un thread ha trovato l'elemento cercato, 
 rende note agli altri thread le coordinate dell'elemento 
 e tutti i thread terminano (sono cancellati).
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void rand_matrix();
void print_matrix();
int coord[] = {-1, -1};
int **matrix;
int n;
int key;
int *tids;
pthread_t *threads;
pthread_mutex_t mutex;

void *search(void *tid)
{
    int *mytid = (int *) tid;
    for(int i = 0; i < n; i++) {
        if(matrix[*mytid][i] == key) {
            pthread_mutex_lock(&mutex);
            coord[0] = *mytid;
            coord[1] = i;
            for(int k = 0; k < n; k++) {
                if(k != *mytid) {
                    pthread_cancel(threads[k]);
                }
            }
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("Usage: %s <size of the matrix> <integer to search>\n", argv[0]);
        return 0;
    }
    n = atoi(argv[1]);
    key = atoi(argv[2]);
    tids = calloc(n, sizeof(int));
    threads = calloc(n, sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    rand_matrix();
    print_matrix();
    for(int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, search, (void *)&tids[i]);
    }
    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    if(coord[0] == -1 && coord[1] == -1) {
        printf("Integer not found.\n");
    } else {
        printf("Integer %d found at %d, %d.\n", key, coord[0], coord[1]);
    }
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(tids);
    return 0;
}

void rand_matrix()
{
    matrix = calloc(n, sizeof(int *));
    for(int i = 0; i < n; i++) {
        matrix[i] = calloc(n, sizeof(int));
    }
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 20;
        }
    }
}

void print_matrix()
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("Value %d, %d: %d\n", i, j, matrix[i][j]);
        }
    }
}
