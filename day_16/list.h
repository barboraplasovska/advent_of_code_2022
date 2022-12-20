#ifndef LIST_H
#define LIST_H

#include <stddef.h>

struct list
{
    size_t size;
    size_t capacity;
    char **data;
};

struct list *list_init();
void list_add(struct list *list, char *new);
void list_delete(struct list *list, char *todelete);
void list_destroy(struct list *list);

#endif /* LIST_H */
