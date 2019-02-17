#include "population.h"
#include "config.h"

// global

void alloc_pop(population* pop, config* conf){
    pop->dead = malloc(conf->max_pop*sizeof(short));
    pop->alive = malloc(conf->max_pop*sizeof(short));
    pop->fam = malloc(conf->max_fam*sizeof(family));
    pop->indiv = malloc(conf->max_pop*sizeof(individual));
}

void free_pop(population* pop){
    free(pop->dead);
    free(pop->alive);
    free(pop->fam);
    free(pop->indiv);
}

// family specific
void init_families_full_random(population* pop, config* conf){
    int nspec;
    if(conf->nspec){
        nspec = conf->nspec;
    } else {
        nspec = (int) map1(RAND(), 0.5, 1.0) * conf->max_spec;
    }
    float ag, fr, sc, vi, norm;
    for(int f = 0; f < conf->init_fam; f++){
        pop->fam[f].species = (int) map(f, 0, conf->init_fam, 0, nspec);

        ag = RAND(); fr = RAND(); sc = RAND(); vi = RAND();
        norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);

        pop->fam[f].aggresiveness = ag / norm;
        pop->fam[f].fertility = fr / norm;
        pop->fam[f].sociability = sc / norm;
        pop->fam[f].vitality = vi / norm;
        pop->fam[f].ressources = 0;
    }
}

void init_families_from_species(population* pop, config* conf, family* spec,
                                int* fam_spec_map){
    int sp;
    float ag, fr, sc, vi, norm;
    for(int f = 0; f < conf->init_fam; f++){
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

void cross_families(population* pop, int f1, int f2, int newf){
    
    float ag, fr, sc, vi, norm;
    ag = RAND(); fr = RAND(); sc = RAND(); vi = RAND();
    norm = sqrt(ag*ag + fr*fr + sc*sc + vi*vi);

    pop->fam[newf].species = pop->fam[f1].species;
    pop->fam[newf].aggresiveness = ag / norm;
    pop->fam[newf].fertility = fr / norm;
    pop->fam[newf].sociability = sc / norm;
    pop->fam[newf].vitality = vi / norm;
    pop->fam[newf].ressources = 0;
    pop->fam[newf].population = 0;
}

// individual specific
void init_pop(population* pop, config* conf){

    int i;
    for(int f = 0; f < conf->init_fam; f++){
        for(int h = 0; h < conf->init_fam_size; h++){
            i = conf->init_fam_size * f + h;
            pop->indiv[i].speed.x = RAND();
            pop->indiv[i].speed.y = RAND();
            pop->indiv[i].pos.x = RAND();
            pop->indiv[i].pos.y = RAND();
            pop->indiv[i].vitality = pop->fam[f].vitality 
                                     * map1(RAND(), 0.5, 1.0);

            pop->indiv[i].family = f;
        }
        pop->fam[f].population = conf->init_fam_size;
    }
    for(int f = conf->init_fam; f < conf->max_fam; f++){
        pop->fam[f].population = 0;
    }
    // Fill stacks
    pop->atop = conf->init_pop - 1;
    pop->dtop = conf->max_pop - conf->init_pop - 1;
    for(int k = 0; k < conf->max_pop; k++){
        if(k < conf->init_pop){
            pop->alive[k] = k;
        } else {
            pop->alive[k] = -1;
        }
        if(conf->max_pop - k < conf->max_pop - conf->init_pop){
            pop->dead[k] = -1;
        } else {
            pop->dead[k] = conf->max_pop - k;
        }
    }
}

void init_newborn(individual* indiv, family* fam, int f){
    indiv->speed.x = RAND();
    indiv->speed.y = RAND();
    indiv->vitality = fam->vitality * map1(RAND(), 0.5, 1.0);
    indiv->pos.x = RAND();
    indiv->pos.y = RAND();
    indiv->family = f;
}

// Tidy up the stack
void tidy_alive_stack(population* pop, int max_pop){
    int first_free = 0;
    int last_occupied = max_pop;
    while(first_free < last_occupied){
        while(pop->alive[first_free] >= 0 && first_free < last_occupied){
            first_free++;
        }
        while(pop->alive[last_occupied] < 0 && last_occupied > first_free){
            last_occupied--;
        }
        if(last_occupied != first_free){
            pop->alive[first_free] = pop->alive[last_occupied];
            pop->alive[last_occupied] = -1;
        }
    }
    pop->atop = first_free;
}

// Kill given the position in the stack (faster)
// Used to kill at the end of the generation
// /!\ Does not tidy up pop->alive
int kill_alive(population* pop, int a){
    int i = pop->alive[a];
    pop->alive[a] = -1;
    pop->indiv[i].vitality = -1;
    pop->fam[pop->indiv[i].family].population--;
    pop->dead[++pop->dtop] = i;
    return i;
}

// Birth the top of the stack (fastest) (useful)
// Used to get slots for new borns
int birth_top(population* pop, int max_pop){
    if(pop->atop < max_pop){
        int i = pop->dead[pop->dtop--];
        pop->alive[++pop->atop] = i;
        return i;
    }
    return -1;
}

