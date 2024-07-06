#include "three_body.h"

#include <stdio.h>
#include <stdlib.h>

void test_simulacion_three_bodies_verlet(int frames, double dt) {
    
    const int planetas_number = 4;

    int planetas_arr_size = sizeof(cuerpo2d) * frames * planetas_number;
    printf("planetas_arr_size = %d\n", planetas_arr_size);
    printf("planetas_arr_elementos = %d\n", planetas_arr_size / (int)sizeof(cuerpo2d));
    printf("frames = %d\n", frames);
    cuerpo2d* planetas3 = (cuerpo2d*)malloc(planetas_arr_size);
    if (planetas3 == NULL) {
        printf("Error malloc planetas3\n");
        return;
    }
    printf("&planetas3 = %p\n", planetas3);

    // Crear unas condiciones iniciales

    cuerpo2d Sol = {
        .m     = 110.0,
        .r     = 100.0,
        .v_x   = 0.0,
        .v_y   = 0.0,
        .pos_x = -4.0,
        .pos_y = 5.0
    };

    cuerpo2d Tierra = {
        .m     = 20.0,
        .r     = 10.0,
        .v_x   = 0.0,
        .v_y   = 1.0,
        .pos_x = 2.0,
        .pos_y = 1.0
    };

    cuerpo2d Luna = {
        .m     = 1.0,
        .r     = 1.0,
        .v_x   = 0.0,
        .v_y   = 0.1,
        .pos_x = -1.0,
        .pos_y = -8.0
    };

    cuerpo2d Marte = {
        .m     = 15.0,
        .r     = 10.0,
        .v_x   = 0.0,
        .v_y   = 1.0,
        .pos_x = 3.0,
        .pos_y = -3.0
    };
    cuerpo2d planetas_t0[4] = {Sol, Tierra, Luna, Marte};

    //cuerpos_simular_verlet(planetas3, planetas_number, planetas_t0, frames, dt);
    //cuerpos_simular(planetas3, planetas_number, planetas_t0, frames, dt);
    cuerpos_simular_euler_2(planetas3, planetas_number, planetas_t0, frames, dt);

    printf("&planetas = %p\n", planetas3);
    free(planetas3);
}
