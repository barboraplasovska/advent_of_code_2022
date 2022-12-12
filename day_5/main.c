#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

size_t init_stacks(struct stack ***stack_arr, size_t len)
{
    size_t nb = len / 4 - 1;
    *stack_arr = calloc(nb, sizeof(struct stack));
    return nb;
}

void add_stacks(struct stack **stack_arr, char *line, size_t len)
{
    size_t i = 0;
    while (i < len)
    {
        while (i < len && line[i] != '[')
            i += 1;
        if (i >= len)
            break;
        size_t count = i / 4;
        stack_arr[count] = stack_push_back(stack_arr[count], line[i + 1]);
        i += 1;
    }
}

void move(struct stack **stack_arr, char *line)
{
    size_t i = 5;
    size_t start = i;
    while (line[i] != ' ')
        i += 1;
    char *nb = strndup(line + start, i);
    int tomove = atoi(nb);
    free(nb);

    i += 6;
    start = i;
    while (line[i] != ' ')
        i += 1;
    nb = strndup(line + start, i - start);
    int src = atoi(nb) - 1;
    free(nb);

    i += 4;
    start = i;
    while (isdigit(line[i]))
        i += 1;
    nb = strndup(line + start, i - start);
    int dest = atoi(nb) - 1;
    free(nb);

    if (tomove == 1)
    {
        char pop = stack_peak(stack_arr[src]);
        stack_arr[src] = stack_pop(stack_arr[src]);
        stack_arr[dest] = stack_push(stack_arr[dest], pop);
    }
    else if (tomove > 1)
    {
        char *str = calloc(tomove, sizeof(char));
        for (int i = 0; i < tomove; i++)
        {
            str[i] = stack_peak(stack_arr[src]);
            stack_arr[src] = stack_pop(stack_arr[src]);
        }

        for (int i = tomove - 1; i >= 0; i--)
        {
            stack_arr[dest] = stack_push(stack_arr[dest], str[i]);
        }
    }
}

int main(void)
{
    FILE *f = fopen("input.txt", "r");
    if (!f)
        return 1;

    size_t len = 0;
    char *line = NULL;

    struct stack **stack_arr = NULL;
    size_t nb_stacks = 0;

    int init = 1;
    while (getline(&line, &len, f) != -1)
    {
        if (strcmp(line, "\n") == 0)
        {
            init = 0;
            continue;
        }
        if (init)
        {
            if (!stack_arr)
                nb_stacks = init_stacks(&stack_arr, len);
            add_stacks(stack_arr, line, len);
            printf("adding\n");
            for (size_t i = 0; i < nb_stacks; i++)
            {
                printf("%lu : ", i + 1);
                stack_print(stack_arr[i]);
            }
            printf("\n");
        }
        else
        {
            move(stack_arr, line);
            printf("%s", line);
            for (size_t i = 0; i < nb_stacks; i++)
            {
                printf("%lu : ", i + 1);
                stack_print(stack_arr[i]);
            }
            printf("\n");
        }
    }

    printf("nb stacks: %lu\n", nb_stacks);
    for (size_t i = 0; i < nb_stacks; i++)
    {
        printf("%lu : ", i + 1);
        stack_print(stack_arr[i]);
    }

    free(line);
    fclose(f);
    printf("res = ");
    for (size_t i = 0; i < nb_stacks; i++)
    {
        printf("%c", stack_peak(stack_arr[i]));
    }
    printf("\n");
    for (size_t i = 0; i < nb_stacks; i++)
    {
        free(stack_arr[i]);
    }
    return 0;
}
