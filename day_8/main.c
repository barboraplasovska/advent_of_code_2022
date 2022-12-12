#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map
{
    int **arr;
    size_t width; // nb columns
    size_t height; // nb rows
};

void init_matrix(char *line, struct map *forest, size_t *curr)
{
    if (*curr >= forest->height)
    {
        forest->height += forest->width;
        forest->arr = realloc(forest->arr, forest->height * sizeof(int *));
    }
    for (size_t i = 0; line[i] != '\n'; i++)
    {
        int c = line[i] - '0';
        forest->arr[*curr][i] = c;
    }
    *curr += 1;
}

void print_forest(struct map *forest)
{
    for (size_t i = 0; i < forest->width; i++)
    {
        for (size_t j = 0; j < forest->height; j++)
        {
            printf("%d", forest->arr[i][j]);
        }
        printf("\n");
    }
};

void free_matrix(struct map *forest)
{
    if (!forest)
        return;

    for (size_t i = 0; i < forest->height; i++)
        free(forest->arr[i]);
    free(forest->arr);
    free(forest);
}

void red()
{
    printf("\033[1;33m");
}

void reset()
{
    printf("\033[0m");
}

int count_visible(struct map *forest)
{
    int max = 0;
    for (size_t i = 0; i < forest->width; i++)
    {
        for (size_t j = 0; j < forest->height; j++)
        {
            int me = forest->arr[i][j];

            if (i == 0 || i == forest->width - 1 || j == 0
                || j == forest->width - 1)
            {
                printf(". ");
                continue;
            }

            int visible = 1;
            int sum = 0;
            int k = i;
            do
            {
                sum += 1;
                k -= 1;
            } while (k - 1 >= 0 && forest->arr[k][j] < me);

            if (sum != 0)
                visible *= sum;
            sum = 0;

            size_t a = i;
            do
            {
                sum += 1;
                a += 1;
            } while (a + 1 < forest->width && forest->arr[a][j] < me);

            if (sum != 0)
                visible *= sum;
            sum = 0;

            k = j;
            do
            {
                sum += 1;
                k -= 1;
            } while (k - 1 >= 0 && forest->arr[i][k] < me);

            if (sum != 0)
                visible *= sum;
            sum = 0;

            a = j;
            do
            {
                sum += 1;
                a += 1;
            } while (a + 1 < forest->width && forest->arr[i][a] < me);

            if (sum != 0)
                visible *= sum;

            if (max < visible)
                max = visible;
            printf("%d ", visible);
        }
        printf("\n");
    }
    return max;
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

    struct map *forest = calloc(1, sizeof(struct map));
    forest->arr = NULL;
    forest->width = 0;
    forest->height = 0;

    size_t curr = 0;

    while (getline(&line, &len, f) != -1)
    {
        if (!forest->arr)
        {
            forest->width = strlen(line) - 1;
            forest->height = forest->width;
            forest->arr = calloc(forest->height, sizeof(int *));
            for (size_t i = 0; i < forest->height; i++)
                forest->arr[i] = calloc(forest->width, sizeof(int));
        }
        init_matrix(line, forest, &curr);
    }
    free(line);
    fclose(f);

    print_forest(forest);

    printf("\nCOUNTING VISIBLE\n");

    int res = count_visible(forest);

    free_matrix(forest);
    printf("res = %d\n", res);
    return 0;
}
