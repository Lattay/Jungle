#include "population.h"

void init_pop(population* pop){

    int i;
    for(int k = 0; k < INIT_FAM_NUMBER; k++){

        for(int h = 0; h < INIT_FAM_SIZE; h++){
            k = INIT_FAM_SIZE * k + h;
            pop->dead[i + INIT_POP] = -1;
            pop->alive[i] = i;
            pop->indiv[i].direction = RAND();
            pop->indiv[i].speed = RAND();
            pop->indiv[i].vitality = pop->fam[k].vitality * (0.5 + RAND() / 2);
            pop->indiv[i].family = i;
        }
    }
    for(int k = INIT_POP; k < MAX_POP; k++){
        pop->alive[k] = -1;
        pop->dead[k-INIT_POP] = k;
    }
}

/* Stack will be kept tidy
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
*/

// Kill given the position in the indiv array (useful)
void kill_indiv(population* pop, int i){
    int a = 0;  
    while(pop->alive[a] != i) a++;
    pop->alive[a] = pop->alive[pop->atop--];
    pop->dead[pop->dtop++] = i;
}
// Kill given the position in the stack (faster)
int kill_alive(population* pop, int a){
    int i = pop->alive[a];
    pop->alive[a] = pop->alive[pop->atop--];
    pop->dead[pop->dtop++] = i;
    return i;
}
// Kill the top of the stack (fastest) (useless ?)
int kill_top(population* pop){
    int i = pop->alive[pop->atop--];
    pop->dead[pop->dtop++] = i;
    return i;
}

// Birth given the position in the indiv array (useless ?)
void birth_indiv(population* pop, int i){
    int d = 0;  
    while(pop->dead[d] != i) d++;
    pop->dead[d] = pop->dead[pop->dtop--];
    pop->alive[pop->atop++] = i;
}
// Birth given the position on the stack (faster) (useless ?)
int birth_dead(population* pop, int d){
    int i = pop->dead[d];
    pop->dead[d] = pop->dead[pop->dtop--];
    pop->alive[pop->atop++] = i;
    return i;
}
// Birth the top of the stack (fastest) (useful)
int birth_top(population* pop){
    int i = pop->dead[pop->dtop--];
    pop->alive[pop->atop++] = i;
    return i;
}


void life_cycle(population* pop){
    // remove random individual => Natural death
    int a, i;
    while(pop->alive[a] > 0){
        i = pop->alive[a];

        // If AGING > 0 individual loose vitality at each generation
        // If vitality reach 0 they die
        if(AGING){
            pop->indiv[i].vitality -= AGING*RAND();
        }
        if(pop->indiv[i].vitality < RAND()*DEATH_CONSTANT){
            kill_alive(pop, a);
        }
    }
    // Select two families
    // create a new one with crossed features
    // use Verhulst like model to conpute the number of individual of the
    // new family
    // FR * Pf * (1 - P*Pf/(RS * Pf + RSf * P))
    // 
    // Adding a mecanism to prevent buffer overflow but tracking
    // the tendency to over populate
}
