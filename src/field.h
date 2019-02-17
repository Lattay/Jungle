#ifndef FIELD_H
#define FIELD_H
#include <stdbool.h>
#include "tools.h"
#include "population.h"

short pos_to_cell(point pos);

typedef struct {
    short indiv;
    short next;
} cell_indiv;

typedef struct {
    short bottom;
    short top;
} cell_stack;

typedef struct {
    int first_free;
    cell_stack stacks[CELL_NB*CELL_NB];
    cell_indiv* cell_content;
} cell_map;

short pos_to_cell(point pos);

void init_cell_map(cell_map* map, int max_pop);
void free_cell_map(cell_map* map);

bool add_to_cell(cell_map* map, short cell, short indiv, int max_pop);
void get_cell_content(cell_map* map, short* outarr, short cell);
bool fill_cell_map(population* pop, cell_map* map, int max_pop);

void move(individual* indiv, float dt);

#endif
