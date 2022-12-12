#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_cycles(char *ctr, size_t *cycles, size_t *curr, size_t *capacity,
                   size_t new)
{
    if (*curr >= *capacity)
    {
        *capacity *= 2;
        cycles = realloc(cycles, *capacity * sizeof(size_t));
    }
    size_t a = *curr % 40;
    if (new - 1 == a || new == a || new + 1 == a)
        ctr[*curr] = '#';
    else
        ctr[*curr] = '.';
    cycles[*curr] = new;
    *curr += 1;
}

size_t calculate_sum(size_t *cycles, size_t capacity)
{
    size_t res = 0;
    for (size_t i = 19; i < capacity; i += 40)
    {
        res += (i + 1) * cycles[i];
    }
    return res;
}

void print_ctr(char *ctr, size_t capacity)
{
    for (size_t i = 0; i < capacity; i++)
    {
        if (i % 40 == 0)
            printf("Cycle %lu -> ", i + 1);
        printf("%c", ctr[i]);
        if ((i + 1) % 40 == 0)
            printf(" <- Cycle  %lu\n", i + 1);
    }
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

    size_t x = 1;

    size_t capacity = 240;
    size_t curr = 0;
    size_t *cycles = calloc(capacity, sizeof(size_t));
    char *ctr = calloc(capacity, sizeof(char));

    while (getline(&line, &len, f) != -1)
    {
        if (strstr(line, "addx") != NULL)
        {
            update_cycles(ctr, cycles, &curr, &capacity, x);
            update_cycles(ctr, cycles, &curr, &capacity, x);

            size_t i = 5;
            while (line[i] != '\n')
                i += 1;

            char *n = strndup(line + 5, i - 5);
            int nb = atoi(n);
            free(n);

            x += nb;
        }
        else
            update_cycles(ctr, cycles, &curr, &capacity, x);
    }
    free(line);
    fclose(f);

    size_t res = calculate_sum(cycles, capacity);

    print_ctr(ctr, capacity);

    free(cycles);
    free(ctr);
    printf("res = %lu\n", res);
    return 0;
}
