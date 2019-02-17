#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

#define CELL_NB 16

typedef struct {
    // 
    bool spec_from_file;
    // space time params
    int coef_d2;
    float dt;
    // demographic params
    int max_spec;
    int nspec;
    int max_fam;
    int max_pop;
    int init_fam;
    int init_fam_size;
    int init_pop;
    int ressources;
} config;

#endif
