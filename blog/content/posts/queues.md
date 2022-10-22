+++
title = "Implementing Circular Queues in C"
author = ["""
  "Abhinav Chavali"
  """]
date = 2022-10-21
tags = ["C", "Data_Structures"]
categories = ["Programming"]
draft = false
+++

## Implementing Circular Queues in C {#implementing-circular-queues-in-c}

In this article, I will be implementing a circular queue in C while explaining the theory of how it works.

[Source Code](https://github.com/dumrich/SiteBackup/blob/master/blog/content-org/source/queue.c)


### What is a Queue? {#what-is-a-queue}

A _queue_ is a [First In, First Out](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)) data structure. This means that items will be removed in the same order that they are added, hence first in, first out.

A frequent analogy that is used to describe queues is a line at some store. The first person in the line will be the first person to be served.

In computer science, queues are used for CPU scheduling (Can't execute all tasks at once), or requests in a web server.


### Simple Queue implementation {#simple-queue-implementation}

We will start off with a simple array based queue implementation and look at the downsides of that:

```C
#define ARR_SIZE 10

int queue[ARR_SIZE];
int front = -1;
int rear = -1;

bool isEmpty();
bool isFull();

void enqueue(int x);
int dequeue(void);

```

We define two main functions, which are the two main operations in the queue: **Enqueue** and **Dequeue**.

In an enqueue, we add an item to our queue, which in this case is an `int`. In a dequeue, we return the first item in the queue and remove it.

Our queue is at most 10 elements large, and we set the front and rear. Everything in between can be considered our queue.

Now our function implementations:

```C
bool isEmpty() {
    return (front == -1 ) && (rear == -1);
}

bool isFull() {
    return (rear == front - 1);
}

void enqueue(int x) {
    if(isEmpty()) {
        front = 0;
    }

    queue[++rear] = x;
}

int dequeue(void) {
    if(isEmpty()) {
        return INT_MIN;
    }
    if(rear == -1) {
        rear = 0;
    }

    return queue[front++];

}
```

Our enqueue function simply increments the rear, and adds our new value. If the queue is empty, we set it up by just setting front to the first value in the queue, or 0.

Our deuque implementation does the same empty check, but if it is determined to be empty, we just return the lowest possible integer value. If the caller recieves this value, they know it is somehow invalid.


#### Testing our queue {#testing-our-queue}

Now, let's write the main function:

```C
int main(void) {
    enqueue(4);
    enqueue(5);
    enqueue(6);
    enqueue(7);
    enqueue(9);
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    enqueue(6);
    enqueue(7);
    enqueue(9);
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
  }
```

This queue works, but there is one major problem: the more enqueues and dequeues we do, the less space we have. Picture the array after a couple enqueues and dequeues: There is space at the beginning that we simply cannot recover without changing our implementation.

This figure illustrates that problem:

```ascii
     Unused Space                         Queue
  ┌────────────────────────┐         ┌───────────────────────────────────────────┐
  │                        │         │                                           │
┌─┴────┐   ┌──────┐    ┌───┴──┐   ┌──┴───┐    ┌──────┐   ┌──────┐   ┌──────┐   ┌─┴────┐
│      ├───┤      ├────┤      ├───┤      ├────┤      ├───┤      ├───┤      ├───┤      │
│      │   │      │    │      │   │      │    │      │   │      │   │      │   │      │
└──────┘   └──────┘    └──────┘   └──────┘    └──────┘   └──────┘   └──────┘   └──────┘
```


### Circular Queue {#circular-queue}

A circular array is a better solution for this. With a circular array, we can recycle old indexes by reusing them once they are no longer in use.

When we reach the end of the array, we should start from the beginning if there are unused spaces. This way, the array becomes ciricular.

Note, when implementing this, we don't modify the array, but rather the way we index and increment the front and rear.

First, lets make this a little easier to understand by creating a length variable:

```C
int length = 0;

int queue[ARR_SIZE];
int front = -1;
int rear = -1;
```

Then, we can modify are `isEmpty` and `isFull` functions:

```C
bool isEmpty() {
    return length == 0;
}

bool isFull() {
    return (length == );
}
```

Finally, we rewrite the enqueue and dequeue functions:

```C
void enqueue(int x) {
    if(isFull()) {
        return;
    }
    if(isEmpty()) {
        front = 0;
    }

    rear = (rear + 1) % ARR_SIZE;
    queue[rear] = x;
    length++;
}

int dequeue(void) {
    if(isEmpty()) {
        return INT_MIN;
    }
    int toReturn = queue[front];
    front = (front + 1) % ARR_SIZE;
    length--;
    return toReturn;

}
```

This works because of the modulus operator. Whenever we reach the size of the array, we can wrap around as a remainder to an index at the beginning of the array.

Now, we have a queue that maximizes memory potential.