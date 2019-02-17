#ifndef SIMULATION_H
#define SIMULATION_H
#include "population.h"
#include "tools.h"
#include "field.h"

#define REPRODUCTION_CONSTANT 0.5
#define BIRTH_CONSTANT 2
#define DEATH_CONSTANT 2
#define AGING 0
#define DAMAGE_CONSTANT 0.1

void life_cycle(population* pop, config* conf);
void time_step(population* pop, config* conf, int date);

#endif
