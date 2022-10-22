// Circular Queue implementation
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define ARR_SIZE 10

int length = 0;

int queue[ARR_SIZE];
int front = -1;
int rear = -1;

bool isEmpty();
bool isFull();

void enqueue(int x);
int dequeue(void);

bool isEmpty() {
    return length == 0;
}

bool isFull() {
    return (length == ARR_SIZE);
}

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

int main(void) {
    enqueue(4);
    enqueue(5);
    enqueue(6);
    enqueue(7);
    enqueue(9);
    enqueue(6);
    enqueue(7);
    enqueue(9);
    enqueue(6);
    enqueue(7);
    enqueue(9);
    enqueue(9);
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
}
