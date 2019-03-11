#include "stdio.h"
#include "stdlib.h"
#include "population.h"
#include "output.h"

struct indiv_dump {
  short spec;
  short fam;
  float x;
  float y;
};

static void dump_text(const population* pop, FILE* fout){
  struct indiv_dump indiv;
  fprintf(fout, "### STEP, POP=%d ###\n", pop->atop);
  for(int a = 0; a < pop->atop; a++){
    int i = pop->alive[a];
    indiv.x = pop->indiv[i].pos.x;
    indiv.y = pop->indiv[i].pos.y;
    indiv.fam = pop->indiv[i].family;
    indiv.spec = pop->fam[indiv.fam].species;
    fprintf(fout, "%d %d %f %f\n", indiv.spec, indiv.fam, indiv.x, indiv.y);
  }
}

static void dump_head(const int n, FILE* fout){

  char zero[8] = {0};
  fwrite(zero, 1, 8, fout);
  fwrite(&n, 1, 4, fout);
}

static void dump_bin(const population* pop, FILE* fout){
  struct indiv_dump dmp;
  dump_head(pop->atop, fout);
  for(int a = 0; a < pop->atop; a++){
    int i = pop->alive[a];
    dmp.fam = pop->indiv[i].family;
    dmp.spec = pop->fam[dmp.fam].species;
    dmp.x = pop->indiv[i].pos.x;
    dmp.y = pop->indiv[i].pos.y;
    fwrite(&dmp, 1, 12, fout);
  }
}

void dump_positions(const population* pop, FILE* fout){
  dump_bin(pop, fout);
}
