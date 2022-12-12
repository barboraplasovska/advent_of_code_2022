#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

struct pos
{
    int x;
    int y;
    int dist;
    char elev;
};

struct list
{
    struct pos *data;
    struct list *next;
};

struct queue
{
    struct list *head;
    struct list *tail;
    size_t size;
};

struct queue *queue_init(void);
size_t queue_size(struct queue *queue);
void queue_push(struct queue *queue, struct pos *elt);
struct pos *queue_head(struct queue *queue);
void queue_pop(struct queue *queue);
void queue_clear(struct queue *queue);
void queue_destroy(struct queue *queue);
void queue_print(const struct queue *queue);

#endif /* !queue_H */
