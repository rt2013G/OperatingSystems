/*
 Scrivere un programma che, con l’ausilio della libreria Pthread,
 lancia n (con n > 1) thread che scrivono concorrentemente un header
 (contenete le informazioni sui diritti RWX e sulla dimensione)
 in altrettanti file con estensione txt specificati dall'utente
 (ed eventualmente creati prima).
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

pthread_mutex_t lock;
int n;

void *thread_fun(void *arg)
{
    char *filename = (char *) arg;
    int fd;
    if((fd = open(filename, O_RDWR | O_APPEND)) < 0) {
        printf("Error while opening file %s.\n", filename);
        pthread_exit(NULL);
    }
    struct stat st;
    fstat(fd, &st);
    char buf[128];
    sprintf(buf, "SIZE: %d, PERMISSIONS: %d", st.st_size, st.st_mode);
    write(fd, &buf, strlen(buf));

    printf("FILE: %s\n", filename);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 0;
    }
    n = argc - 1;
    pthread_t threads[n];
    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, thread_fun, (void *) argv[i + 1]);
    }
    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
