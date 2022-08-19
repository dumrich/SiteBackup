#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define BUFFSIZE 1024

char buffer[BUFFSIZE];

int main(int argc, char** argv) {
    printf("%d\n", argc); 
    if(argc != 3 || strcmp(argv[1], "--help") == 0) {
        char s[] = "cp(1) takes 3 arguments\n";
        write(STDERR_FILENO, s, sizeof(s) - 1);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    int to_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    while(read(fd, buffer, BUFFSIZE) > 0) {
        write(to_fd, buffer, BUFFSIZE);
    }

    return 0;
}
