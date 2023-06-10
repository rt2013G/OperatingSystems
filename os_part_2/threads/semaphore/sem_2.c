/*
 Realizzare un programma in cui un thread scrittore, dato un intero N da riga di comando
 (dove 0 < N <= 15), scrive in un file nella prima posizione, uno alla volta e ogni 1/2 secondo
 la sequenza di Fibonacci di ordine N, alternandosi con un thread lettore che legge,
 uno alla volta dalla prima posizione, i numeri scritti dal thread scrittore.
 Un terzo thread attende la lettura dell'N-esimo intero, quindi stampa a video il messaggio:
 "Operazioni concluse, arrivederci dal thread: tid", attende 5 secondi e termina.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>

struct {
    sem_t sem_prod;
    sem_t sem_cons;
    sem_t sem_print;
} shared;

struct timespec t = {0, 500000000};
int num;
int fd;
char *filename = "sem_2_file";

int fib(int n);
void *writer(void *arg);
void *reader(void *arg);
void *printer(void *arg);

int main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <integer between 0 and 15>\n", argv[0]);
        exit(0);
    }
    num = atoi(argv[1]);
    if(num <= 0 || num > 15) {
        fprintf(stderr, "Error. Integer must have value between 1 and 15.\n");
        exit(0);
    }
    pthread_t writer_thread;
    pthread_t reader_thread;
    pthread_t printer_thread;
    sem_init(&shared.sem_prod, 0, 0);
    sem_init(&shared.sem_cons, 0, 0);
    sem_init(&shared.sem_print, 0, 0);
    if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777)) < 0) {
        fprintf(stderr, "Error while opening file %s\n", filename);
        exit(0);
    }
    pthread_create(&writer_thread, NULL, writer, NULL);
    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&printer_thread, NULL, printer, NULL);
    pthread_join(printer_thread, NULL);

    sem_destroy(&shared.sem_prod);
    sem_destroy(&shared.sem_cons);
    sem_destroy(&shared.sem_print);
    return 0;
}

void *writer(void *arg)
{
    int temp_fib;
    for(int i = 0; i < num; i++) {
        temp_fib = fib(i);
        lseek(fd, 0, SEEK_SET);
        write(fd, &temp_fib, sizeof(int));
        sem_post(&shared.sem_cons);
        sem_wait(&shared.sem_prod);
        nanosleep(&t, NULL);
    }
    pthread_exit(NULL);
}

void *reader(void *arg)
{
    int read_fib;
    for(int i = 0; i < num; i++) {
        sem_wait(&shared.sem_cons);
        lseek(fd, 0, SEEK_SET);
        read(fd, &read_fib, sizeof(int));
        printf("%dth Fibonacci: %d\n", i, read_fib);
        sem_post(&shared.sem_prod);
    }
    sem_post(&shared.sem_print);
    pthread_exit(NULL);
}

void *printer(void *arg)
{
    sem_wait(&shared.sem_print);
    sleep(5);
    printf("Operazioni concluse, arrivederci dal thread: %u\n", (unsigned) pthread_self());
    pthread_exit(NULL);
}

int fib(int n)
{
    if(n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
