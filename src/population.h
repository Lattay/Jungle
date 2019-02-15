#ifndef POPULATION_H
#define POPULATION_H
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "tools.h"

#define MAX_SPEC 10
// Force 10 species in full random mode
#define NSPEC 10

#define MAX_FAM_NUMBER 200
#define INIT_FAM_NUMBER 100
#define INIT_FAM_SIZE 10
#define MAX_POP MAX_FAM_NUMBER*INIT_FAM_SIZE

#define INIT_POP INIT_FAM_NUMBER*INIT_FAM_SIZE

#define RESOURCES INIT_POP

#define REPRODUCTION_CONSTANT 0.5
#define BIRTH_CONSTANT 1
#define DEATH_CONSTANT 2
#define AGING 0

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
} family_t;

typedef struct {
    point speed;
    point pos;
    float vitality;
    short family;
} individual;

typedef struct {
    int dtop;
    short dead[MAX_POP];
    int atop;
    short alive[MAX_POP];
    individual indiv[MAX_POP];
    family_t fam[MAX_FAM_NUMBER];
} population;

// family.c
void init_families_full_random(population* pop);
void init_families_from_species(population* pop, int nspec, family_t* spec,
                                int fam_spec_map[MAX_FAM_NUMBER]);
void cross_families(population* pop, int f1, int f2, int newf);

// individual.c
void init_pop(population* pop);
void init_newborn(population* pop, int i, int f);
void tidy_alive_stack(population* pop);
void kill_indiv(population* pop, int i);
int kill_alive(population* pop, int a);
int birth_top(population* pop);

#endif
