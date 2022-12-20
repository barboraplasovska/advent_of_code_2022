#include "valve.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct valve *valve_init(char *name, int flow_rate)
{
    struct valve *new = calloc(1, sizeof(struct valve));
    if (!new)
        return NULL;
    new->pressure = 0;
    new->open = 0;
    new->nb_valves = 0;
    new->tunnel = NULL;
    new->ind = 0;
    new->name = name;
    new->flow_rate = flow_rate;
    return new;
}

struct valve *valve_add(struct valve *valves, struct valve *new)
{
    if (!valves)
        return new;
    struct valve *head = valves;
    while (valves->next)
        valves = valves->next;
    valves->next = new;
    return head;
}
struct valve *valve_get(struct valve *valves, char *name)
{
    struct valve *p = valves;
    while (p && strcmp(p->name, name) != 0)
        p = p->next;

    return p;
}

void valve_destroy(struct valve *valves)
{
    while (valves)
    {
        struct valve *tmp = valves->next;
        if (valves->name)
            free(valves->name);
        if (valves->tunnel)
        {
            for (size_t i = 0; i < valves->nb_valves; i++)
                if (valves->tunnel[i])
                    free(valves->tunnel[i]);
            free(valves->tunnel);
        }
        free(valves);
        valves = tmp;
    }
}

void valve_print(struct valve *valves)
{
    while (valves)
    {
        printf("Valve %s (%lu): ", valves->name, valves->ind);
        for (size_t i = 0; i < valves->nb_valves; i++)
            printf("%s, ", valves->tunnel[i]);
        printf("\n");
        valves = valves->next;
    }
}
