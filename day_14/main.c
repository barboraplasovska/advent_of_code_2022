#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

int parse_number(char *str, size_t *i)
{
    size_t start = *i;

    while (isdigit(str[*i]))
        *i += 1;

    char *nb = strndup(str + start, *i - start);
    int res = atoi(nb);
    free(nb);

    return res;
}

void parse_line(struct map *map, char *line)
{
    struct pos *old = NULL;
    struct pos *new;
    size_t i = 0;
    while (line[i] != '\n')
    {
        new = calloc(1, sizeof(struct pos));
        new->x = parse_number(line, &i);
        i += 1;
        new->y = parse_number(line, &i);

        if (line[i] != '\n')
            i += 4;
        if (!old)
        {
            old = new;
            continue;
        }
        map_add_line(map, old, new);
        if (line[i] != '\n')
        {
            free(old);
            old = new;
        }
    }
    free(old);
    free(new);
}

void add_bound(struct map *map, char c)
{
    int stone = 0;
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->arr[i][j] == '#')
                stone = i + 2;
        }
    }
    for (int j = 0; j < map->width; j++)
    {
        map->arr[stone][j] = c;
    }
}

int is_reachable(struct map *map, struct pos new)
{
    if (new.x < 0 || new.y < 0 || new.x >= map->height || new.y >= map->width)
        return 0;

    char c = map->arr[new.x][new.y];

    if (c == '-')
        return 2;

    return c == '.';
}

int solve(struct map *map, struct pos p, struct pos init)
{
    int res;
    struct pos new = { p.x + 1, p.y };
    if ((res = is_reachable(map, new)) == 1)
    {
        p.x += 1;
        return solve(map, p, init);
    }

    new.y = p.y - 1;
    if ((res = is_reachable(map, new)) == 1)
    {
        p.x += 1;
        p.y -= 1;
        return solve(map, p, init);
    }

    new.y = p.y + 1;
    if ((res = is_reachable(map, new)) == 1)
    {
        p.x += 1;
        p.y += 1;
        return solve(map, p, init);
    }

    if (res == 2)
    {
        map->arr[p.x][p.y] = 'E';
        return 2;
    }

    map->arr[p.x][p.y] = 'o';
    if (p.x == init.x && p.y == init.y)
        return 2;
    return 1;
}

void part_one(char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return;

    int height = 1000;
    int width = 1000; // ??

    struct map *map = map_init(height, width);

    size_t len = 0;
    char *line = NULL;

    while (getline(&line, &len, f) != -1)
    {
        parse_line(map, line);
    }

    add_bound(map, '-');

    struct pos init = { 0, 500 };
    map->arr[init.x][init.y] = '+'; // sand source 500 0

    int i = 0;

    while (solve(map, init, init) != 2)
        i += 1;

    // map_print(map);

    printf("part one res: %d\n", i);

    map_destroy(map);

    free(line);

    fclose(f);
}

void part_two(char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return;

    int height = 700;
    int width = 200; // ??

    struct map *map = map_init(height, width);

    size_t len = 0;
    char *line = NULL;

    while (getline(&line, &len, f) != -1)
    {
        parse_line(map, line);
    }

    add_bound(map, '#');
    // map_print(map);

    struct pos init = { 0, 500 }; //{0, 500};
    map->arr[init.x][init.y] = '+'; // sand source 500 0

    int i = 1;

    while (solve(map, init, init) != 2)
        i += 1;

    // map_print(map);

    printf("part two res: %d\n", i);

    map_destroy(map);

    free(line);

    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main filename\n");
    }

    part_one(argv[1]);

    part_two(argv[1]);

    return 0;
}
