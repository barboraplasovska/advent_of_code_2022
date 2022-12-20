#ifndef LIST_H
#define LIST_H

struct pos
{
    long x;
    long y;
};

struct list
{
    struct pos *s;
    struct pos *b;
    long dist;
    struct list *next;
};

struct list *list_init(struct pos *b, struct pos *s, long dist);

struct list *list_add(struct list *list, struct pos *b, struct pos *s,
                      long dist);

struct list *list_push_back(struct list *list, struct pos *b, struct pos *s,
                            long dist);

void list_destroy(struct list *list);

#endif /* ! LIST_H */
