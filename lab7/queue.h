#ifndef _QUEUE_H
#define _QUEUE_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#define QSIZE 256       //??

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* values;
};

struct Queue* createQueue();
bool isFull(struct Queue* queue);
bool isEmpty(struct Queue* queue);
bool enqueue(struct Queue* queue, uint8_t item);
uint8_t dequeue(struct Queue* queue);
uint8_t front(struct Queue* queue);
uint8_t rear(struct Queue* queue);
int queue_size(struct Queue* queue);
void freeQueue(struct Queue* queue);

#endif
