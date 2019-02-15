#include "tools.h"
#include "population.h"
#include "field.h"
#include "simulation.h"

int main(int argc, char** argv){
    
    population pop;
    init_families_full_random(&pop);
    init_pop(&pop);
    int gen = 0;
    while(pop.atop > 1){
        for(int step = 0; step < 10; step++){
            printf("Generation %d, step %d", gen, step);
            time_step(&pop, step);
        }

        life_cycle(&pop);
        gen++;
    }

    return 0;
}
