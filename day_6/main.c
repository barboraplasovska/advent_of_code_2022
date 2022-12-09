#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_distinct(char *line, size_t j, size_t i)
{
    int h[26] = { 0 };
    for (size_t a = j; a <= i; a++)
    {
        int c = line[a] - 'a';
        h[c] += 1;
        if (h[c] > 1)
            return 0;
    }
    return 1;
}

size_t getmarker(char *line, size_t len)
{
    size_t i = 0;
    size_t count = 0;
    while (i < len && line[i] != '\n')
    {
        if (i < 13)
        {
            count += 1;
            i += 1;
            continue;
        }
        if (count != 0)
        {
            int ok = 0;
            size_t j = count > i ? 0 : i - count;
            count += 1;
            if (!is_distinct(line, j, i))
            {
                count = 13;
                ok = 1;
            }

            if (ok == 0 && count == 14)
                return i + 1;
        }
        i += 1;
    }
    return i;
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
