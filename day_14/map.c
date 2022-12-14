#define _GNU_SOURCE
#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map *map_init(int width, int height)
{
    struct map *m = calloc(1, sizeof(struct map));
    if (!m)
        return NULL;
    m->width = width;
    m->height = height;
    m->arr = calloc(m->height, sizeof(char *));
    for (int i = 0; i < m->height; i++)
    {
        m->arr[i] = calloc(m->width, sizeof(char));
    }

    for (int i = 0; i < m->height; i++)
    {
        for (int j = 0; j < m->width; j++)
        {
            m->arr[i][j] = '.';
        }
    }

    return m;
}

void map_print(struct map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        printf("%03d: ", i);
        for (int j = 0; j < map->width; j++)
        {
            printf("%c", map->arr[i][j]);
        }
        printf("\n");
    }
}

void map_destroy(struct map *map)
{
    if (!map)
        return;

    for (int i = 0; i < map->height; i++)
    {
        free(map->arr[i]);
    }
    free(map->arr);
    free(map);
}

void map_add_line(struct map *map, struct pos *old, struct pos *new)
{
    int same_x = old->x == new->x;
    int same_y = old->y == new->y;

    if (same_x && same_y)
        map->arr[old->x][old->y] = '#';
    else if (same_x)
    {
        int lower = old->y > new->y ? new->y : old->y;
        int higher = old->y < new->y ? new->y : old->y;
        for (int i = lower; i <= higher; i++)
            map->arr[i][old->x] = '#';
    }
    else if (same_y)
    {
        int lower = old->x > new->x ? new->x : old->x;
        int higher = old->x < new->x ? new->x : old->x;
        for (int i = lower; i <= higher; i++)
            map->arr[old->y][i] = '#';
    }
}
