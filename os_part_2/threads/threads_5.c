/*
 Si realizzi un programma che, con l’ausilio della libreria Pthread,
 lancia n thread per calcolare la somma degli elementi di ciascuna riga
 di una matrice n*n di interi generati casualmente in un intervallo [0,255], allocata dinamicamente.
 Il calcolo della somma degli elementi di ciascuna riga deve essere effettuato concorrentemente 
 su tutte le righe secondo la seguente modalità:
 - il thread i-esimo con i pari, calcola la somma degli elementi di indice pari della riga i-esima
 - il thread con indice i dispari, calcola la somma degli elementi di indice dispari della riga i-esima.
 Calcolate le somme parziali, si provvederà a ricercarne il minimo ed a stamparlo a video.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void rand_matrix();
void print_matrix();
int **matrix;
int *tids;
int *sums;
int n;
pthread_t *threads;

void *func(void *tid)
{
    int *mytid = (int *) tid;
    int mysum = 0;
    if(*mytid % 2 == 0) {
        for(int i = 0; i < n; i++) {
            if(i % 2 == 0) {
                mysum += matrix[*mytid][i];
            }
        }
    } else {
        for(int i = 0; i < n; i++) {
            if(i % 2 != 0) {
                mysum += matrix[*mytid][i];
            }
        }
    }
    sums[*mytid] = mysum;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <size of the matrix>\n", argv[0]);
        return 0;
    }
    n = atoi(argv[1]);
    threads = calloc(n, sizeof(pthread_t));
    tids = calloc(n, sizeof(int));
    sums = calloc(n, sizeof(int));
    rand_matrix();
    print_matrix();

    for(int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, func, (void *)&tids[i]);
    }
    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    int min = sums[0];
    for(int i = 1; i < n; i++) {
        if(sums[i] < min) min = sums[i];
    }
    printf("The minimum sum is: %d.\n", min);

    free(threads);
    free(sums);
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
            matrix[i][j] = rand() % 256;
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
