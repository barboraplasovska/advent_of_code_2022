#include "helper.h"

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "node.h"

char *left = NULL;
char *right = NULL;

char *my_strdup(char *source)
{
    size_t len = 0;
    while (source[len] != '\n')
        len += 1;

    char *dest = calloc(len + 1, sizeof(char));

    size_t i = 0;
    for (; i < len; i++)
        dest[i] = source[i];

    dest[i] = '\0';

    return dest;
}

int parse_number(char *str, size_t *i)
{
    size_t start = *i;
    while (isdigit(str[*i]))
        *i += 1;

    char *nb = strndup(str + start, *i - start);
    int res = atoi(nb);

    free(nb);

    return res;
}

char *parse_number_str(char *str, size_t *i)
{
    size_t start = *i;
    while (isdigit(str[*i]))
        *i += 1;

    char *nb = strndup(str + start, *i - start);

    return nb;
}

char *convert_to_list(char *str, size_t k)
{
    size_t len = strlen(str);
    len += 2;
    char *new = calloc(len + 1, sizeof(char));
    size_t i = 0;
    for (; i < k; i++)
        new[i] = str[i];

    size_t curr = i;
    new[curr] = '[';
    curr += 1;

    char *nb = parse_number_str(str, &i);
    for (size_t j = 0; j < strlen(nb); j++, curr++)
        new[curr] = nb[j];

    new[curr] = ']';
    curr += 1;
    free(nb);

    for (size_t j = i; j < len - 2; j++, curr++)
        new[curr] = str[j];

    new[curr] = '\0';
    free(str);
    return new;
}

int compare_values(size_t *i, size_t *j)
{
    int left_nb = parse_number(left, i);
    int right_nb = parse_number(right, j);
    if (left_nb < right_nb)
        return 2; // done
    return left_nb == right_nb;
}

int compare_lists(size_t *i, size_t *j)
{
    // skipping the bracket [
    *i += 1;
    *j += 1;

    while (left[*i] != ']' && right[*j] != ']')
    {
        if (left[*i] == '[' && right[*j] == '[')
        {
            int res = compare_lists(i, j); // new list
            if (res != 1)
                return res;
        }
        else if (left[*i] == ',')
        {
            if (right[*j] != ',')
                return 0; // more elements in the left
            *i += 1;
            *j += 1;
        }
        else if (right[*j] == ',')
        {
            if (left[*i] != ',')
                return 2; // more elements in the right
            *j += 1;
        }
        else if (left[*i] == '[' && right[*j] != '[')
            right = convert_to_list(right, *j);
        else if (right[*i] == '[' && left[*i] != '[')
            left = convert_to_list(left, *i);
        else
        {
            int res = compare_values(i, j);
            if (res != 1)
                return res;
        }
    }
    if (left[*i] == ']' && right[*j] != ']')
        return 2;
    if (left[*i] != ']' && right[*j] == ']')
        return 0;
    *i += 1;
    *j += 1;
    return 1;
}

int compare(char *left_l, char *right_r)
{
    left = left_l;
    right = right_r;

    int l_list = 0;
    int r_list = 0;

    size_t i = 0;
    size_t j = 0;
    while (left[i] != '\0' && right[j] != '\0')
    {
        l_list = left[i] == '[';
        r_list = right[j] == '[';

        if (l_list && r_list)
        {
            int res = compare_lists(&i, &j);
            if (res != 1)
            {
                free(right);
                free(left);
                return res;
            }
        }
        else if (l_list && !r_list)
        {
            right = convert_to_list(right, j);
        }
        else if (!l_list && r_list)
        {
            left = convert_to_list(left, i);
        }
        else
        {
            int res = compare_values(&i, &j);
            if (res != 1)
            {
                free(right);
                free(left);
                return res;
            }
        }
    }

    free(right);
    free(left);

    return 2;
}

int compare_nodes(struct node *one, struct node *two)
{
    left = one->temp;
    right = two->temp;

    int l_list = 0;
    int r_list = 0;

    size_t i = 0;
    size_t j = 0;
    while (left[i] != '\0' && right[j] != '\0')
    {
        l_list = left[i] == '[';
        r_list = right[j] == '[';

        if (l_list && r_list)
        {
            int res = compare_lists(&i, &j);
            if (res != 1) {
                free(right);
                free(left);
                return res;
            }
        }
        else if (l_list && !r_list)
        {
            right = convert_to_list(right, j);
        }
        else if (!l_list && r_list)
        {
            left = convert_to_list(left, i);
        }
        else
        {
            int res = compare_values(&i, &j);
            if (res != 1) {
                free(right);
                free(left);
                return res;
            }
        }
    }
    free(right);
    free(left);
    return 2;
}
