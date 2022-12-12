#ifndef MONKEY_H
#define MONKEY_H

#include <stddef.h>

#include "stack.h"

enum op_type
{
    ADD = 0,
    MULTI,
    SQUARE
};

struct monkey
{
    struct stack *items; // stack of items to inspect
    long test_nb; // divisible by X
    size_t test_true;
    size_t test_false;
    long op_nb; // operation number
    enum op_type type; // operation type
    size_t inspected; // nb items inspected
};

struct monkey *monkey_init(void);

void monkey_add_item(struct monkey *m, long el);

void monkey_destroy(struct monkey *m);

void monkey_print(struct monkey **monkeys, size_t count);

void monkey_print_round(struct monkey **monkeys, size_t count);

#endif /* ! MONKEY_H */
