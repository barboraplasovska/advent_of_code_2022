#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map
{
    char **arr;
    int **visited; // visited by T
    size_t nb_visited;
    size_t width; // nb columns
    size_t height; // nb rows
};

struct pos
{
    size_t x;
    size_t y;
    char symbol;
};

struct map *init_map(size_t size)
{
    struct map *m = calloc(1, sizeof(struct map));
    if (!m)
        return NULL;
    m->width = size;
    m->height = size;
    m->nb_visited = 0;
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
            m->visited[i][j] = 0;
            m->arr[i][j] = '.';
        }
    }

    return m;
}

void increase_capacity(struct map *m)
{
    size_t before = m->width;
    m->width *= 2;
    m->height *= 2;
    m->arr = realloc(m->arr, m->height * sizeof(char *));
    for (size_t i = 0; i < m->height; i++)
        m->arr[i] = realloc(m->arr[i], m->height * sizeof(char));

    for (size_t i = before; i < m->width; i++)
    {
        for (size_t j = before; j < m->width; j++)
        {
            m->arr[i][j] = '.';
            m->visited[i][j] = 0;
        }
    }
}

void print_map(struct map *map)
{
    for (size_t i = 0; i < map->width; i++)
    {
        for (size_t j = 0; j < map->height; j++)
        {
            printf("%c", map->arr[i][j]);
        }
        printf("\n");
    }
};

void print_visited(struct map *map)
{
    for (size_t i = 0; i < map->width; i++)
    {
        for (size_t j = 0; j < map->height; j++)
        {
            if (map->visited[i][j] == 1)
                printf("#");
            else
                printf(".");
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

void set_init_pos(struct map *map, struct pos pos)
{
    map->arr[pos.x][pos.y] = 'H';
    map->visited[pos.x][pos.y] = 1;
}

void update_position(struct map *map, struct pos *p, size_t x, size_t y)
{
    size_t oldx = p->x;
    size_t oldy = p->y;
    map->arr[oldx][oldy] = '.';
    map->arr[x][y] = p->symbol;
    if (p->symbol == '9' && map->visited[x][y] == 0)
    {
        map->visited[x][y] = 1;
        map->nb_visited += 1;
    }
    p->x = x;
    p->y = y;
}

// 0 - its ok
// 1 - move right
// 2 - move left
// 3 - move up
// 4 - move down
// 5 - move diagonally up left
// 6 - move diagonally up right
// 7 - move diagonally down left
// 8 - move diagonally down right
int is_tail_far(struct pos *t, struct pos *h)
{
    if ((h->x == t->x && h->y == t->y) || (h->x == t->x && h->y == t->y + 1)
        || (h->x == t->x && h->y == t->y - 1)
        || (h->x == t->x + 1 && h->y == t->y)
        || (h->x == t->x - 1 && h->y == t->y)
        || (h->x == t->x - 1 && h->y == t->y - 1)
        || (h->x == t->x + 1 && h->y == t->y + 1)
        || (h->x == t->x + 1 && h->y == t->y - 1)
        || (h->x == t->x - 1 && h->y == t->y + 1))
        return 0;

    if (h->x == t->x) // same column
    {
        if (h->y > t->y)
            return 1;
        return 2;
    }
    else if (h->y == t->y)
    {
        if (h->x > t->x)
            return 4;
        return 3;
    }

    if (h->x > t->x)
    {
        if (h->y > t->y)
            return 8;
        return 7;
    }
    if (h->y > t->y)
        return 6; // 7;
    return 5;
}

void update_tail_pos(struct map *map, struct pos *t, struct pos *h)
{
    for (size_t i = 0; i < 9; i++)
    {
        int m;
        if (i == 0)
            m = is_tail_far(&t[i], h);
        else
            m = is_tail_far(&t[i], &t[i - 1]);

        struct pos *p = &t[i];

        if (m == 0)
            continue;
        else if (m == 1)
            update_position(map, p, p->x, p->y + 1);
        else if (m == 2)
            update_position(map, p, p->x, p->y - 1);
        else if (m == 3)
            update_position(map, p, p->x - 1, p->y);
        else if (m == 4)
            update_position(map, p, p->x + 1, p->y);
        else if (m == 5)
            update_position(map, p, p->x - 1, p->y - 1);
        else if (m == 6)
            update_position(map, p, p->x - 1, p->y + 1);
        else if (m == 7)
            update_position(map, p, p->x + 1, p->y - 1);
        else if (m == 8)
            update_position(map, p, p->x + 1, p->y + 1);
    }
}

void move_right(int nb, struct map *map, struct pos *t, struct pos *h)
{
    if (nb > 0)
    {
        update_position(map, h, h->x, h->y + 1);
        update_tail_pos(map, t, h);
        move_right(nb - 1, map, t, h);
    }
}

void move_left(int nb, struct map *map, struct pos *t, struct pos *h)
{
    if (nb > 0)
    {
        update_position(map, h, h->x, h->y - 1);
        update_tail_pos(map, t, h);
        move_left(nb - 1, map, t, h);
    }
}

void move_up(int nb, struct map *map, struct pos *t, struct pos *h)
{
    if (nb > 0)
    {
        update_position(map, h, h->x - 1, h->y);
        update_tail_pos(map, t, h);
        move_up(nb - 1, map, t, h);
    }
}

void move_down(int nb, struct map *map, struct pos *t, struct pos *h)
{
    if (nb > 0)
    {
        update_position(map, h, h->x + 1, h->y);
        update_tail_pos(map, t, h);
        move_down(nb - 1, map, t, h);
    }
}

void move(char *line, struct map *map, struct pos *t, struct pos *h)
{
    size_t i = 2;
    while (line[i] != '\n')
        i += 1;
    char *n = strndup(line + 2, i - 2);
    int nb = atoi(n);
    free(n);

    if (line[0] == 'R')
        move_right(nb, map, t, h);
    else if (line[0] == 'L')
        move_left(nb, map, t, h);
    else if (line[0] == 'U')
        move_up(nb, map, t, h);
    else if (line[0] == 'D')
        move_down(nb, map, t, h);
}

void update_points(struct map *map, struct pos init, struct pos *tail)
{
    if (map->arr[init.x][init.y] == '.')
        map->arr[init.x][init.y] = 's';
    for (size_t i = 0; i < 9; i++)
    {
        if (map->arr[tail[i].x][tail[i].y] == '.')
            map->arr[tail[i].x][tail[i].y] = tail[i].symbol;
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

    struct map *map = init_map(10000);

    struct pos init = { 99, 99, 's' }; // s
    struct pos h = { 99, 99, 'H' };
    struct pos *tail = calloc(9, sizeof(struct pos));
    for (int i = 0; i < 9; i++)
    {
        tail[i].x = 99;
        tail[i].y = 99;
        tail[i].symbol = (i + 1) + '0';
    }
    set_init_pos(map, init);

    while (getline(&line, &len, f) != -1)
    {
        //        printf("\n----%s\n", line);
        move(line, map, tail, &h);
        update_points(map, init, tail);
        //      print_map(map);
        //    printf("\n");
    }
    free(line);
    fclose(f);

    // print_map(map);
    // print_visited(map);

    printf("res = %lu\n", map->nb_visited + 1);
    free_matrix(map);
    free(tail);
    return 0;
}
