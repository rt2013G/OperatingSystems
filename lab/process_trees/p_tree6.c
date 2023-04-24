#include <stdio.h>
#include <unistd.h>

int glob = 5;
int pid = 0;

int main()
{
    fork();
	glob--;
	if(fork()) {
		glob--;
	}
	if(pid == 0) {
		pid = fork();
		glob--;
	}
	printf("Sono %d figlio di %d. Glob: %d\n\n", getpid(), getppid(), glob);

    return 0;
}
