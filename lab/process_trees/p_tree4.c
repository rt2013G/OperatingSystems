#include <stdio.h>
#include <unistd.h>

int glob = 0;
int pid = 0;

int main()
{
    pid = fork();
	if(pid != 0) {	
		glob++;
		pid = fork();
		glob--;
	} else if(pid == 0) {
		for(int i = 1; i <= 2; i++) {
			pid = fork();
			glob++;
		}
	}
	glob--;	
	printf("pid = %d, glob = %d\n", getpid(), glob);

    return 0;
}
