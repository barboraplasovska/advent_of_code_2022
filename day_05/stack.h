#ifndef STACK_H
#define STACK_H

struct stack
{
    char data;
    struct stack *next;
};

struct stack *stack_push(struct stack *s, char c);
struct stack *stack_push_back(struct stack *s, char c);
struct stack *stack_pop(struct stack *s);
char stack_peak(struct stack *s);
void stack_print(struct stack *s);

#endif /* ! STACK_H */
