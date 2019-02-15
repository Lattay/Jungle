#ifndef SIMULATION_H
#define SIMULATION_H
#include "population.h"
#include "tools.h"
#include "field.h"

void life_cycle(population* pop);
void time_step(population* pop, int date);

#endif
