#ifndef SETUP
#define SETUP

#include "../cuerpos/cuerpos.h"
#include "../points/points.h"

typedef struct {
    int total_iter;
    int max_mem;
    int body_size;
    int body_number_per_frame;
    int point_size;
    int point_number_per_frame; //Deberia tener el grid ya calculado para poder poner este valor
} sim_info;

int SETUP_divisions(sim_info *sim_info);
int* SETUP_frames_division_indexes(int *array, int divisions, int total);

#endif
