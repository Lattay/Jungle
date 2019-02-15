#ifndef FIELD_H
#define FIELD_H
#include "tools.h"
#include "population.h"

short pos_to_cell(point pos);

#define CELL_NB 16
#define COEF_D2 1000
#define DT 0.0005

typedef struct {
    short indiv;
    short next;
} cell;

typedef struct {
    int first_free;
    cell cells[MAX_POP]; 
} cell_map;

short pos_to_cell(point pos);
void get_cell(cell_map* cells, short* outarr, short cell);
int set_cell(cell_map* cells, short cell, short indiv);
int make_cell_map(population* pop, cell_map* cells);

void move(population* pop, short i);

#endif
