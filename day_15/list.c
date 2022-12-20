#define _GNU_SOURCE
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list *list_init(struct pos *b, struct pos *s, long dist)
{
    struct list *l = calloc(1, sizeof(struct list));
    if (!l)
        return l;
    l->s = s;
    l->b = b;
    l->dist = dist;
    l->next = NULL;
    return l;
}

struct list *list_add(struct list *list, struct pos *b, struct pos *s,
                      long dist)
{
    struct list *new = list_init(b, s, dist);
    if (!list)
        return new;
    new->next = list;
    return new;
}

struct list *list_push_back(struct list *list, struct pos *b, struct pos *s,
                            long dist)
{
    struct list *new = list_init(b, s, dist);
    if (!list)
        return new;

    struct list *head = list;
    while (list->next)
        list = list->next;
    list->next = new;
    return head;
}

void list_destroy(struct list *list)
{
    while (list)
    {
        struct list *tmp = list->next;
        if (list->s)
            free(list->s);
        if (list->b)
            free(list->b);
        free(list);
        list = tmp;
    }
}
