#include <math.h>
#include <stdio.h>

#include "setup.h"

// Necesito saber de antemano cuantas divisiones voy a hacer para alojar el array frame_division
// Un array con n divisiones necesita n+1 indices
int SETUP_divisions(sim_info *sim_info) {

    printf("Frames: %d\n", sim_info->total_iter);
    int total_size_bodies_per_frame = sim_info->body_size * sim_info->body_number_per_frame;
    int total_size_points_per_frame = sim_info->point_size * sim_info->point_number_per_frame;
    int total_size_per_frame = total_size_points_per_frame + total_size_bodies_per_frame;
    printf("total_size_per_frame: %d bytes\n", total_size_per_frame);

    int frames_per_division = (int)floor((double)sim_info->max_mem / total_size_per_frame);
    printf("Max mem por frame = %d\n", sim_info->max_mem);
    
    int frames_per_division_mod = sim_info->max_mem % total_size_per_frame;
    //printf("frames_per_division_mod = %d\n", frames_per_division_mod);

    int divisions = (int)floor((double)sim_info->total_iter / frames_per_division);
    printf("Memoria total tenida en cuenta para dividir los frames: %d\n", total_size_per_frame * sim_info->total_iter);

    return divisions + (frames_per_division_mod > 0);
}

// Esta funcion que divide los frames, debe poder ser usada para la paralelizacion
int* SETUP_frames_division_indexes(int *array, int divisions, int total) {
    int dx = total / divisions;
    int dx_mod = total % divisions;
    array[0] = 0;
    int start = 0;
    int index = 0;
    int contador = 1;

    while (divisions > 0) {
        //printf("Indice = %d\n", index);
        index = start + dx;

        if (dx_mod > 0) {
            index += 1;
            dx_mod -= 1;
        }
        //printf("Indice = %d\n", index);
        array[contador] = index - 1;
        start = index;
        divisions -= 1;
        contador += 1;
    }

    //printf("Contador = %d\n", contador);
    return array;
}
