#ifndef SIMULATION_H
#define SIMULATION_H
#include "population.h"
#include "tools.h"
#include "field.h"

void life_cycle(population* pop, config* conf);
void time_step(population* pop, config* conf, int date);

#endif
