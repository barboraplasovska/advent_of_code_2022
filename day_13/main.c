#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "helper.h"

void parttwo(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
        return;

    size_t len = 0;
    char *line = NULL;

    struct list *list = list_init();

    while (getline(&line, &len, f) != -1)
    {
        if (strcmp(line, "\n") == 0)
            continue;
        list_add(list, my_strdup(line));
    }

    size_t ind1 = list_add(list, my_strdup("[[2]]\n")) + 1;
    size_t ind2 = list_add(list, my_strdup("[[6]]\n")) + 1;

    // list_print_packets(list);

    printf("part 2: %lu * %lu = %lu\n", ind1, ind2, ind1 * ind2);

    free(line);
    fclose(f);
    list_destroy(list);
}

void partone(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
        return;

    size_t len = 0;
    char *line = NULL;

    int nb = 0;
    int curr = 1;

    char *left = NULL;
    char *right = NULL;
    while (getline(&line, &len, f) != -1)
    {
        if (!left)
            left = my_strdup(line);
        else if (left && !right)
            right = my_strdup(line);
        else if (strcmp(line, "\n") == 0)
        {
            int res;
            if ((res = compare(left, right)) == 2)
                nb += curr;
            curr += 1;
            right = NULL;
            left = NULL;
        }
    }

    int res;
    if ((res = compare(left, right)) == 2)
        nb += curr;

    printf("part 1: sum of indices: %d\n", nb);

    free(line);
    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main filename\n");
    }

    partone(argv[1]);

    parttwo(argv[1]);

    return 0;
}
