/*
 Scrivere un programma che, con l’ausilio della libreria Pthread,
 lancia 3 thread che leggono concorrentemente altrettante directory specificate dall'utente.
 I thread, inoltre, scrivono in  una variabile il numero di file della directory che,
 tra le tre esaminate, contiene il maggior numero di entrate.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>

int num_file;
pthread_mutex_t lock;

void *thread_fun(void *dir)
{
    char *mydir = (char *)dir;
    printf("%s\n", mydir);
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(mydir)) == NULL) {
        printf("Error while opening dir: %s.\n", mydir);
        pthread_exit(NULL);
    }
    int mycount = 0;
    while((dirp = readdir(dp)) != NULL) {
        mycount++;
    }
    pthread_mutex_lock(&lock);
    if(mycount > num_file) {
        num_file = mycount - 2;
    }
    pthread_mutex_unlock(&lock);
    
    closedir(dp);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 4) {
        printf("Usage: %s <dir1> <dir2> <dir3>\n", argv[0]);
        return 0;
    }
    pthread_t threads[3];
    pthread_mutex_init(&lock, NULL);
    num_file = 0;
    for(int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_fun, argv[i+1]);
    }
    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Directory with the most number of files contains %d files.\n", num_file);
    pthread_mutex_destroy(&lock);
    return 0;
}
