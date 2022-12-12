#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

int INFINITY = 10000000;

struct map
{
    char **arr;
    int **visited;
    int **sol;
    int nb_steps;
    int width;
    int height;
};

struct map *init_map(int width, int height)
{
    struct map *m = calloc(1, sizeof(struct map));
    if (!m)
        return NULL;
    m->width = width;
    m->height = height;
    m->nb_steps = 0;
    m->arr = calloc(m->height, sizeof(char *));
    m->visited = calloc(m->height, sizeof(int *));
    m->sol = calloc(m->height, sizeof(int *));
    for (int i = 0; i < m->height; i++)
    {
        m->arr[i] = calloc(m->width, sizeof(char));
        m->visited[i] = calloc(m->width, sizeof(int));
        m->sol[i] = calloc(m->width, sizeof(int));
    }

    for (int i = 0; i < m->height; i++)
    {
        for (int j = 0; j < m->width; j++)
        {
            m->visited[i][j] = 0;
            m->sol[i][j] = INFINITY;
            m->arr[i][j] = '.';
        }
    }

    return m;
}

void add_to_map(char *line, struct map *map, int *curr, struct pos *s,
                struct pos *e)
{
    for (int i = 0; line[i] != '\n'; i++)
    {
        if (line[i] == 'S')
        {
            s->x = *curr;
            s->y = i;
        }
        if (line[i] == 'E')
        {
            e->x = *curr;
            e->y = i;
        }
        map->arr[*curr][i] = line[i];
    }
    *curr += 1;
}

void print_map(char **map, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("%02d: ", i);
        for (int j = 0; j < width; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void print_sol(int **map, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("%02d: ", i);
        for (int j = 0; j < width; j++)
        {
            printf("%i", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_matrix(struct map *map)
{
    if (!map)
        return;

    for (int i = 0; i < map->height; i++)
    {
        free(map->visited[i]);
        free(map->arr[i]);
        free(map->sol[i]);
    }
    free(map->visited);
    free(map->arr);
    free(map->sol);
    free(map);
}

int is_reachable(struct map *map, int x, int y, struct pos *prev)
{
    if (x < 0 || y < 0 || x >= map->height || y >= map->width)
        return 0;
    char c = map->arr[x][y];
    // if (c == 'S')
    //   return 1;
    return map->visited[x][y] == 0
        && (prev->elev == 'S'
            || ((prev->elev == 'y' || prev->elev == 'z') && c == 'E')
            || (c <= prev->elev + 1 && c >= 'a')); //&& c != '-');
}

struct pos *create_pos(int x, int y, int elev, int dist)
{
    struct pos *p = calloc(1, sizeof(struct pos));
    p->x = x;
    p->y = y;
    p->dist = dist;
    p->elev = elev;
    return p;
}

int nodes_to_visit(struct map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->visited[i][j] != 1)
                return 1;
        }
    }
    return 0;
}

struct pos *get_lowest_node(struct map *map)
{
    struct pos *temp = calloc(1, sizeof(struct pos));
    temp->x = 0;
    temp->y = 0;
    temp->dist = INFINITY;
    temp->elev = 'a';
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->sol[i][j] < temp->dist && map->visited[i][j] != 1)
            {
                temp->x = i;
                temp->y = j;
                temp->dist = map->sol[i][j];
                temp->elev = map->arr[i][j];
            }
        }
    }
    if (temp->x == 0 && temp->y == 0 && map->visited[0][0] == 1)
    {
        free(temp);
        return NULL;
    }
    return temp;
}

void solve(struct map *map, struct pos *s)
{
    map->sol[s->x][s->y] = 0; // distance from src to src is 0

    while (nodes_to_visit(map))
    {
        struct pos *low = get_lowest_node(map);
        if (!low)
        {
            // printf("got stuck\n");
            return;
        }

        // printf("x: %i y: %i\n", low->x, low->y);
        if (map->arr[low->x][low->y] == 'E')
        {
            free(low);
            return;
        }

        map->visited[low->x][low->y] = 1;

        struct pos prev = *low;

        if (is_reachable(map, low->x + 1, low->y, &prev))
        {
            if (map->sol[low->x + 1][low->y] > low->dist + 1)
                map->sol[low->x + 1][low->y] = low->dist + 1;
        }
        if (is_reachable(map, low->x - 1, low->y, &prev))
        {
            if (map->sol[low->x - 1][low->y] > low->dist + 1)
                map->sol[low->x - 1][low->y] = low->dist + 1;
        }
        if (is_reachable(map, low->x, low->y + 1, &prev))
        {
            if (map->sol[low->x][low->y + 1] > low->dist + 1)
                map->sol[low->x][low->y + 1] = low->dist + 1;
        }
        if (is_reachable(map, low->x, low->y - 1, &prev))
        {
            if (map->sol[low->x][low->y - 1] > low->dist + 1)
                map->sol[low->x][low->y - 1] = low->dist + 1;
        }
        free(low);

        // print_sol(map->visited, map->width, map->height);
        // printf("\n");
    }
}

void print_final_map(struct map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        printf("%02d", i);
        for (int j = 0; j < map->width; j++)
        {
            if (map->visited[i][j] == 1
                && (map->arr[i][j] != 'S' || map->arr[i][j] != 'E'))
                printf(".");
            else
                printf("%c", map->arr[i][j]);
        }
        printf("\n");
    }
}

void reinit_map(struct map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            map->visited[i][j] = 0;
            map->sol[i][j] = INFINITY;
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

    int width = 159;
    int height = 41;

    if (strcmp(argv[1], "test.txt") == 0)
    {
        width = 8;
        height = 5;
    }

    struct map *map = init_map(width, height);

    struct pos s = { 0, 0, 0, 0 };
    struct pos e = { 0, 0, 0, 0 };

    int curr = 0;

    while (getline(&line, &len, f) != -1)
    {
        add_to_map(line, map, &curr, &s, &e);
    }

    // print_map(map->arr, map->width);

    s.elev = 'a';

    // map->sol[e.x][e.y] = 4;
    solve(map, &s);

    printf("\n");

    map->arr[s.x][s.y] = 'S';
    print_final_map(map);

    // print_map(map->visited, map->width);

    // printf("nb steps: %i\n", res);

    printf("Part 1: nb steps: %i\n", map->sol[e.x][e.y]);

    reinit_map(map);
    int size = 6;
    int *arr = calloc(size, sizeof(int));
    curr = 0;

    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->arr[i][j] == 'a')
            {
                s.x = i;
                s.y = j;
                s.dist = 0;
                s.elev = 'a';
                solve(map, &s);

                if (curr >= size)
                {
                    size *= 2;
                    arr = realloc(arr, size * sizeof(int));
                }

                arr[curr] = map->sol[e.x][e.y];
                curr += 1;

                reinit_map(map);
            }
        }
    }

    int min = arr[0];

    for (int i = 0; i < curr; i++)
    {
        if (arr[i] < min)
            min = arr[i];
    }

    printf("part 2: %d\n", min);

    free(arr);
    free(line);
    fclose(f);
    free_matrix(map);
    return 0;
}
