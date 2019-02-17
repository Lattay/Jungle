#include "tools.h"
#include "population.h"
#include "field.h"
#include "simulation.h"

void life_cycle(population* pop, config* conf){
    // remove random individual => Natural death
    int tot = pop->atop; // indirectly atop is the population counter

    int a = 0;
    while(a <= pop->atop){
        float fvit, ivit;
        int i = pop->alive[a];

        fvit = pop->fam[pop->indiv[i].family].vitality;
        // If AGING > 0 individual loose vitality at each generation
        // If vitality reach 0 they die
        if(AGING){
            pop->indiv[i].vitality -= AGING*RAND();
        }
        ivit = pop->indiv[i].vitality;
        if(ivit <= sat1(fvit * RAND() * conf->max_pop / (conf->max_pop - tot))){
            // MAX_POP/(MAX_POP - tot) is the over population term
            kill_alive(pop, a);
            pop->indiv[i].vitality = -1;
            tot--;
        }
        a++;
    }
    tidy_alive_stack(pop, conf->max_pop);


    // This array is an array of stacks
    // one stack correspond to one species
    // MAX_FAM_NUMBER*s store the high of the s'th stack
    // MAX_FAM_NUMBER*s + 1... are the elements of the stack
    short* spec_fam = malloc(conf->max_spec*(conf->max_fam + 1)*sizeof(short));
    for(int k = 0; k < conf->max_spec; k++){
        spec_fam[conf->max_fam*k] = 0;
    }
    short dead_fam[conf->max_fam];
    int dead_fam_p = 0;
    for(int f = 0; f < conf->max_fam; f++){
        if(pop->fam[f].population > 0){
            int s = pop->fam[f].species;
            spec_fam[conf->max_fam*s + ++spec_fam[conf->max_fam*s]] = f;
        } else {
            dead_fam[dead_fam_p++] = f;
        }
    }
    
    for(int f = 0; f < conf->max_fam; f++){
        if(pop->fam[f].population > 0){
            if(RAND() > REPRODUCTION_CONSTANT){
                if(dead_fam_p > 0){
                    int nf = dead_fam[--dead_fam_p];
                    int s = pop->fam[f].species;
                    int fp = f;
                    int n = spec_fam[conf->max_fam*s];
                    // select a random family of the same species
                    while(fp == f){
                        fp = spec_fam[(int)RAND()*n + conf->max_fam*s];
                    }
                    // use features of families f and fp to create a new
                    // nf family
                    cross_families(pop, f, fp, nf);
                    // compute a number of newborn for this new family
                    // Verhulst like model
                    // FR * Pf * (1 - P*Pf/(RS * Pf + RSf * P))
                    float fr = pop->fam[f].fertility;
                    int popf = pop->fam[f].population;
                    int ressf = pop->fam[f].ressources;
                    int n_newborn = fr * popf * ( 1 - tot * popf
                            / (conf->ressources * popf + tot * ressf));
                    // init the newborns
                    for(int nb = 0; nb < n_newborn; nb++){
                        int i = birth_top(pop, conf->max_pop);
                        if(i >= 0){
                            init_newborn(pop->indiv+i, pop->fam+nf, nf);
                            pop->fam[nf].population += 1;
                        } else {
                            // no more space for a newborn
                            warning("No more space for a newborn.");
                        }
                    }
                } else {
                    warning("No more space for a new family.");
                }
            }
        }
    }
}


void time_step(population* pop, config* conf, int date){
    cell_map map;
    init_cell_map(&map, conf->max_pop);
    bool err = fill_cell_map(pop, &map, conf->max_pop);
    if(err){
        warning("Not enought space in cell_map !");
    }
    short* neigbors = malloc(conf->max_pop*sizeof(short));
    for(short a = 0; a < pop->atop; a++){
        short i = pop->alive[a];
        short f = pop->indiv[i].family;
        short cell = pos_to_cell(pop->indiv[i].pos);
        get_cell_content(&map, neigbors, cell);
        int c = 0;
        int nearest_other = -1;
        float min_d_other = 2;  // distances are between 0 and 1

        // for each neighbors
        while(neigbors[c] >= 0){
            short ip = neigbors[c];
            short fp = pop->indiv[neigbors[c]].family;
            float d = norm(sub(pop->indiv[i].pos, pop->indiv[ip].pos));
            if(pop->fam[f].species == pop->fam[fp].species){
                // sum attractivity of fellow individual
                float attr = pop->fam[f].sociability / (d * d);
                point diff = sub(pop->indiv[ip].pos, pop->indiv[i].pos);
                pop->indiv[i].speed = add(
                        pop->indiv[i].speed,
                        mul(diff, attr/norm(diff)/conf->coef_d2)
                );
            } else {
                if(d < min_d_other){
                    min_d_other = d;
                    nearest_other = ip;
                }
            }
            c++;
        }

        // cap speed to 2
        float n = norm(pop->indiv[i].speed);
        if(n > 2){
            pop->indiv[i].speed = mul(pop->indiv[i].speed, 2.0/n);
        }
        
        if(min_d_other*min_d_other*conf->coef_d2 < pop->fam[f].aggresiveness){
            // assault
            float damage = 0.3 * RAND();
            pop->fam[f].ressources += damage;
            pop->indiv[nearest_other].vitality -= damage;
        }

        // move
        move(pop->indiv+i, conf->dt);
    }
    free(neigbors);
    free_cell_map(&map);

    // Kill individuals that got negative vitality
    for(short a = 0; a < pop->atop; a++){
        short i = pop->alive[a];
        if(pop->indiv[i].vitality <= 0){
            kill_alive(pop, a);
        }
    }
    tidy_alive_stack(pop, conf->max_pop);
}
