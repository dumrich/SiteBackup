+++
title = "Files and I/O on Unix systems"
author = ["""
  "Abhinav Chavali"
  """]
date = 2022-07-31
tags = ["Unix", "C", "Files"]
categories = ["Programming"]
draft = true
+++

Most Unix systems have two ways of doing Input/Output (I/O) in C:

-   POSIX File I/O
-   ISO C Standard I/O

There are 2 different interfaces for I/O because the latter is meant to be portable. C implmentations must provide the `<stdio.h>` header, so it would work theoretically for any C implementation.

The POSIX implementation are unbuffered (more on that later), and usually invoke direct system calls to the kernel.

This post will go over both of them, as well as some other ways to manipulate files and directories.

Sources: [APUE](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739), [TLPI](https://www.amazon.com/Linux-Programming-Interface-System-Handbook/dp/1593272200/ref=pd_lpo_1?pd_rd_i=1593272200&psc=1)


## POSIX File I/O {#posix-file-i-o}

Unix systems provide some functions to perform direct, unbuffered I/O with files.