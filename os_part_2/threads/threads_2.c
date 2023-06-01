/*
 Scrivere un programma che prende in input un intero n, 
 il nome di un file di testo ed un carattere x.
 Il programma ha il compito di contare le occorrenze 
 del carattere x nel file di testo.

 Il programma esegue tale compito creando n thread, 
 ognuno dei quali esamina una porzione diversa del file di testo, 
 ad esempio, se il file è lungo 1000 bytes ed n=4, 
 il primo thread esaminerà i primi 250 bytes, 
 il secondo thread esaminerà i 250 bytes successivi, e così via.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

typedef struct shared {
    int num_thread;
    int id_thread;
} shared;
char *filename;
char c_to_find;
int char_found;
pthread_mutex_t mutex;

void *thread_fun(void *arg)
{
    shared *res = (shared *)arg;
    int n = res->num_thread;
    int id = res->id_thread;
    int char_found_thread = 0;
    int fd;
    int bytes_read;
    if((fd = open(filename, O_RDONLY)) == 0) {
        printf("Errore nella open.\n");
        exit(1);
    }
    struct stat st;
    fstat(fd, &st);
    off_t size = st.st_size;
    int bytes_to_read = size / n;
    off_t offset = (size / n) * id;
    lseek(fd, offset, SEEK_SET);
    char buf[bytes_to_read];
    if((bytes_read = read(fd, &buf, bytes_to_read)) == 0) {
        printf("Error while reading bytes.\n");
        pthread_exit(NULL);
    }
    for(int i = 0; i < bytes_read; i++) {
        if(buf[i] == c_to_find) {
            char_found_thread++;
        }
    }
    pthread_mutex_lock(&mutex);
    char_found += char_found_thread;
    pthread_mutex_unlock(&mutex);
    printf("Char found by thread %d: %d.\n", pthread_self(), char_found_thread);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 4) {
        printf("Usage: %s <n of threads> <file name> <character>\n", argv[0]);
        exit(1);
    }
    int n_threads = atoi(argv[1]);
    filename = argv[2];
    c_to_find = *argv[3];
    char_found = 0;
    pthread_t threads[n_threads];
    pthread_mutex_init(&mutex, NULL);
    shared *shared_res;
    for(int i = 0; i < n_threads; i++) {
        shared_res = malloc(sizeof(shared));
        shared_res->num_thread = n_threads;
        shared_res->id_thread = i;
        pthread_create(&threads[i], NULL, thread_fun, (void *) shared_res);
    }
    for(int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    sleep(1);
    pthread_mutex_destroy(&mutex);
    printf("Characters found: %d.\n", char_found);
}
