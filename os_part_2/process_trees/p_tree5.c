#include <stdio.h>
#include <unistd.h>

int glob = 0;
int cocc = 0;
int hug = 0;

int main()
{
    int i = 0;
    int pid = 0;
    glob++;
    cocc++;
	pid = fork();
	glob = cocc;
	cocc = glob;
	pid = fork();
    if(pid != 0) {
		for(i = 1; i < 2; i++) {
            pid = fork();
		}
		cocc = glob + 2 * 3;
	} else if(pid == 0) {
        for(i = 2; i <= 3; i++) {
            pid = fork();
        }
		if(pid != 0) {
			hug = 4;
			cocc = cocc * 5;
		}
		glob = cocc - 2*4;
    }
    hug = hug - 3; 
	printf("Process %d. glob %d, cocc %d, hug %d, i %d\n\n", getpid(), glob, cocc, hug, i);

    return 0;
}
