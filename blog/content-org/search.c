#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#define BUFFSIZE 4096

struct Res {
    int line_num;
    char* line;
    struct Res* next;
    struct Res* prev;
};

typedef struct Res Res;

Res* find_in_file(int, size_t, char*);

int main() {
    /* Open File with flags */
    int fd = open("unix-file-io.org", O_RDONLY|O_CREAT|O_TRUNC);
    /* Arbitrary value for testing purposes. */
    int offset = 22;

    return close(fd);
}

char buf[BUFFSIZE];
Res* find_in_file(int fd, size_t s, char* phrase) {
    lseek(fd, s, SEEK_SET); /* Other options: SEEK_CUR, SEEK_END */

    int current_line = 0;
    bool is_eof = false;

    int buf_len;
    
    while(!is_eof) {
        if((buf_len = read(fd, buf, BUFFSIZE)) != BUFFSIZE) {
            is_eof = true;
        } 

        for(int i = 0; i < buf_len - strlen(phrase); i++) {
            int match_length = strlen(phrase);
            int cur_cmp = 0;
            while(match_length != 0 && cur_cmp <= match_length) {
                if(buf[i + cur_cmp] != phrase[i + cur_cmp]) {
                    goto result;
                }
                cur_cmp++;
            }
            
        result:
            continue;
        }
    }
}
