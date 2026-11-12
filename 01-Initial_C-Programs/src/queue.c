/*
 * Linked-list FIFO queue of int values.
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
    int value;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *back;
    size_t size;
} Queue;

static void queue_init(Queue *queue)
{
    if (queue == NULL) return;
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0U;
}

static int enqueue(Queue *queue, int value)
{
    if (queue == NULL) return 1;
    QueueNode *node = malloc(sizeof(*node));
    if (node == NULL) return 1;
    node->value = value;
    node->next = NULL;

    if (queue->back == NULL) queue->front = node;
    else queue->back->next = node;
    queue->back = node;
    ++queue->size;
    return 0;
}

static int dequeue(Queue *queue, int *value)
{
    if (queue == NULL || queue->front == NULL || value == NULL) return 1;
    QueueNode *removed = queue->front;
    *value = removed->value;
    queue->front = removed->next;
    if (queue->front == NULL) queue->back = NULL;
    free(removed);
    --queue->size;
    return 0;
}

static void queue_destroy(Queue *queue)
{
    if (queue == NULL) return;
    while (queue->front != NULL) {
        QueueNode *next = queue->front->next;
        free(queue->front);
        queue->front = next;
    }
    queue->back = NULL;
    queue->size = 0U;
}

int main(void)
{
    Queue queue;
    int value;
    queue_init(&queue);
    assert(enqueue(&queue, 10) == 0);
    assert(enqueue(&queue, 20) == 0);
    assert(enqueue(&queue, 30) == 0);
    assert(dequeue(&queue, &value) == 0 && value == 10);
    assert(dequeue(&queue, &value) == 0 && value == 20);
    assert(dequeue(&queue, &value) == 0 && value == 30);
    assert(dequeue(&queue, &value) == 1);
    printf("Queue drained; size=%zu\n", queue.size);
    queue_destroy(&queue);
    return 0;
}
