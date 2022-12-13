#ifndef NODE_H
#define NODE_H

#include <stddef.h>

struct node
{
    char *packet;
    char *temp;
    struct node *next;
};

struct list
{
    size_t size;
    struct node *head;
};

struct list *list_init();
size_t list_add(struct list *list, char *packet);
void list_sort(struct list *list);
void list_pop(struct list *list);
void list_destroy(struct list *list);
void list_print_packets(struct list *list);

#endif /* ! NODE_H */
