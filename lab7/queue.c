#include "queue.h"


struct Queue* createQueue(){
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = QSIZE;
    queue->front = queue->size = 0;

    queue->rear = QSIZE - 1;
    queue->values = (int*) malloc(queue->capacity * sizeof(int));

    return queue;
}

bool isFull(struct Queue* queue){
    return (queue->size == (int) queue->capacity);
}

bool isEmpty(struct Queue* queue){
    return (queue->size == 0);
}

bool enqueue(struct Queue* queue, uint8_t item){
    if(isFull(queue)){
        return false;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->values[queue->rear] = item;
    queue->size = queue->size + 1;
    return true;
}

uint8_t dequeue(struct Queue* queue){
    if(isEmpty(queue)){
        return 0;
    }
    uint8_t item = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;

}

uint8_t front(struct Queue* queue){
    if(isEmpty(queue)){
        return 0;
    }
    return queue->values[queue->front];
}

uint8_t rear(struct Queue* queue){
    if(isEmpty(queue)){
        return 0;
    }
    return queue->values[queue->rear];

}

int queue_size(struct Queue* queue){
    return queue->size;
}

void freeQueue(struct Queue* queue){
    free(queue->values);
    free(queue);
}
