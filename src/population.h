#ifndef POPULATION_H
#define POPULATION_H
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "tools.h"

#define MAX_FAM_NUMBER 200
#define INIT_FAM_NUMBER 100
#define INIT_FAM_SIZE 10
#define INIT_POP INIT_FAM_NUMBER*INIT_FAM_SIZE
#define MAX_POP MAX_FAM_NUMBER*INIT_FAM_SIZE
#define VAR_COEF 10
#define RESOURCES INIT_POP
// Force 10 species in full random mode
#define NSPEC 10
#define BIRTH_CONSTANT 1
#define DEATH_CONSTANT 2
#define AGING 0

#define RAND() (((float) rand())/RAND_MAX)



typedef struct {
    int species;
    float aggresiveness;
    float fertility;
    float sociability;
    float vitality;
    int ressources;
} family_t;

typedef struct {
    float direction;
    float speed;
    float vitality;
    int family;
} individual;

typedef struct {
    int dtop;
    int dead[MAX_POP];
    int atop;
    int alive[MAX_POP];
    individual indiv[MAX_POP];
    family_t fam[MAX_FAM_NUMBER];
} population;

void init_families_full_random(population* pop);
void init_families_from_species(population* pop, int nspec, family_t* spec,
                                int fam_spec_map[MAX_FAM_NUMBER]);

void init_pop(population* pop);

#endif
