#ifndef MAP_H
#define MAP_H

struct pos
{
    int x;
    int y;
};

struct map
{
    char **arr;
    int width;
    int height;
};

struct map *map_init(int width, int height);

void map_print(struct map *map);

void map_destroy(struct map *map);

void map_add_line(struct map *map, struct pos *old, struct pos *new);

#endif /* ! MAP_H */
