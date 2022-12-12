#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "monkey.h"

void parse_starting_items(struct monkey *m, char *line)
{
    size_t i = 18;
    size_t start = 18;
    while (line[i] != '\n')
    {
        start = i;
        while (line[i] != ',' && line[i] != '\n')
            i += 1;
        char *nb = strndup(line + start, i - start);
        monkey_add_item(m, atoi(nb));
        free(nb);
        if (line[i] == '\n')
            break;
        i += 2;
    }
}

void parse_operation(struct monkey *m, char *line)
{
    char c = line[23];
    if (c == '*')
        m->type = MULTI;
    else
        m->type = ADD;

    size_t i = 25;
    if (line[i] == 'o')
    {
        m->type = SQUARE;
        return;
    }

    while (line[i] != '\n')
        i += 1;

    char *nb = strndup(line + 25, i - 25);
    m->op_nb = atoi(nb);
    free(nb);
}

void parse_test(struct monkey *m, char *line)
{
    size_t i = 21;
    while (line[i] != '\n')
        i += 1;
    char *nb = strndup(line + 21, i - 21);
    m->test_nb = atoi(nb);
    free(nb);
}

void parse_test_true(struct monkey *m, char *line)
{
    size_t i = 29;
    while (line[i] != '\n')
        i += 1;
    char *nb = strndup(line + 29, i - 29);
    m->test_true = atoi(nb);
    free(nb);
}

void parse_test_false(struct monkey *m, char *line)
{
    size_t i = 30;
    while (line[i] != '\n')
        i += 1;
    char *nb = strndup(line + 30, i - 30);
    m->test_false = atoi(nb);
    free(nb);
}

long apply_worry_level(struct monkey *m, long item)
{
    if (m->type == SQUARE)
        return item * item;
    if (m->type == ADD)
        return item + m->op_nb;
    return item * m->op_nb;
}

void monkey_round(struct monkey **monkeys, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        struct monkey *m = monkeys[i];
        if (!m)
        {
            printf("Monkey %lu is NULL\n", i);
            continue;
        }
        while (m->items)
        {
            m->inspected += 1;
            long item = stack_peak(m->items);
            m->items = stack_pop(m->items);
            item = apply_worry_level(m, item);
            // item /= 3;
            item %= (long)(2 * 7 * 13 * 3 * 19 * 5 * 17 * 11);
            if (item % m->test_nb == (long)0)
            {
                monkey_add_item(monkeys[m->test_true], item);
                // if (m->test_true == 2)
                //   printf("%lu\n", i);
            }
            else
            {
                monkey_add_item(monkeys[m->test_false], item);
                // if (m->test_false == 2)
                //   printf("%lu\n", i);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main filename\n");
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
        return 1;

    size_t len = 0;
    char *line = NULL;

    struct monkey **m = calloc(8, sizeof(struct monkey *));
    size_t curr = 0;
    size_t count = 8;

    int done = 0;

    while (getline(&line, &len, f) != -1)
    {
        if (done == 1)
        {
            done = 0;
            curr += 1;
        }
        if (strstr(line, "Monkey") != NULL)
            m[curr] = monkey_init();
        else if (strstr(line, "Starting items") != NULL)
            parse_starting_items(m[curr], line);
        else if (strstr(line, "Operation") != NULL)
            parse_operation(m[curr], line);
        else if (strstr(line, "Test") != NULL)
            parse_test(m[curr], line);
        else if (strstr(line, "If true") != NULL)
            parse_test_true(m[curr], line);
        else if (strstr(line, "If false") != NULL)
            parse_test_false(m[curr], line);
        else if (strcmp("\n", line) == 0)
            done = 1;
    }

    for (size_t i = 0; i < 10000; i++) // 20 rounds
    {
        monkey_round(m, count);
        if (i == 0 || i == 19 || (i + 1) % 1000 == 0)
        {
            printf("\n== After round %lu ==\n", i + 1);
            for (size_t i = 0; i < count; i++)
                printf("Monkey %lu inspected items %lu times\n", i,
                       m[i]->inspected);
        }
    }

    /*    printf("\n\n");
        for (size_t i = 0; i < count; i++)
            printf("Monkey %lu inspected items %lu times\n", i,
       m[i]->inspected);
    */

    size_t max = 0;
    for (size_t i = 0; i < count; i++)
        if (m[i]->inspected > max)
            max = m[i]->inspected;

    size_t second = 0;
    for (size_t i = 0; i < count; i++)
        if (m[i]->inspected > second && m[i]->inspected < max)
            second = m[i]->inspected;

    printf("\nres = %lu\n", max * second);

    // monkey_print(m, count);

    for (size_t i = 0; i < count; i++)
        monkey_destroy(m[i]);
    free(m);

    free(line);
    fclose(f);
    return 0;
}
