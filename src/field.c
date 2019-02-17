#include "field.h"
#include "tools.h"
#include "population.h"


short pos_to_cell(point pos){
    return (int) (CELL_NB - 1) * (((int) CELL_NB * pos.x) + pos.y);
}

void init_cell_map(cell_map* map, int max_pop){
    map->cell_content = malloc(max_pop * sizeof(cell_indiv));
    map->first_free = 0;
    for(int s = 0; s < CELL_NB; s++){
        map->stacks[s].bottom = -1;
        map->stacks[s].top = -1;
    }
}

void free_cell_map(cell_map* map){
    free(map->cell_content);
}

bool add_to_cell(cell_map* map, short cell, short indiv, int max_pop){
    int newcell = map->first_free;
    if(newcell >= 0){
        if(map->stacks[cell].bottom == -1){
            map->stacks[cell].bottom = newcell;
        }
        if(map->stacks[cell].top >= 0){
            map->cell_content[map->stacks[cell].top].next = newcell;
        }
        map->cell_content[newcell].indiv = indiv;
        map->cell_content[newcell].next = -1;
        map->stacks[cell].top = newcell;
        map->first_free = (newcell + 1 < max_pop) ? newcell + 1 : -1;
    } else {
        // error no more space
        return true;
    }
    return false;
}

void get_cell_content(cell_map* map, short* outarr, short cell){
    short next = map->stacks[cell].bottom;
    int c = 0;
    while(next != -1){
        outarr[c++] = map->cell_content[next].indiv;
        next = map->cell_content[next].next;
    }
    outarr[c] = -1;  // stop mark
}

bool fill_cell_map(population* pop, cell_map* map, int max_pop){
    for(int i = 0; i < max_pop; i++){
        short cell = pos_to_cell(pop->indiv[i].pos);
        bool err = add_to_cell(map, cell, i, max_pop);
        if(err){
            // error not enough space in cell_map
            return true;
        }
    }
    return false;
}

void move(individual* indiv, float dt){
    indiv->pos.x += dt * indiv->speed.x;
    indiv->pos.y += dt * indiv->speed.y;
    
    // bounce against walls
    if(indiv->pos.x < 0.0){
        indiv->pos.x = 0.0;
        indiv->speed.x *= -1;
    }
    if(indiv->pos.x > 1.0){
        indiv->pos.x = 1.0;
        indiv->speed.x *= -1;
    }
    if(indiv->pos.y < 0.0){
        indiv->pos.y = 0.0;
        indiv->speed.y *= -1;
    }
    if(indiv->pos.y > 1.0){
        indiv->pos.y = 1.0;
        indiv->speed.y *= -1;
    }
}
