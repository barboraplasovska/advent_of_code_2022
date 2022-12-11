#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "monkey.h"

struct monkey *monkey_init(void)
{
    struct monkey *m = calloc(1, sizeof(struct monkey));
    if (!m)
        return NULL;

    m->items = NULL; // stack is empty
    m->inspected = 0;

    return m;
}

void monkey_add_item(struct monkey *m, long el)
{
    m->items = stack_push_back(m->items, el);
}

void monkey_destroy(struct monkey *m)
{
    if (!m)
        return;
    if (m->items)
        stack_destroy(m->items);
    free(m);
}

void monkey_print(struct monkey **monkeys, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        struct monkey *m = monkeys[i];
        if (!m)
            continue;
        printf("Monkey %lu:\n", i);
        printf("  Starting items: ");
        for (struct stack *p = m->items; p ; p = p->next)
        {
            printf("%lu", p->data);
            if (p->next)
                printf(", ");
        }
        printf("\n");
        printf("  Operation: old ");
        if (m->type == SQUARE)
            printf("* old\n");
        else if (m->type == ADD)
            printf("+ %lu\n", m->op_nb);
        else
            printf("* %lu\n", m->op_nb);

        printf("  Test: divisible by %lu\n", m->test_nb);
        printf("    If true: throw to monkey %lu\n", m->test_true);
        printf("    If false: throw to monkey %lu\n", m->test_false);
        printf("\n");
    }
}

void monkey_print_round(struct monkey **monkeys, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        struct monkey *m = monkeys[i];
        printf("Monkey %lu: ", i);
        for (struct stack *p = m->items; p ; p = p->next)
        {
            printf("%lu", p->data);
            if (p->next)
                printf(", ");
        }
        printf("\n");
    }
}
