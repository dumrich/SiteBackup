#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "%s: Enter 1 argument", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    int size = lseek(fd, 0, SEEK_END);
    printf("%d\n", size);
}
