#ifndef STACK_H
#define STACK_H

struct stack
{
    long data;
    struct stack *next;
};

struct stack *stack_push(struct stack *s, long c);
struct stack *stack_push_back(struct stack *s, long c);
struct stack *stack_pop(struct stack *s);
void stack_destroy(struct stack *s);
long stack_peak(struct stack *s);
void stack_print(struct stack *s);

#endif /* ! STACK_H */
