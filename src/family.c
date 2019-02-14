#include "population.h"

void init_families_full_random(population* pop){
    int nspec;
    if(NSPEC){
        nspec = NSPEC;
    } else {
        nspec = (int) map1(RAND(), 0.5, 1.0) * MAX_SPEC;
    }
    float ag, fr, sc, vi, norm;
    for(int f = 0; f < INIT_FAM_NUMBER; f++){
        pop->fam[f].species = (int) map(f, 0, INIT_FAM_NUMBER, 0, nspec);

        ag = RAND(); fr = RAND(); sc = RAND(); vi = RAND();
        norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);

        pop->fam[f].aggresiveness = ag / norm;
        pop->fam[f].fertility = fr / norm;
        pop->fam[f].sociability = sc / norm;
        pop->fam[f].vitality = vi / norm;
        pop->fam[f].ressources = 0;
    }
}

void init_families_from_species(population* pop, int nspec, family_t* spec,
                                int fam_spec_map[INIT_FAM_NUMBER]){
    int sp;
    float ag, fr, sc, vi, norm;
    for(int f = 0; f < INIT_FAM_NUMBER; f++){
        sp = fam_spec_map[f];
        ag = spec[sp].aggresiveness * map1(RAND(), 0.5, 1.5);
        fr = spec[sp].fertility * map1(RAND(), 0.5, 1.5);
        sc = spec[sp].sociability * map1(RAND(), 0.5, 1.5);
        vi = spec[sp].vitality * map1(RAND(), 0.5, 1.5);
        norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);
        pop->fam[f].aggresiveness = ag / norm;
        pop->fam[f].fertility = fr / norm;
        pop->fam[f].sociability = sc / norm;
        pop->fam[f].vitality = vi / norm;
        pop->fam[f].ressources = 0;
    }
}
