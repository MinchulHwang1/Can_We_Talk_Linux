/*
 * Filename:    queue.c
 * Project:     CanWeTalkSystem 
 * By:          Salman Nouman, Minchul Hwang, Md Saiful Islam, Saje-Antoine Rose
 * Date:        April, 02, 2024
 * Description: This file contains functionality to handle the message queue operations.
 */

#include "../inc/queue.h"

/*
 * Function:    queueInit
 * Description: Initializes a message queue.
 * Parameters:  MessageQueue* queue: Pointer to the message queue structure to be initialized
 * Returns:     void
 */

void queueInit(MessageQueue* queue)
{
	queue->front = queue->rear = NULL;
    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

/*
 * Function:    enqueue
 * Description: Adds a message to the end of a message queue.
 * Parameters:  MessageQueue* queue: Pointer to the message queue structure.
 *              const Message* message: Pointer to the message to be enqueued.
 * Returns:     void
 */
void enqueue(MessageQueue *queue, const Message* message)
{
    pthread_mutex_lock(&queue->lock);
    QueueNode* newNode = malloc(sizeof(QueueNode));
    
    // Copy the provided message into the new node
    memcpy(&newNode->message, message, sizeof(Message));
    newNode->next = NULL;

    if (queue->rear == NULL) 
	{ // Empty queue
        queue->front = queue->rear = newNode;
    } 
	else 
	{ // Non-empty queue
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->lock);
}


/*
 * Function:    dequeue
 * Description: Removes a message from the front of a message queue.
 * Parameters:  MessageQueue* queue: Pointer to the message queue structure.
 *              Message* msgOut: Pointer to a Message structure where the dequeued message will be stored.
 * Returns:     int: 1 if successful (message dequeued), 0 if the queue was empty.
 */
int dequeue(MessageQueue *queue, Message* msgOut)
{
	pthread_mutex_lock(&queue->lock);
    
    if (queue->front == NULL) 
	{ // Check for empty queue
        pthread_mutex_unlock(&queue->lock);
        return EMPTY_QUEUE; // Indicate queue was empty
    }

    QueueNode* temp = queue->front;
    memcpy(msgOut, &temp->message, sizeof(Message)); // Copy the message out
    queue->front = queue->front->next;

    if (queue->front == NULL) 
	{
        queue->rear = NULL;
    }

    free(temp);
    pthread_mutex_unlock(&queue->lock);
    return MESSAGE_DEQUEUED; // Indicate success
}


/*
 * Function:    freeQueue
 * Description: Frees the memory associated with a message queue.
 * Parameters:  MessageQueue* queue: Pointer to the message queue structure.
 * Returns:     void
 */
void freeQueue(MessageQueue* queue) 
{
    pthread_mutex_lock(&queue->lock);

    QueueNode* current = queue->front;
    while (current != NULL) 
	{
        QueueNode* temp = current;
        current = current->next;
        
        free(temp); // Free the node itself
    }

    queue->front = NULL;
    queue->rear = NULL;
    pthread_mutex_unlock(&queue->lock);

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->cond);
}