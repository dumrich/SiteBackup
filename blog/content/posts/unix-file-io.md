+++
title = "File I/O on Unix systems"
author = ["""
  "Abhinav Chavali"
  """]
date = 2022-07-31
tags = ["Unix", "C", "Files"]
categories = ["Programming"]
draft = false
+++

Unix systems have two ways of doing Input/Output (I/O) in C:

-   POSIX File I/O
-   ISO C Standard I/O

There are 2 different interfaces for I/O because the latter is meant to be portable. C implmentations must provide the `<stdio.h>` header, so it could work theoretically for any C implementation.

The POSIX implementation are unbuffered (more on that later), and usually invoke direct system calls to the kernel.

This post will go over both of types of I/O by implementing a very simple [Swiper](https://github.com/abo-abo/swiper#swiper)-like utility (for use in the [Cemacs editor](https://git.dumrich.com/chabi/cemacs)) in two different ways.

Sources: [APUE](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739), [TLPI](https://www.amazon.com/Linux-Programming-Interface-System-Handbook/dp/1593272200/ref=pd_lpo_1?pd_rd_i=1593272200&psc=1)


## POSIX File I/O {#posix-file-i-o}

At the core of the unbuffered POSIX functions are `file descriptors`. File descriptors are non-negative integers that are used to identify open files.

In C, three file descriptors (always the smallest numbers), are automatically reserved for the standard input, the standard output and the standard error (`STDIN_FILNO`, `STDOUT_FILENO`, and `STDERR_FILENO` in `<unistd.h>`).

In Unix, these are considered files, which we can read and write to as any other.

One thing to note about file descriptors, is that the operating system will always assign an open file the smallest file descriptor. For example, if the programmer decides to open a new file at the very beginning of the main function, the file will likely have the descriptor of `3`, as 0, 1, and 2 are already reserved.


### Basic implementation {#basic-implementation}

We start with our interface function; It usually takes a `*Editor`, but for our purposes, we'll reduce it down to the pathname and the string we want to search for. We will also test it in a :

```C
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define BUFFSIZE 4096

void find_in_file(int, size_t, char*);

int main() {
    /* Open File with flags */
    int fd = open("unix-file-io.org", O_RDONLY|O_CREAT);

    find_in_file(fd, "file");

    return close(fd);
}
```

First, we include some headers, specifically:

-   `<unistd.h>` gives us the close, lseek functions.
-   `<fcntl.h>` gives us the open function, and oflag constants
-   `<ctype.h>` gives us the tolower function

We also define the function `find_in_file` and the buffer size.

The buffer size will be how many bytes we will read from the file at a time. More on this later.

The `open` function takes a path, and some flags, and returns a file descriptor. This file descriptor is how we will refer to the file from here on out. On most Unix systems, this will be the value 3, as it is the smallest available integer.

Different flags can be Or'd together to create a mode that will determine access permissions. A complete list can be found [here](https://www.gnu.org/software/libc/manual/html_node/Access-Modes.html).

In this case, we Or the RDONLY flag, and the CREAT flag. This means, that we can only read the file, and if it doesn't exist, it will be created. If we Or'd with a write flag, we would also be able to write to the file.

Next, we call our function, giving it the phrase "file".

Finally, we close the file. When the process exits, the open files automatically close.

Next, the find_in_file function:

```C
char buf[BUFFSIZE];
void find_in_file(int fd, char* phrase) {

    int current_line = 1;
    bool is_eof = false;

    int match_length = strlen(phrase);
    int buf_len;

    while(!is_eof) {
        if((buf_len = read(fd, buf, BUFFSIZE)) != BUFFSIZE) {
            is_eof = true;
        }

        for(int i = 0; i < buf_len - strlen(phrase); i++) {
            int cur_cmp = 0;
            while(cur_cmp < match_length) {
                if(buf[i+cur_cmp] == '\n') {
                    current_line++;
                    goto result;
                }
                if(tolower(buf[i + cur_cmp]) != tolower(phrase[cur_cmp])) {
                    goto result;
                } else {
                    cur_cmp++;
                }
            }
            printf("Line: %d\n", current_line);

        result:
            continue;
        }
    }
}
```

-   First, we create a buffer of size BUFFSIZE.

-   Then, we set some preliminary flags. The initial line is 1, and the offset has not reached the end of file. Finally, we set the length of the phrase to how many characters we have to match.

-   Now, we read BUFFSIZE bytes into the buffer. `read` returns the number of bytes it read, so if that number is less than the size of the buffer, we know that we have reached the end of the file. In that case, the while loop won't loop again.

-   Now that the bytes are in the buffer, we iterate through the current buffer with an algorithm to check if the characters match.

-   We check how many characters have been matched, and if the number of characters matched is equal to the length of the phrase, that is a match. If so, we print the line number. We increment the line number if we encounter a newline character.


### Other Functions {#other-functions}

There are some other functions that may be useful to know.

```C
#include <fcntl.h>
int creat(const char* path, mode_t mode);
```

This function is equal to `open(path, O_WRONLY | O_CREAT | O_TRUNC, mode)`;

Notice how it is opened for writing only.

The `lseek` function is another useful function that is used to manipulate the offset (aka current position in a file).

```C
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
```

offset could mean different things depending on `whence`.

-   If whence is SEEK_SET, the files current position if offset from the beginning.
-   If whence is SEEK_CUR, the files current position if offset from the current position.
-   If whence is SEEK_END, the files current position if offset from the size of the file.

If you seek past the end of the file, that creates a hole in the file. The hole is not backed by a storage medium.

Reading and writing operations also manage the offset of the file.


## C Standard I/O {#c-standard-i-o}

The C Standard I/O works a little bit differently. We still read and write and interact with files, but this I/O is a little bit more featureful because it comes with internal buffering. The FILE object maintains its own buffer so it doesn't have to make a system call to read or write every single time.

It may read a little more the first time, so it would just be getting data from its own buffer.

The 3 kinds of I/O are:

-   Character I/O, where one character is read or written
-   Line I/O, where one line is read or written
-   Direct I/O, where the exact number of objects and size of each object is specified.


### Basic implementation {#basic-implementation}

This is a basic implementation with buffering. This time when we use fopen, we are returned a `FILE*`. Instead of Or-ing options, we can just add a string of permissions. In this case, we only need "r" (read) permissions.

```C
#define BUFFSIZE 4096

void find_in_file(FILE*, const char*);

int main() {
    /* Open File with flags */
    FILE* fd = fopen("search.c", "r");

    find_in_file(fd, "file");

    return fclose(fd);
}
```

Now, we implement the same basic algorithm as before:

```C
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
```

This time fgets reads only one line at a time into the buffer. We implement the same algorithm, though this one works a little bit better.


### Other Functions {#other-functions}

Some functions commonly used for Binary I/O are the fread and write functions.

For example, a binary array can be written to a file as such:

```C
float buf[8] = {1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1};

int main(void) {
    fwrite(&buf[2], sizeof(float), 2, stdout);
    exit(0);
}

```

Also, an equivalent to the lseek function above is `fseek` and `ftell`.

`ftell` simply returns the current offset.

`fseek` can be used to set the offset, with the same file modes.

```C
#include <stdio.h>

long ftell(FILE* fp);

int fseek(FILE* fp, long offset, int whence);
```