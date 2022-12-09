#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t getmarker(char *line, size_t len)
{
    size_t i = 0;
    size_t count = 1;
    while (i < len && line[i] != '\n')
    {
        if (i < 3)
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
            for (; j < i; j++)
            {
                if (line[i] == line[j])
                {
                    count = 3;
                    ok = 1;
                    break;
                }
            }
            if (ok == 0 && count == 4)
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
