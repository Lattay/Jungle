#include "population.h"

void init_pop(population* pop){

    int k;
    for(int i = 0; i < INIT_FAM_NUMBER; i++){

        for(int j = 0; j < INIT_FAM_SIZE; j++){
            pop->dead[k] = -1; // no dead at the begining
            pop->alive[k] = k; // all alive at the begining
            k = INIT_FAM_SIZE * i + j;
            pop->indiv[k].direction = RAND();
            pop->indiv[k].speed = RAND();
            pop->indiv[k].vitality = pop->fam[i].vitality * (0.5 + RAND() / 2);
            pop->indiv[k].family = i;
        }
    }
}


static void sort_index_array(int* arr, int size){
    int first_free = 0;
    int last_occupied = size;
    while(first_free < last_occupied){
        while(arr[first_free] >= 0) first_free++;
        while(arr[last_occupied] < 0) last_occupied--;
        arr[first_free] = arr[last_occupied];
        arr[last_occupied] = -1;
    }
}

void life_cycle(population* pop){
    // remove random individual => Natural death
    int k, h;
    for(int i = 0; i < INIT_POP; i++){
        if(pop->alive[i] != -1){
            k = pop->alive[i];

            // If AGING individual loose vitality at each generation
            // If vitality reach 0 they die
            if(AGING){
                pop->indiv[k].vitality -= 0.5*RAND();
            }
            if(pop->indiv[k].vitality < RAND()*DEATH_CONSTANT){
                // Death
                pop->alive[i] = -1;
                h = INIT_POP;
                while(pop->dead[h] >= 0) h--;
                pop->dead[h] = k;
            }
        }
    }
    sort_index_array(pop->dead, INIT_POP);
    // create new individual based on:
    // - family ressources
    // - global ressources
    // - available space in the population array
    // - family population
    // (global_ressources/total_pop + fam_ressources/family_pop) 
    //   * family_pop * fertility * CONSTANT
    sort_index_array(pop->alive, INIT_POP);
}
