#ifndef HELPER_H
#define HELPER_H

#include "node.h"

char *my_strdup(char *source);

int parse_number(char *str, size_t *i);

char *parse_number_str(char *str, size_t *i);

char *convert_to_list(char *str, size_t k);

int compare_values(size_t *i, size_t *j);

int compare_lists(size_t *i, size_t *j);

int compare_nodes(struct node *one, struct node *two);

int compare(char *left_l, char *right_r);

#endif /* ! HELPER_H */
