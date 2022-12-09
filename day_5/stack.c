#include "stack.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct stack *stack_push(struct stack *s, char c)
{
    struct stack *temp = malloc(sizeof(struct stack));
    temp->data = c;
    temp->next = s;
    return temp;
}

struct stack *stack_push_back(struct stack *s, char c)
{
    if (!s)
    {
        struct stack *temp = malloc(sizeof(struct stack));
        temp->data = c;
        temp->next = NULL;
        return temp;
    }
    struct stack *head = s;
    while (s->next)
        s = s->next;
    struct stack *temp = malloc(sizeof(struct stack));
    temp->data = c;
    s->next = temp;
    temp->next = NULL;
    return head;
}

struct stack *stack_pop(struct stack *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    struct stack *temp = s;
    s = s->next;
    free(temp);
    return s;
}

char stack_peak(struct stack *s)
{
    if (!s)
        return ' ';
    return s->data;
}

void stack_print(struct stack *s)
{
    while (s)
    {
        printf("%c ", s->data);
        s = s->next;
    }
    printf("\n");
}
