/*
 * Filename:    queue.h
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains the defined values, dependencies and function prototypes to handle message queues in the CanWeTalkSystem
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "message.h"
#include <pthread.h>

#define EMPTY_QUEUE 0
#define MESSAGE_DEQUEUED 1

// Queue structs here
typedef struct QueueNode // Nodes in queue
{
	Message message;
	struct QueueNode* next;
} QueueNode;

typedef struct  // Message Queue
{
    QueueNode* front;
    QueueNode* rear;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} MessageQueue;

void queueInit(MessageQueue* queue);
void enqueue(MessageQueue *queue, const Message* message);
int dequeue(MessageQueue *queue, Message* msgOut);
void freeQueue(MessageQueue* queue);


#endif