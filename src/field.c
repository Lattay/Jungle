#include "field.h"
#include "tools.h"
#include "population.h"


short pos_to_cell(point pos){
    return (int) CELL_NB * (CELL_NB * pos.x + pos.y);
}

void get_cell(cell_map* cells, short* outarr, short cell){
    short next = cells->cells[cell].next;
    int c = 0;
    
    while(next != -1){
        next = cells->cells[next].next;
        outarr[c++] = cells->cells[next].indiv;
    }
    outarr[c] = -1;
}

int set_cell(cell_map* cells, short cell, short indiv){
    short top = cells->cells[cell].indiv;  // here indiv is the index of the top
    if(cells->first_free >= 0){
        cells->cells[top].indiv = indiv;
        cells->cells[top].next = cells->first_free;
        cells->cells[cell].indiv = cells->first_free;
        cells->first_free = cells->first_free + 1 < MAX_POP ?
                             cells->first_free + 1 : -1;
        return 0;
    } else {
        // no more space (should never happend)
        return -1;
    }
}


int make_cell_map(population* pop, cell_map* cells){
    for(int i = 0; i < MAX_POP; i++){
        short cell = pos_to_cell(pop->indiv[i].pos);
        int ret = set_cell(cells, cell, i);
        if(ret == -1){
            return -1;
        }
    }
    return 0;
}

void move(population* pop, short i){
    pop->indiv[i].pos.x += DT * pop->indiv[i].speed.x;
    pop->indiv[i].pos.y += DT * pop->indiv[i].speed.y;
    
    // bounce against walls
    if(pop->indiv[i].pos.x < 0.0){
        pop->indiv[i].pos.x = 0.0;
        pop->indiv[i].speed.x *= -1;
    }
    if(pop->indiv[i].pos.x > 1.0){
        pop->indiv[i].pos.x = 1.0;
        pop->indiv[i].speed.x *= -1;
    }
    if(pop->indiv[i].pos.y < 0.0){
        pop->indiv[i].pos.y = 0.0;
        pop->indiv[i].speed.y *= -1;
    }
    if(pop->indiv[i].pos.y > 1.0){
        pop->indiv[i].pos.y = 1.0;
        pop->indiv[i].speed.y *= -1;
    }
}
