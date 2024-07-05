#include "three_body.h"

#include <stdio.h>

void test_simulacion_three_bodies_verlet(int frames, double dt) {
    
    const int planetas_number = 3;

    int planetas_arr_size = sizeof(cuerpo2d) * frames * planetas_number;
    cuerpo2d* planetas = malloc(planetas_arr_size);
    if (planetas == NULL) {
        printf("Error malloc planetas\n");
        return;
    }

    // Crear unas condiciones iniciales

    cuerpo2d Sol = {
        .m     = 100.0,
        .r     = 100.0,
        .v_x   = 0.0,
        .v_y   = 0.0,
        .pos_x = 0.0,
        .pos_y = 0.0
    };

    cuerpo2d Tierra = {
        .m     = 10.0,
        .r     = 10.0,
        .v_x   = 0.0,
        .v_y   = 1.0,
        .pos_x = 10.0,
        .pos_y = 0.0
    };

    cuerpo2d Luna = {
        .m     = 1.0,
        .r     = 1.0,
        .v_x   = 0.0,
        .v_y   = 0.1,
        .pos_x = 11.0,
        .pos_y = 0.0
    };

    cuerpo2d planetas_t0[3] = {Sol, Tierra, Luna};

    cuerpos_simular_verlet(planetas, planetas_number, planetas_t0, frames, dt);

    for (int f = 0; f < frames; f++) {
        for (int i = 0; i < planetas_number; i++) {
            printf("%12.10f %12.10f\n",
                   planetas[f * planetas_number + i].pos_x,
                   planetas[f * planetas_number + i].pos_y);
        }
        printf("\n");
    }

    free(planetas);
}
