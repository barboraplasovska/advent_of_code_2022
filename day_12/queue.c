#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue *queue_init(void)
{
    struct queue *f = malloc(sizeof(struct queue));
    f->head = NULL;
    f->tail = NULL;
    f->size = 0;
    return f;
}

void queue_clear(struct queue *queue)
{
    struct list *p = queue->head;
    struct list *temp = NULL;
    while (p != NULL)
    {
        temp = p->next;
        free(p->data);
        free(p);
        p = temp;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

void queue_destroy(struct queue *queue)
{
    queue_clear(queue);
    free(queue);
}

size_t queue_size(struct queue *queue)
{
    return queue->size;
}

void queue_push(struct queue *queue, struct pos *elt)
{
    struct list *new = malloc(sizeof(struct list));
    new->data = elt;
    new->next = NULL;
    if (queue->head == NULL)
    {
        queue->head = new;
        queue->tail = new;
    }
    else
    {
        struct list *oldtail = queue->tail;
        queue->tail = new;
        if (oldtail)
        {
            oldtail->next = new;
        }
    }
    queue->size += 1;
}

struct pos *queue_head(struct queue *queue)
{
    return queue->head->data;
}

void queue_pop(struct queue *queue)
{
    if (queue->size > 0)
    {
        struct list *oldhead = queue->head;
        queue->head = queue->head->next;
        free(oldhead->data);
        free(oldhead);
        queue->size -= 1;
    }
}

