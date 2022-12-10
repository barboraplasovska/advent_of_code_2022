#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_s.h"

void handle_cd(struct file_s **dir, char *line, size_t len)
{
    size_t i = 5;
    size_t start = 5;
    while (i < len && line[i] != '\n')
        i += 1;
    char *name = strndup(line + start, i - start);

    struct file_s *p = (*dir)->child;
    while (p && strcmp(p->name, name) != 0)
        p = p->next;

    *dir = p;

    free(name);
}

void create_dir(struct file_s *dir, char *line, size_t len)
{
    size_t i = 4;
    size_t start = i;
    while (i < len && line[i] != '\n')
        i += 1;

    char *name = strndup(line + start, i - start);

    file_s_add_child(dir, DIR, name, 0);
}


void create_file(struct file_s *dir, char *line, size_t len)
{
    size_t i = 0;
    size_t start = i;
    while (i < len && line[i] != ' ')
        i += 1;

    char *nb = strndup(line + start, i - start);
    int size = atoi(nb);
    free(nb);

    i += 1;
    start = i;
    while (i < len && line[i] != '\0' && line[i] != '\n')
        i += 1;

    char *name = strndup(line + start, i - start);

    file_s_add_child(dir, FIL, name, size);
}

size_t calculate_dir_size(struct file_s *root)
{
    if (!root)
        return 0;
    if (root->type == DIR)
    {
        struct file_s *child = root->child;
        while (child)
        {
            root->size += calculate_dir_size(child);
            child = child->next;
        }
        return root->size;
    }
    //if (root->type == FIL)
    return root->size;
}

size_t sum_dirs(struct file_s *root, size_t *count)
{
    if (!root)
        return 0;
    size_t sum = 0;
    if (root->type == DIR)
    {
        *count += 1;
        if (root->size < 100000)
            sum += root->size;
    }

    if (root->child)
        sum += sum_dirs(root->child, count);

    if (root->next)
        sum += sum_dirs(root->next, count);

    return sum;
}

void save_sizes(struct file_s *root, size_t *dir_sizes, size_t *count)
{
    if (!root || root->type != DIR)
        return;


    while (root)
    {
        if (root->type != DIR)
        {
            root = root->next;
            continue;
        }

        int i = *count - 1;
        while (i >= 0 && dir_sizes[i] > root->size)
        {
            dir_sizes[i + 1] = dir_sizes[i];
            i -= 1;
        }
        dir_sizes[i + 1] = root->size;

        *count += 1;

        if (root->child)
            save_sizes(root->child, dir_sizes, count);
        root = root->next;
    }
}

size_t get_smallest(size_t *dir_sizes, size_t count, size_t totalsize)
{
    size_t leftover = 70000000 - totalsize;

    for (size_t i = 0; i < count; i++)
    {
        if (leftover + dir_sizes[i] >= 30000000)
            return dir_sizes[i];
    }
    return totalsize;
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

    struct file_s *dir = file_s_init(strdup("/"), DIR);
    struct file_s *root = dir;
    int ls = 0;

    while (getline(&line, &len, f) != -1)
    {
        if (strstr(line, "$ cd") != NULL)
        { // changing directory
            ls = 0;
            if (strcmp(line, "$ cd ..\n") == 0)
                dir = dir->parent;
            else if (strcmp(line, "$ cd /\n") == 0)
                continue; // handled outside of the loop (init)
            else
                handle_cd(&dir, line, len);
        }
        else if (strstr(line, "$ ls") != NULL)
            ls = 1;
        else if (ls == 1)
        {
            if (strstr(line, "dir") != NULL)
                create_dir(dir, line, len);
            else
                create_file(dir, line, len);
        }
    }

//    file_s_print(0, root);

    size_t totalsize = calculate_dir_size(root);

    file_s_print(0, root);

    size_t count = 0;
    sum_dirs(root, &count);

    size_t *dir_sizes = calloc(count, sizeof(size_t));
    dir_sizes[0] = root->size;
    size_t c = 1;
    struct file_s *child = root->child;
    while (child && child->type != DIR)
        child = child->next;
    save_sizes(child, dir_sizes, &c);

    printf("\nDIR SIZES ARRAY: %lu\n", count);
    for (size_t i = 0; i < count; i ++)
        printf("%lu ", dir_sizes[i]);
    printf("\n");
    printf("total size = %lu\n", totalsize);

    size_t res = get_smallest(dir_sizes, count, totalsize);

    file_s_destroy(root);
    free(dir_sizes);
    free(line);
    fclose(f);
    printf("res = %lu\n", res);
    return 0;
}
