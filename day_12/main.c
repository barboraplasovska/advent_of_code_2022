#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct map
{
    char **arr;
    char **visited;
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
            m->visited[i][j] = '.';
            m->sol[i][j] = 0;
            m->arr[i][j] = '.';
        }
    }

    return m;
}

void add_to_map(char *line, struct map *map, int *curr,
        struct pos *s, struct pos *e)
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

void print_map(char **map,int width, int height) 
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
    if (c == 'S')
        return 1;
    return prev->elev == 'S' ||
        (prev->elev == 'z' && c == 'E') ||
        (c <= prev->elev + 1 && c >= prev->elev - 1 && c != '-');
}

int solve(struct map *map, struct pos *s)
{
    struct queue *queue = queue_init();
    struct pos *s_copy = calloc(1, sizeof(struct queue));
    s_copy->x = s->x;
    s_copy->y = s->y;
    s_copy->elev = s->elev;
    s_copy->dist = s->dist;
    queue_push(queue, s_copy);
    map->arr[s->x][s->y] = '-';

    while (queue_size(queue) != 0)
    {
        struct pos *p = queue_head(queue);
        if (!p)
        {
            queue_pop(queue);
            continue;
        }

        if (map->arr[p->x][p->y] == 'E')
        {
            int dist = p->dist;
            queue_destroy(queue);
            return dist;
        }

        struct pos prev = *p;
        if (is_reachable(map, p->x + 1, p->y, &prev))
        {
            struct pos *q = calloc(1, sizeof(struct queue));
            q->x = p->x + 1;
            q->y = p->y;
            q->dist = p->dist + 1;
            q->elev = map->arr[q->x][q->y];
            queue_push(queue, q);
            printf("LETTER: %c, p.x = %d, p.y = %d, p.dist = %d\n",map->arr[q->x][q->y], q->x, q->y, q->dist);
            print_map(map->arr, map->width, map->height);
            printf("\n");
            map->arr[q->x][q->y] = '-';
        }

        if (is_reachable(map, p->x - 1, p->y, &prev))
        {
            struct pos *q = calloc(1, sizeof(struct queue));
            q->x = p->x - 1;
            q->y = p->y;
            q->dist = p->dist + 1;
            q->elev = map->arr[q->x][q->y];
            queue_push(queue, q);
            printf("LETTER: %c, p.x = %d, p.y = %d, p.dist = %d\n",map->arr[q->x][q->y], q->x, q->y, q->dist);
            print_map(map->arr, map->width, map->height);
            printf("\n");
            map->arr[q->x][q->y] = '-';
        }

        if (is_reachable(map, p->x, p->y + 1, &prev))
        {
            struct pos *q = calloc(1, sizeof(struct queue));
            q->x = p->x;
            q->y = p->y + 1;
            q->dist = p->dist + 1;
            q->elev = map->arr[q->x][q->y];
            queue_push(queue, q);
            printf("LETTER: %c, p.x = %d, p.y = %d, p.dist = %d\n",map->arr[q->x][q->y], q->x, q->y, q->dist);
            print_map(map->arr, map->width, map->height);
            printf("\n");
            map->arr[q->x][q->y] = '-';
        }

        if (is_reachable(map, p->x, p->y - 1, &prev))
        {
            struct pos *q = calloc(1, sizeof(struct queue));
            q->x = p->x;
            q->y = p->y - 1;;
            q->dist = p->dist + 1;
            q->elev = map->arr[q->x][q->y];
            queue_push(queue, q);

            printf("LETTER: %c, p.x = %d, p.y = %d, p.dist = %d\n",map->arr[q->x][q->y], q->x, q->y, q->dist);
            print_map(map->arr, map->width, map->height);
            printf("\n");

        map->arr[q->x][q->y] = '-';
        }

        queue_pop(queue);

    }
    queue_destroy(queue);
    return -1;
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

    struct map *map = init_map(159, 41);

    struct pos s = {0, 0, 0, 0};
    struct pos e = {0, 0, 0, 0};

    int curr = 0;

    while (getline(&line, &len, f) != -1)
    {
        add_to_map(line, map, &curr, &s, &e);
    }

    //print_map(map->arr, map->width);

    s.elev = 'a';

    map->sol[e.x][e.y] = 4;
    int res = solve(map, &s);

    printf("\n");

   // print_map(map->arr, map->width, map->height);

    //print_map(map->visited, map->width);

    printf("nb steps: %i\n", res);

    free(line);
    fclose(f);
    free_matrix(map);
    return 0;
}
