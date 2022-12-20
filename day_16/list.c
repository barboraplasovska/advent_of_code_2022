#include "list.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct list *list_init()
{
    struct list *new = calloc(1, sizeof(struct list));
    new->size = 0;
    new->capacity = 8;
    new->data = calloc(8, sizeof(char *));
    // for (size_t i = 0; i < new->capacity; i++)
    //   new->data[i] = calloc(3, sizeof(char));

    return new;
}
void list_add(struct list *list, char *new)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(char *));
    }

    list->data[list->size] = new;
    list->size += 1;
}

void list_delete(struct list *list, char *todelete)
{
    size_t i = 0;
    while (i < list->size && strcmp(list->data[i], todelete) != 0)
        i += 1;
    list->data[i] = "  ";
}

void list_destroy(struct list *list)
{
    if (!list)
        return;
    if (list->data)
        free(list->data);
    free(list);
}
