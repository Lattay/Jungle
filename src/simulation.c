#include "tools.h"
#include "population.h"
#include "field.h"
#include "simulation.h"

void life_cycle(population* pop){
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
        if(ivit == 0.0 || ivit < fvit * RAND() * MAX_POP / (MAX_POP - tot)){
            // MAX_POP/(MAX_POP - tot) is the over population term
            kill_alive(pop, a);
            tot--;
        }
        a++;
    }
    tidy_alive_stack(pop);


    // This array is an array of stacks
    // one stack correspond to one species
    // MAX_FAM_NUMBER*s store the high of the s'th stack
    // MAX_FAM_NUMBER*s + 1... are the elements of the stack
    short spec_fam[MAX_SPEC*(MAX_FAM_NUMBER + 1)];
    for(int k = 0; k < MAX_SPEC; k++){
        spec_fam[MAX_FAM_NUMBER*k] = 0;
    }
    short dead_fam[MAX_FAM_NUMBER];
    int dead_fam_p = 0;
    for(int f = 0; f < MAX_FAM_NUMBER; f++){
        if(pop->fam[f].population > 0){
            int s = pop->fam[f].species;
            spec_fam[MAX_FAM_NUMBER*s + ++spec_fam[MAX_FAM_NUMBER*s]] = f;
        } else {
            dead_fam[dead_fam_p++] = f;
        }
    }
    
    for(int f = 0; f < MAX_FAM_NUMBER; f++){
        if(pop->fam[f].population > 0){
            if(RAND() > REPRODUCTION_CONSTANT){
                if(dead_fam_p > 0){
                    int nf = dead_fam[--dead_fam_p];
                    int s = pop->fam[f].species;
                    int fp = f;
                    int n = spec_fam[MAX_FAM_NUMBER*s];
                    // select a random family of the same species
                    while(fp == f){
                        fp = spec_fam[(int)RAND()*n + MAX_FAM_NUMBER*s];
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
                    int n_newborn = fr * popf * (
                            1 - tot * popf / (RESOURCES * popf + tot * ressf)
                    );
                    // init the newborns
                    for(int nb = 0; nb < n_newborn; nb++){
                        int i = birth_top(pop);
                        if(i >= 0){
                            init_newborn(pop, i, nf);
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


void time_step(population* pop, int date){
    cell_map cells;
    int ret = make_cell_map(pop, &cells);
    if(ret == -1){
        warning("Not enought space in cell_map !");
    }
    for(short i = 0; i < MAX_POP; i++){
        if(pop->indiv[i].vitality > 0){
            short f = pop->indiv[i].family;
            short neigbors[MAX_POP];
            short cell = pos_to_cell(pop->indiv[i].pos);
            get_cell(&cells, neigbors, cell);
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
                            mul(diff, attr/norm(diff)/COEF_D2)
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
            
            if(min_d_other*min_d_other*COEF_D2 < pop->fam[f].aggresiveness){
                // assault
                float damage = 0.3 * RAND();
                pop->fam[f].ressources += damage;
                pop->indiv[nearest_other].vitality -= damage;
            }

            // move
            move(pop, i);

        }
    }

    // Kill individuals that got negative vitality
    for(int i = 0; i < MAX_POP; i++){
        if(pop->indiv[i].vitality != -1 && pop->indiv[i].vitality <= 0){
            kill_indiv(pop, i);
            pop->indiv[i].vitality = 0;
        }
    }

}
