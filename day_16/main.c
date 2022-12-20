#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "list.h"
#include "valve.h"

int parse_number(char *str, size_t *i)
{
    size_t start = *i;

    while (str[*i] == '-' || isdigit(str[*i]))
        *i += 1;

    char *nb = strndup(str + start, *i - start);
    int res = atoi(nb);
    free(nb);

    return res;
}

int is_caps(char c)
{
    return c >= 'A' && c <= 'Z';
}

void parse_line(struct valve **valves, char *line)
{
    size_t i = 6;
    char *name = strndup(line + i, 2);

    i = 23;
    int flow_rate = parse_number(line, &i);

    struct valve *new = valve_init(name, flow_rate);
    new->nb_valves = 0;
    new->tunnel = calloc(8, sizeof(char *));
    while (!is_caps(line[i]))
        i += 1;
    while (line[i] != '\n')
    {
        new->tunnel[new->nb_valves++] = strndup(line + i, 2);
        if (line[i + 2] == '\n')
            break;
        i += 4;
    }
    new->tunnel = realloc(new->tunnel, new->nb_valves * sizeof(char *));
    *valves = valve_add(*valves, new);
}

int **fill_paths(struct valve *valves, size_t *len)
{
    *len = 0;
    for (struct valve *p = valves; p; p = p->next)
    {
        p->ind = *len;
        *len += 1;
    }
    *len += 1;
    int **res = calloc(*len, sizeof(int *));
    for (size_t i = 0; i < *len; i++)
        res[i] = calloc(*len, sizeof(int));

    for (struct valve *from = valves; from; from = from->next)
    {
        for (struct valve *to = valves; to; to = to->next)
        {
            res[from->ind][to->ind] = *len;
        }
    }

    for (struct valve *p = valves; p; p = p->next)
    {
        res[p->ind][p->ind] = 0;
        for (size_t i = 0; i < p->nb_valves; i++)
        {
            struct valve *t = valve_get(valves, p->tunnel[i]);
            res[p->ind][t->ind] = 1;
        }
    }
    for (struct valve *p = valves; p; p = p->next)
    {
        for (struct valve *from = valves; from; from = from->next)
        {
            for (struct valve *to = valves; to; to = to->next)
            {
                if (res[from->ind][to->ind]
                    > res[from->ind][p->ind] + res[p->ind][to->ind])
                    res[from->ind][to->ind] =
                        res[from->ind][p->ind] + res[p->ind][to->ind];
            }
        }
    }
    return res;
}

int get_flow(struct valve *valves)
{
    int res = 0;
    while (valves)
    {
        if (valves->open == 1)
            res += valves->flow_rate;
        valves = valves->next;
    }
    return res;
}

int dfs(int **dist, struct valve *valves, struct valve *curr, int min,
        int total)
{
    int max = total + (30 - min) * get_flow(valves);

    for (struct valve *v = valves; v; v = v->next)
    {
        if (v->flow_rate == 0 || v->open != 0)
            continue;

        int delta = dist[curr->ind][v->ind] + 1;

        if (delta + min >= 30)
            continue;

        int new_total = total + delta * get_flow(valves);
        v->open = 1;

        int value = dfs(dist, valves, v, min + delta, new_total);
        if (max < value)
            max = value;

        v->open = 0;
    }
    return max;
}

int get_flow_open(struct list *open, struct valve *valves)
{
    int res = 0;
    for (size_t i = 0; i < open->size; i++)
    {
        if (strcmp(open->data[i], "  ") == 0)
            continue;
        struct valve *v = valve_get(valves, open->data[i]);
        res += v->flow_rate;
    }
    return res;
}

int dfs_el(int el, int **dist, struct valve *valves, struct valve *curr,
           int min, int total, struct list *open, struct list *useful)
{
    int max = total + (26 - min) * get_flow_open(open, valves);

    if (!el)
    {
        struct list *new_useful = list_init();
        for (size_t i = 0; i < useful->size; i++)
        {
            struct valve *v = valve_get(valves, useful->data[i]);
            if (!v->open)
                list_add(new_useful, useful->data[i]);
        }

        struct list *new_open = list_init();

        int max_el = dfs_el(1, dist, valves, valve_get(valves, "AA"), 0, 0,
                            new_open, new_useful);
        max = total + (26 - min) * get_flow_open(open, valves) + max_el;

        list_destroy(new_useful);
        list_destroy(new_open);
    }

    for (size_t i = 0; i < useful->size; i++)
    {
        if (strcmp(useful->data[i], "  ") == 0)
            continue;
        struct valve *v = valve_get(valves, useful->data[i]);
        if (v->flow_rate == 0 || v->open != 0)
            continue;

        int delta = dist[curr->ind][v->ind] + 1;

        if (delta + min >= 26)
            continue;

        int new_total = total + delta * get_flow_open(open, valves);
        list_add(open, v->name);
        v->open = 1;

        int value =
            dfs_el(el, dist, valves, v, min + delta, new_total, open, useful);
        if (max < value)
            max = value;
        v->open = 0;
        list_delete(open, v->name);
    }
    return max;
}

void part_one(int **dist, struct valve *valves)
{
    struct valve *curr = valve_get(valves, "AA");
    int res = dfs(dist, valves, curr, 0, 0);

    printf("part one res = %d\n", res);
}

void part_two(int **dist, struct valve *valves)
{
    struct list *open = list_init();
    struct list *useful = list_init();

    for (struct valve *p = valves; p; p = p->next)
    {
        if (p->flow_rate != 0)
            list_add(useful, p->name);
    }

    struct valve *curr = valve_get(valves, "AA");

    int res = dfs_el(0, dist, valves, curr, 0, 0, open, useful);

    list_destroy(open);
    list_destroy(useful);

    printf("part two res = %d\n", res);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main filename\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
        return 1;

    struct valve *valves = NULL;
    size_t len = 0;
    char *line = NULL;

    while (getline(&line, &len, f) != -1)
    {
        parse_line(&valves, line);
    }

    len = 0;
    int **dist = fill_paths(valves, &len);

    clock_t start, end;

    start = clock();
    part_one(dist, valves);
    end = clock();
    printf("time taken: %fs\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    start = clock();
    part_two(dist, valves);
    end = clock();
    printf("time taken: %fs\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    for (size_t i = 0; i < len; i++)
        free(dist[i]);
    free(dist);
    valve_destroy(valves);

    free(line);
    fclose(f);
    return 0;
}
