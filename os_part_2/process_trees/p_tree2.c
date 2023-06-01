#include <stdio.h>
#include <unistd.h>

int glob = 5;
int pid = 0;

int main()
{
	for (int i = 1; i < glob; i++) {
		pid = fork();
		if (pid == 0) {
			glob--;
		}
	}
	printf("pid = %d, glob = %d\n", getpid(), glob);

    return 0;
}
