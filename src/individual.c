#include "population.h"

void init_pop(population* pop){

    int i;
    for(int f = 0; f < INIT_FAM_NUMBER; f++){
        for(int h = 0; h < INIT_FAM_SIZE; h++){
            i = INIT_FAM_SIZE * f + h;
            pop->indiv[i].direction = RAND();
            pop->indiv[i].speed = RAND();
            pop->indiv[i].vitality = pop->fam[f].vitality * (0.5 + RAND() / 2);

            pop->indiv[i].family = f;
        }
        pop->fam[f].population = INIT_FAM_SIZE;
    }
    for(int f = INIT_FAM_NUMBER; f < MAX_FAM_NUMBER; f++){
        pop->fam[f].population = 0;
    }
    // Fill stacks
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
static void tidy_alive_stack(population* pop){
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
        }
        a++;
    }
    tidy_alive_stack(pop);

    for(int f = 0; f < MAX_FAM_NUMBER; f++){
        if(pop->fam[f].population > 0){
            
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
