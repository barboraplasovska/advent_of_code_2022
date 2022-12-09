#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fill_histograms(int h1[52], char *line, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int c = 0;
        if (line[i] >= 'a' && line[i] <= 'z')
            c = line[i] - 'a';
        else if (line[i] >= 'A' && line[i] <= 'Z')
            c = line[i] - 'A' + 26;
        else
            continue;
        if (c < 0)
            return; // error
        h1[c] += 1;
    }
}

int common_items(int h1[52], int h2[52], int h3[52])
{
    int sum = 0;
    for (int i = 0; i < 52; i++)
    {
        if (h1[i] != 0 && h2[i] != 0 && h3[i] != 0)
        {
            sum += i + 1;
        }
    }
    return sum;
}

void reinit(int h1[52], int h2[52], int h3[52])
{
    for (size_t i = 0; i < 52; i++)
    {
        h1[i] = 0;
        h2[i] = 0;
        h3[i] = 0;
    }
}

int main(void)
{
    FILE *f = fopen("input.txt", "r");
    if (!f)
        return 1;

    size_t len = 0;
    char *line = NULL;
    int sum = 0;
    int h1[52] = { 0 };
    int h2[52] = { 0 };
    int h3[52] = { 0 };
    int count = 0;

    while (getline(&line, &len, f) != -1)
    {
        size_t len = strlen(line);
        if (count == 0 || count == 3)
        {
            sum += common_items(h1, h2, h3);
            count = 0;
            reinit(h1, h2, h3);
            fill_histograms(h1, line, len);
        }
        else if (count == 1)
        {
            fill_histograms(h2, line, len);
        }
        else
        { // count == 2
            fill_histograms(h3, line, len);
        }
        count += 1;
    }
    if (count == 3)
        sum += common_items(h1, h2, h3);

    free(line);
    fclose(f);
    printf("res = %d\n", sum);
    return 0;
}
