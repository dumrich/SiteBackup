#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define BUFFSIZE 4096

void find_in_file(FILE*, const char*);

int main() {
    /* Open File with flags */
    FILE* fd = fopen("search.c", "r");

    find_in_file(fd, "file");

    return fclose(fd);
}

char buf[BUFFSIZE];
void find_in_file(FILE* fp, const char* phrase) {
    int current_line = 1;

    const int match_length = strlen(phrase);
    while(fgets(buf, BUFFSIZE, fp)) {
        int buf_len = strlen(buf);
        if(buf_len < match_length) {
            continue;
        }

        for(int i = 0; i < buf_len - match_length; i++) {
            int cur_cmp = 0;
            while(cur_cmp < match_length) {
                if(tolower((unsigned char)buf[i + cur_cmp]) != tolower((unsigned char)phrase[cur_cmp])) {
                    goto result;
                } else {
                    cur_cmp++;
                }
            }
            printf("Line: %d\n", current_line); 

        result:
            continue;
        }
        current_line++;
    }
}
