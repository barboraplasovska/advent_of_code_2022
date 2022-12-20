#ifndef VALVE_H
#define VALVE_H

#include <stddef.h>

struct valve
{
    char *name;
    size_t ind;
    int flow_rate;
    char **tunnel;
    size_t nb_valves;
    int pressure;
    int open;
    struct valve *next;
};

struct valve *valve_init(char *name, int flow_rate);
struct valve *valve_add(struct valve *valves, struct valve *new);
struct valve *valve_get(struct valve *valves, char *name);
void valve_destroy(struct valve *valves);
void valve_print(struct valve *valves);

#endif /* VALVE_H */
