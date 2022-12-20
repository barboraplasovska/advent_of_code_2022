#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

long myabs(long x)
{
    if (x < 0)
        return -1 * x;
    return x;
}

long parse_number(char *str, size_t *i)
{
    size_t start = *i;

    while (str[*i] == '-' || isdigit(str[*i]))
        *i += 1;

    char *nb = strndup(str + start, *i - start);
    int res = atoi(nb);
    free(nb);

    return res;
}

void parse_line(struct list **list, char *line)
{
    size_t i = 0;

    while (line[i] != '-' && !isdigit(line[i]))
        i += 1;

    long x = parse_number(line, &i);

    while (line[i] != '-' && !isdigit(line[i]))
        i += 1;

    long y = parse_number(line, &i);

    struct pos *s = calloc(1, sizeof(struct pos));
    s->x = x;
    s->y = y;

    while (line[i] != '-' && !isdigit(line[i]))
        i += 1;

    x = parse_number(line, &i);

    while (line[i] != '-' && !isdigit(line[i]))
        i += 1;

    y = parse_number(line, &i);

    struct pos *b = calloc(1, sizeof(struct pos));
    b->x = x;
    b->y = y;

    *list = list_add(*list, b, s, myabs(b->x - s->x) + myabs(b->y - s->y));
}

void get_min_max(struct list *list, long *min, long *max)
{
    while (list)
    {
        long x = list->s->x - list->dist;
        if (*min > x)
            *min = x;
        x = list->s->x + list->dist;
        if (*max < x)
            *max = x;
        list = list->next;
    }
}

long calculate_distance(struct list *list, int row)
{
    long min = 0;
    long max = 0;
    get_min_max(list, &min, &max);
    long count = 0;
    for (long x = min; x <= max; x++)
    {
        int find = 0;
        for (struct list *p = list; p; p = p->next)
        {
            if (myabs(x - p->s->x) + myabs(row - p->s->y) <= p->dist)
                find = 1;
            if (p->b->x == x && p->b->y == row)
            {
                find = 0;
                break;
            }
        }
        count += find;
    }
    return count;
}

long get_frequency(struct list *list, int maxrow)
{
    struct list *tmp = NULL;
    long res = 0;
    long freq = 0;
    for (long y = 0; y <= maxrow && freq == 0; y++)
    {
        for (struct list *p = list; p; p = p->next)
        {
            long dist = p->dist - myabs(y - p->s->y);
            if (dist >= 0)
            {
                struct pos *s = calloc(1, sizeof(struct pos));
                s->x = p->s->x - dist;
                s->y = p->s->y + dist;
                tmp = list_push_back(tmp, NULL, s, 0);
            }
        }

        int found = 0;
        long x = 0;
        for (struct list *p = tmp; p; p = p->next)
        {
            if (x >= p->s->x && x <= p->s->y)
            {
                x = p->s->y + 1;
                found = 1;
                break;
            }
        }
        if (found && x >= 0 && x <= maxrow)
        {
            freq = 4 * x + y / 1000000;
            // res = freq + y % 1000000;
            res = y > 0 ? y % res : 0;
            printf("res = 4000000 * %lu + %lu = %lu%lu\n", x, y, freq, res);
            break;
        }
    }
    list_destroy(tmp);
    return res;
}

void part_one(char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return;

    size_t len = 0;
    char *line = NULL;

    struct list *list = NULL;

    while (getline(&line, &len, f) != -1)
    {
        parse_line(&list, line);
    }

    int row = 2000000;
    if (strcmp(path, "test.txt") == 0)
        row = 10;
    long res = calculate_distance(list, row);
    printf("part one res: %lu\n", res);

    list_destroy(list);

    free(line);
    fclose(f);
}

void part_two(char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return;

    size_t len = 0;
    char *line = NULL;

    struct list *list = NULL;

    while (getline(&line, &len, f) != -1)
    {
        parse_line(&list, line);
    }

    int maxrow = 4000000;
    if (strcmp(path, "test.txt") == 0)
        maxrow = 20;

    long res = get_frequency(list, maxrow);

    // printf("part two res: %lu\n", res);

    list_destroy(list);

    free(line);
    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main filename\n");
        return 1;
    }
    part_one(argv[1]);
    // part_two(argv[1]);
}
