#include "tools.h"
#include "config.h"
#include "population.h"
#include "field.h"
#include "simulation.h"
#include "output.h"

int main(int argc, char** argv){

  printf("%f\n", RAND());
  FILE* fout = NULL;
  if(argc > 1){
    fout = fopen(argv[1], "w");
    if(!fout){
      printf("Cannot open file.");
      exit(1);
    }
  }

  config conf;

  // Default config
  conf.spec_from_file = false;
  conf.coef_d2 = 0.00001;
  conf.dt = 0.01;
  conf.max_spec = 10;
  conf.nspec = 10;
  conf.max_fam = 200;
  conf.max_pop = 2000;
  conf.init_fam = 100;
  conf.init_fam_size = 10;
  conf.init_pop = 1000;
  conf.ressources = 1000;

  population pop;
  alloc_pop(&pop, &conf);

  init_families_full_random(&pop, &conf);
  init_pop(&pop, &conf);
  int gen = 0;
  while(pop.atop > 1){
    printf("Gen %d\n", gen);
    for(int step = 0; step < 10; step++){
      printf("      step %d, pop %d\n", step+1, pop.atop);
      if(fout){
        dump_positions(&pop, fout);
      }
      time_step(&pop, &conf, step);
    }

    life_cycle(&pop, &conf);
    gen++;
  }
  fclose(fout);
  free_pop(&pop);

  return 0;
}
