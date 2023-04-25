#include <stdio.h>
#include <unistd.h>

int glob = 4;
int pid = 0;

int main()
{
    for (int i = 1; i < 4; i++) {
		if(pid == 0) {
			pid = fork();
		}
        if(pid != 0) {
			glob = glob * 2;
		}
        glob = glob - 1;
	}
	printf("pid = %d, glob = %d\n", getpid(), glob);

    return 0;
}
