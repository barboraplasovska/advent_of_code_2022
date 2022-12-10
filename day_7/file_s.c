#include "file_s.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct file_s *file_s_init(char *name, enum file_type type)
{
    struct file_s *f = calloc(1, sizeof(struct file_s));
    if (!f)
        return NULL;
    f->type = type;
    f->name = name;
    f->child = NULL;
    f->next = NULL;
    f->prev = NULL;
    f->size = 0;
    return f;
}

void file_s_add_child(struct file_s *f, enum file_type type,
        char *name, size_t size)
{
    if (!f)
        return;

    struct file_s *new = calloc(1, sizeof(struct file_s));
    if (!f)
        return;
    new->type = type;
    new->name = name;
    new->size = size;
    new->child = NULL;
    new->parent = NULL;
    new->next = NULL;
    new->prev = NULL;

    if (!f->child)
    {
        f->child = new;
        new->parent = f;
        return;
    }

    struct file_s *p = f->child;
    while (p->next)
    {
        p = p->next;
    }
    p->next = new;
    new->prev = p;
    new->parent = p->parent;
}

void file_s_add_sibling(struct file_s *f, enum file_type type,
        char *name, size_t size)
{
    if (!f)
        return;

    struct file_s *new = calloc(1, sizeof(struct file_s));
    if (!f)
        return;
    new->type = type;
    new->name = name;
    new->size = size;
    new->parent = NULL;
    new->child = NULL;
    new->next = NULL;
    new->prev = NULL;

    if (!f->next)
    {
        f->next = new;
        new->parent = f;
        return;
    }

    struct file_s *p = f->next;
    while (p->next)
    {
        p = p->next;
    }
    p->next = new;
    new->prev = p;
    new->parent = p->parent;
}

void file_s_destroy(struct file_s *f)
{
    if (f)
    {
        if (f->child)
            file_s_destroy(f->child);
        if (f->next)
            file_s_destroy(f->next);
        if (f->name)
            free(f->name);
        free(f);
    }
}

void file_s_print(int prefix, struct file_s *f)
{
    if (!f)
        return;
    while (f)
    {
        for (int i = 0; i < prefix; i++)
            printf(" ");
        if (f->type == FIL)
            printf("- %s (file, size=%lu)\n", f->name, f->size);
        else
            printf("- %s (DIR, size=%lu) \n", f->name, f->size);

        file_s_print(prefix+1, f->child);
        f = f->next;
    }
}

