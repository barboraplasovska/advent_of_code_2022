#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_contained(int a1, int b1, int a2, int b2)
{
    if (a1 >= a2 && b2 >= b1)
        return 1;
    if (a2 >= a1 && b1 >= b2)
        return 1;
    if (a2 <= b1 && a1 <= a2)
        return 1;
    if (a1 <= b2 && a2 <= a1)
        return 1;
    return 0;
}

int main(void)
{
    FILE *f = fopen("input.txt", "r");
    if (!f)
        return 1;

    size_t len = 0;
    char *line = NULL;
    int res = 0;

    while (getline(&line, &len, f) != -1)
    {
        size_t i = 0;
        while (i < len && line[i] != '-')
            i += 1;
        char *nb1 = strndup(line, i);
        int a1 = atoi(nb1);
        free(nb1);
        i += 1;

        size_t start = i;
        while (i < len && line[i] != ',')
            i += 1;

        nb1 = strndup(line+start, i - start);
        int b1 = atoi(nb1);
        free(nb1);

        i += 1;
        start = i;
        while (i < len && line[i] != '-')
            i += 1;

        nb1 = strndup(line+start, i - start);
        int a2 = atoi(nb1);
        free(nb1);

        i += 1;
        start = i;
        while (i < len && line[i] != ',')
            i += 1;

        nb1 = strndup(line+start, i - start);
        int b2 = atoi(nb1);
        free(nb1);

        res += is_contained(a1, b1, a2, b2);
        printf("line: %s", line);
        printf("overlign: %d\n\n", is_contained(a1, b1, a2, b2));
    }

    free(line);
    fclose(f);
    printf("res = %d\n", res);
    return 0;
}
