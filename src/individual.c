#include "population.h"

void init_pop(population* pop){

    int i;
    for(int f = 0; f < INIT_FAM_NUMBER; f++){
        for(int h = 0; h < INIT_FAM_SIZE; h++){
            i = INIT_FAM_SIZE * f + h;
            pop->indiv[i].speed.x = RAND();
            pop->indiv[i].speed.y = RAND();
            pop->indiv[i].pos.x = RAND();
            pop->indiv[i].pos.y = RAND();
            pop->indiv[i].vitality = pop->fam[f].vitality 
                                     * map1(RAND(), 0.5, 1.0);

            pop->indiv[i].family = f;
        }
        pop->fam[f].population = INIT_FAM_SIZE;
    }
    for(int f = INIT_FAM_NUMBER; f < MAX_FAM_NUMBER; f++){
        pop->fam[f].population = 0;
    }
    // Fill stacks
    pop->atop = INIT_POP - 1;
    pop->dtop = MAX_POP - INIT_POP - 1;
    for(int k = 0; k < MAX_POP; k++){
        if(k < INIT_POP){
            pop->alive[k] = k;
        } else {
            pop->alive[k] = -1;
        }
        if(MAX_POP - k < MAX_POP - INIT_POP){
            pop->dead[k] = -1;
        } else {
            pop->dead[k] = MAX_POP - k;
        }
    }
}

void init_newborn(population* pop, int i, int f){
    pop->indiv[i].speed.x = RAND();
    pop->indiv[i].speed.y = RAND();
    pop->indiv[i].vitality = pop->fam[f].vitality * map1(RAND(), 0.5, 1.0);
    pop->indiv[i].pos.x = RAND();
    pop->indiv[i].pos.y = RAND();
    pop->indiv[i].family = f;
}

/* The dead stack is kept tidy
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
*/

// Tidy up the stack
void tidy_alive_stack(population* pop){
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

// Kill given the position in the indiv array
// Used to kill in fight
void kill_indiv(population* pop, int i){
    int a = 0;  
    while(pop->alive[a] != i) a++;
    pop->alive[a] = pop->alive[pop->atop--];
    pop->fam[pop->indiv[i].family].population--;
    pop->dead[++pop->dtop] = i;
}
// Kill given the position in the stack (faster)
// Used to kill at the end of the generation
// /!\ Does not tidy up pop->alive
int kill_alive(population* pop, int a){
    int i = pop->alive[a];
    pop->alive[a] = -1;
    pop->fam[pop->indiv[i].family].population--;
    pop->dead[++pop->dtop] = i;
    return i;
}
// Kill the top of the stack (fastest) (useless ?)
/*
int kill_top(population* pop){
    if(pop->atop){
        int i = pop->alive[pop->atop--];
        pop->fam[pop->indiv[i].family].population--;
        pop->dead[++pop->dtop] = i;
        return i;
    }
    return -1;
}
*/
/*
// Birth given the position in the indiv array (useless ?)
void birth_indiv(population* pop, int i){
    int d = 0;  
    while(pop->dead[d] != i) d++;
    pop->dead[d] = pop->dead[pop->dtop--];
    pop->alive[++pop->atop] = i;
}
// Birth given the position on the stack (faster) (useless ?)
int birth_dead(population* pop, int d){
    int i = pop->dead[d];
    pop->dead[d] = pop->dead[pop->dtop--];
    pop->alive[++pop->atop] = i;
}
*/
// Birth the top of the stack (fastest) (useful)
// Used to get slots for new borns
int birth_top(population* pop){
    if(pop->atop < MAX_POP){
        int i = pop->dead[pop->dtop--];
        pop->alive[++pop->atop] = i;
        return i;
    }
    return -1;
}

