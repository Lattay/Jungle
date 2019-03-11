#ifndef POPULATION_H
#define POPULATION_H
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "config.h"
#include "tools.h"

typedef struct {
  short species;
  // features
  float aggresiveness;
  float fertility;
  float sociability;
  float vitality;
  //
  float ressources;
  short population;
} family;

typedef struct {
  point speed;
  point pos;
  float vitality;
  short family;
} individual;

typedef struct {
  int dtop;
  short* dead;
  int atop;
  short* alive;
  individual* indiv;
  family* fam;
} population;

// 
void alloc_pop(population* pop, config* conf);
void free_pop(population* pop);

// family
void init_families_full_random(population* pop, config* conf);

void init_families_from_species(population* pop, config* conf, family* spec,
    int* fam_spec_map);
void cross_families(population* pop, int f1, int f2, int newf);

// individual
void init_pop(population* pop, config* conf);
void init_newborn(individual* indiv, family* fam, int f);
void tidy_alive_stack(population* pop, int max_pop);
int kill_alive(population* pop, int a);
int birth_top(population* pop, int max_pop);

#endif
