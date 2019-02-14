#include "population.h"

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
    int spec_fam[MAX_SPEC*(MAX_FAM_NUMBER + 1)];
    for(int k = 0; k < MAX_SPEC; k++){
        spec_fam[MAX_FAM_NUMBER*k] = 0;
    }
    int dead_fam[MAX_FAM_NUMBER];
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
                            init_new_born(pop, i, nf);
                            pop->fam[nf].population += 1;
                        } else {
                            // no more space for a newborn
                        }
                    }
                } else {
                    // no more space for a new family
                }
            }
        }
    }
    // Select two families
    // create a new one with crossed features
    // 
    // Adding a mecanism to prevent buffer overflow but tracking
    // the tendency to over populate
}
