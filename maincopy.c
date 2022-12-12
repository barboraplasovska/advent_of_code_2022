#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    size_t res = 0;
    while (getline(&line, &len, f) != -1)
    {
        res = getmarker(line, len);
    }
    free(line);
    fclose(f);
    printf("res = %lu\n", res);
    return 0;
}
