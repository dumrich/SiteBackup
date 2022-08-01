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

This post will go over both of types of I/O by implementing a very simple [Swiper](https://github.com/abo-abo/swiper#swiper)-like utility (for use in the [Cemacs editor](https://git.dumrich.com/chabi/cemacs)) in two different ways. Then, we compare the performance of both implementations.

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


```