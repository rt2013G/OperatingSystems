#include <stdio.h>
#include <unistd.h>

int glob = 20;
int pid = 0;

int main()
{
    for(int i = 2; i < 4; i++) {
        if((pid = fork()) == 0) {
            glob *= 2;
        }
        glob--;
    }
    printf("pid = %d, glob = %d\n", getpid(), glob);

    return 0;
}
