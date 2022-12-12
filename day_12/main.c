#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map
{
    char **arr;
    char **visited;
    size_t nb_steps;
    size_t width;
    size_t height;
};

struct pos
{
    size_t x;
    size_t y;
    char elev;
};

struct map *init_map(size_t size)
{
    struct map *m = calloc(1, sizeof(struct map));
    if (!m)
        return NULL;
    m->width = size;
    m->height = size;
    m->nb_steps = 0;
    m->arr = calloc(m->height, sizeof(char *));
    m->visited = calloc(m->height, sizeof(int *));
    for (size_t i = 0; i < m->height; i++)
    {
        m->arr[i] = calloc(m->height, sizeof(char));
        m->visited[i] = calloc(m->height, sizeof(int));
    }

    for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->width; j++)
        {
            m->visited[i][j] = '.';
            m->arr[i][j] = '.';
        }
    }

    return m;
}

void add_to_map(char *line, struct map *map, size_t *curr, struct pos *s,
                struct pos *e)
{
    for (size_t i = 0; line[i] != '\n'; i++)
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

void print_map(char **map, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(struct map *map)
{
    if (!map)
        return;

    for (size_t i = 0; i < map->height; i++)
    {
        free(map->visited[i]);
        free(map->arr[i]);
    }
    free(map->visited);
    free(map->arr);
    free(map);
}

int is_reachable(struct map *map, size_t x, size_t y, struct pos *cp)
{
    if (x >= map->width || y >= map->height)
        return 0;
    char c = map->arr[x][y];
    return (c <= cp->elev + 1 && c >= cp->elev - 1);
}

void move_right(struct map *map, struct pos *cp)
{
    map->nb_steps += 1;
    map->visited[cp->x][cp->y] = '>';
    cp->y += 1;
}

void move_left(struct map *map, struct pos *cp)
{
    map->nb_steps += 1;
    map->visited[cp->x][cp->y] = '<';
    cp->y -= 1;
}

void move_up(struct map *map, struct pos *cp)
{
    map->nb_steps += 1;
    map->visited[cp->x][cp->y] = '^';
    cp->x -= 1;
}

void move_down(struct map *map, struct pos *cp)
{
    map->nb_steps += 1;
    map->visited[cp->x][cp->y] = 'v';
    cp->x += 1;
}

void move(struct map *map, struct pos *e, struct pos *cp)
{
    if (cp->x == e->x && cp->y == e->y)
        return;

    if (cp->x == e->x)
    {
        if (e->y < cp->y && is_reachable(map, cp->x, cp->y - 1, cp))
        {
            move_left(map, cp);
            move(map, e, cp);
        }
        else if (e->y > cp->y && is_reachable(map, cp->x, cp->y + 1, cp))
        {
            move_right(map, cp);
            move(map, e, cp);
        }
    }
    else if (cp->y == e->y)
    {
        if (e->x > cp->x && is_reachable(map, cp->x + 1, cp->y, cp))
        {
            move_down(map, cp);
            move(map, e, cp);
        }
        else if (e->x < cp->x && is_reachable(map, cp->x - 1, cp->y, cp))
        {
            move_up(map, cp);
            move(map, e, cp);
        }
    }
    else if (e->x > cp->x && is_reachable(map, cp->x + 1, cp->y, cp))
    {
        move_down(map, cp);
        move(map, e, cp);
    }
    else if (e->x < cp->x && is_reachable(map, cp->x - 1, cp->y, cp))
    {
        move_up(map, cp);
        move(map, e, cp);
    }
    else if (e->y > cp->y && is_reachable(map, cp->x, cp->y + 1, cp))
    {
        move_right(map, cp);
        move(map, e, cp);
    }
    else if (e->y < cp->y && is_reachable(map, cp->x, cp->y - 1, cp))
    {
        move_left(map, cp);
        move(map, e, cp);
    }

    move(map, e, cp);
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

    struct map *map = init_map(8);

    struct pos s;
    struct pos e;

    size_t curr = 0;

    while (getline(&line, &len, f) != -1)
    {
        add_to_map(line, map, &curr, &s, &e);
    }

    print_map(map->arr, map->width);

    s.elev = 'a';
    struct pos curr_pos = s;

    move(map, &e, &curr_pos);

    printf("\n");

    print_map(map->visited, map->width);

    printf("nb steps: %lu\n", map->nb_steps);

    free(line);
    fclose(f);
    free_matrix(map);
    return 0;
}
