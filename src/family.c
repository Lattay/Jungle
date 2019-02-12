#include "population.h"

void init_families_full_random(population* pop){
    int nspec;
    if(NSPEC){
        nspec = NSPEC;
    } else {
        nspec = (int) map1(RAND(), 0.5, 1.0) * INIT_FAM_NUMBER / VAR_COEF;
    }
    float ag, fr, sc, vi, norm;
    for(int i = 0; i < INIT_FAM_NUMBER; i++){
        pop->fam[i].species = (int) map(i, 0, INIT_FAM_NUMBER, 0, nspec);

        ag = RAND(); fr = RAND(); sc = RAND(); vi = RAND();
        norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);

        pop->fam[i].aggresiveness = ag / norm;
        pop->fam[i].fertility = fr / norm;
        pop->fam[i].sociability = sc / norm;
        pop->fam[i].vitality = vi / norm;
        pop->fam[i].ressources = 0;
    }
}

void init_families_from_species(population* pop, int nspec, family_t* spec,
                                int fam_spec_map[INIT_FAM_NUMBER]){
    int sp;
    float ag, fr, sc, vi, norm;
    for(int i = 0; i < INIT_FAM_NUMBER; i++){
        sp = fam_spec_map[i];
        ag = spec[sp].aggresiveness * map1(RAND(), 0.5, 1.5);
        fr = spec[sp].fertility * map1(RAND(), 0.5, 1.5);
        sc = spec[sp].sociability * map1(RAND(), 0.5, 1.5);
        vi = spec[sp].vitality * map1(RAND(), 0.5, 1.5);
        norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);
        pop->fam[i].aggresiveness = ag / norm;
        pop->fam[i].fertility = fr / norm;
        pop->fam[i].sociability = sc / norm;
        pop->fam[i].vitality = vi / norm;
        pop->fam[i].ressources = 0;
    }
}
