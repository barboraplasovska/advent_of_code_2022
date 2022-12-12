#ifndef FILE_S_H
#define FILE_S_H

#include <stddef.h>

enum file_type
{
    FIL = 0,
    DIR
};

struct file_s
{
    enum file_type type;
    char *name;
    size_t size;
    struct file_s *parent;
    struct file_s *child; // if file => NULL
    struct file_s *prev;
    struct file_s *next;
};

struct file_s *file_s_init(char *name, enum file_type type);

void file_s_add_child(struct file_s *f, enum file_type type, char *name,
                      size_t size);

void file_s_add_sibling(struct file_s *f, enum file_type type, char *name,
                        size_t size);

void file_s_destroy(struct file_s *f);

void file_s_print(int prefix, struct file_s *f);

#endif /* FILE_S_H */
