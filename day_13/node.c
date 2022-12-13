#include "node.h"

#define  _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

struct list *list_init()
{
    struct list *new = calloc(1, sizeof(struct list));
    if (!new)
        return NULL;
    new->size = 0;
    new->head = NULL;
    return new;
}

static int is_greater(struct node *one, struct node *two)
{
    int res = compare_nodes(one, two);
    // cleaning up
    one->temp = strdup(one->packet);
    two->temp = strdup(two->packet);

    return res; // 0 means one is larger, 2 means two is larger
}

size_t list_add(struct list *list, char *packet)
{
    struct node *new = malloc(sizeof(struct node));
    new->packet = packet;
    new->temp = strdup(packet);
    new->next = NULL;
    if (!list->head)
    {
        list->head = new;
        list->size += 1;
        return 0;
    }
    struct node *p = list->head;
    if (is_greater(new, p) != 0)
    {
        new->next = p;
        list->head = new;
        list->size += 1;
        return 0;
    }
    size_t i = 0;
    while (p->next && is_greater(new, p->next) == 0)
    {
        p = p->next;
        i += 1;
    }

    struct node *tmp = p->next;
    p->next = new;
    new->next = tmp;
    list->size += 1;
    return i + 1;
}

void list_pop(struct list *list)
{
    if (list->size > 0)
    {
        struct node *oldhead = list->head;
        list->head = list->head->next;
        if (oldhead->packet)
            free(oldhead->packet);
        if (oldhead->temp)
            free(oldhead->temp);
        free(oldhead);
        list->size -= 1;
    }
}

void list_destroy(struct list *list)
{
    while (list->size != 0)
        list_pop(list);
    free(list);
}


void list_print_packets(struct list *list)
{
    struct node *p = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        if (strcmp(p->packet, "[[2]]") == 0 ||
            strcmp(p->packet, "[[6]]") == 0)
            printf("%03lu: %s     <=======\n", i + 1, p->packet);
        else
            printf("%03lu: %s\n", i + 1, p->packet);

        p = p->next;
    }
}
