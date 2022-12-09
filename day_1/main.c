#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE *f = fopen("input.txt", "r");
    if (!f)
        return 1;

    size_t capacity = 8;
    size_t size = 0;
    int *arr = malloc(capacity * sizeof(int));
    int sum = 0;

    size_t len = 0;
    char *line = NULL;
    while (getline(&line, &len, f) != -1)
    {
        if (strcmp(line, "\n") == 0)
        {
            if (size == capacity)
            {
                capacity *= 2;
                arr = realloc(arr, capacity * sizeof(int));
            }
            arr[size] = sum;
            size += 1;
            sum = 0;
        }
        else
        {
            sum += atoi(line);
        }
    }

    if (size == capacity)
    {
        capacity *= 2;
        arr = realloc(arr, capacity * sizeof(int));
    }
    arr[size] = sum;
    size += 1;
    sum = 0;
    free(line);
    fclose(f);
    int one = 0;
    int two = 0;
    int three = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] > one)
            one = arr[i];
    }
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] > two && arr[i] < one)
            two = arr[i];
    }
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] > three && arr[i] < two)
            three = arr[i];
    }
    free(arr);
    printf("res = %d\n", one+two+three);

    return 0;
}
