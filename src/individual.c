#include "population.h"

void init_pop(population* pop){

    int k;
    for(int i = 0; i < INIT_FAM_NUMBER; i++){

        for(int j = 0; j < INIT_FAM_SIZE; j++){
            k = INIT_FAM_SIZE * i + j;
            pop->dead[k + INIT_POP] = -1;
            pop->alive[k] = k;
            pop->indiv[k].direction = RAND();
            pop->indiv[k].speed = RAND();
            pop->indiv[k].vitality = pop->fam[i].vitality * (0.5 + RAND() / 2);
            pop->indiv[k].family = i;
        }
    }
    for(int i = INIT_POP; i < MAX_POP; i++){
        pop->alive[i] = -1;
        pop->dead[i-INIT_POP] = i;
    }
}


static void sort_dead_stack(population* pop){
    int first_free = 0;
    int last_occupied = MAX_POP;
    while(first_free <= last_occupied){
        while(pop->dead[first_free] >= 0) first_free++;
        while(pop->dead[last_occupied] < 0) last_occupied--;
        pop->dead[first_free] = pop->dead[last_occupied];
        pop->dead[last_occupied] = -1;
    }
    pop->dtop = first_free;
}

static void sort_alive_stack(population* pop){
    int first_free = 0;
    int last_occupied = MAX_POP;
    while(first_free <= last_occupied){
        while(pop->alive[first_free] >= 0) first_free++;
        while(pop->alive[last_occupied] < 0) last_occupied--;
        pop->alive[first_free] = pop->alive[last_occupied];
        pop->alive[last_occupied] = -1;
    }
    pop->atop = first_free;
}

void life_cycle(population* pop){
    // remove random individual => Natural death
    int k, h;
    sort_dead_stack(pop);
    while(pop->alive[h] > 0){
        k = pop->alive[h];

        // If AGING > 0 individual loose vitality at each generation
        // If vitality reach 0 they die
        if(AGING){
            pop->indiv[k].vitality -= AGING*RAND();
        }
        if(pop->indiv[k].vitality < RAND()*DEATH_CONSTANT){
            // Death
            pop->alive[h] = -1;
            h = INIT_POP;
            pop->dead[pop->dtop++] = k;
        }
    }
    sort_alive_stack(pop);
    // Select two families
    // create a new one with crossed features
    // use Verhulst like model to conpute the number of individual of the
    // new family
    // FR * Pf * (1 - P*Pf/(RS * Pf + RSf * P))
    // 
    // Adding a mecanism to prevent buffer overflow but tracking
    // the tendency to over populate
}
