/*
 Scrivere un programma che resta sempre in attesa di un segnale: 
 - se riceve SIGINT stampa a video il fattoriale di un numero, 
   compreso tra 1 e 20, passato come argomento da linea di commando; 
 - se riceve SIGUSR1 legge il contenuto del file prova.txt
 - se riceve SIGUSR2 crea un file prova1.txt con diritti di lettura e scrittura per tutti
   tranne per gli “altri” e vi scrive il contenuto del file prova.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

char buf[4096];
int n;
int fact(int num);

static void sig_fun(int signo)
{
    if(signo == SIGINT) {
        printf("Factorial of %d: %d\n", n, fact(n));
        return;
    } else if(signo == SIGUSR1) {
        int fd;
        if((fd = open("test_dir/prova.txt", O_RDONLY)) < 0) {
            printf("Error while opening prova.txt\n");
            return;
        }
        read(fd, &buf, 4096);
        printf("%s\n", buf);
        return;        
    } else if(signo == SIGUSR2) {
        int fd;
        fd = open("test_dir/prova1.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        write(fd, &buf, strlen(buf));
        return;
    }
    return;
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return 0;
    }
    n = atoi(argv[1]);
    if(signal(SIGINT, sig_fun) == SIG_ERR) {
        printf("Error: can't catch SIGINT");
        return 0;
    }
    if(signal(SIGUSR1, sig_fun) == SIG_ERR) {
        printf("Error: can't catch SIGUSR1");
        return 0;
    }
    if(signal(SIGUSR2, sig_fun) == SIG_ERR) {
        printf("Error: can't catch SIGUSR2");
        return 0;
    }
    while(1) pause();
}

int fact(int num)
{
    int fact = 1;
    for(int i = 1; i <= num; i++) {
        fact *= i;
    }
    return fact;
}
