#include "tools.h"
#include "population.h"
#include "field.h"
#include "simulation.h"

int main(int argc, char** argv){
    
    config conf;
    
    // Default config
    conf.spec_from_file = false;
    conf.coef_d2 = 0.00001;
    conf.max_spec = 10;
    conf.nspec = 10;
    conf.max_fam = 200;
    conf.max_pop = 2000;
    conf.init_fam = 100;
    conf.init_fam_size = 10;
    conf.init_pop = 1000;
    conf.ressources = 1000;

    for(int arg = 0; arg < argc; arg++){
        printf("%s\n", argv[arg]);
    }

    population pop;
    alloc_pop(&pop, &conf);

    init_families_full_random(&pop, &conf);
    init_pop(&pop, &conf);
    int gen = 0;
    while(pop.atop > 1){
        printf("Gen %d\n", gen);
        for(int step = 0; step < 10; step++){
            printf("      step %d, pop %d\n", step+1, pop.atop);
            time_step(&pop, &conf, step);
        }

        life_cycle(&pop, &conf);
        gen++;
    }

    return 0;
}
