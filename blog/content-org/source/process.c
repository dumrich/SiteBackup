#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int id = fork();

    if(id != 0) {
        printf("%d\n", getpid());
        printf("%d\n", getppid());
    } else {
        printf("\n%d %d", getpid(), getppid());
    }
}
