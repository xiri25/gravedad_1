#include "three_body.h"
#include "energy.h"
#include "../gravity/cm.h"

#include <stdio.h>
#include <stdlib.h>

void test_simulacion_three_bodies_verlet(int frames, double dt) {
    
    const int planetas_number = 4;

    int planetas_arr_size = sizeof(cuerpo2d) * frames * planetas_number;
    //printf("planetas_arr_size = %d\n", planetas_arr_size);
    //printf("planetas_arr_elementos = %d\n", planetas_arr_size / (int)sizeof(cuerpo2d));
    //printf("frames = %d\n", frames);
    cuerpo2d* planetas3 = (cuerpo2d*)malloc(planetas_arr_size);
    if (planetas3 == NULL) {
        printf("Error malloc planetas3\n");
        return;
    }
    //printf("&planetas3 = %p\n", planetas3);

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
        .m     = 1.0,
        .r     = 10.0,
        .v_x   = -3.33,
        .v_y   = 0.0,
        .pos_x = 0.0,
        .pos_y = 10.0
    };

    cuerpo2d Luna = {
        .m     = 1.0,
        .r     = 1.0,
        .v_x   = sqrt(2.5),
        .v_y   = sqrt(2.5),
        .pos_x = sqrt(200),
        .pos_y = -sqrt(200)
    };

    cuerpo2d Marte = {
        .m     = 1.0,
        .r     = 10.0,
        .v_x   = sqrt(1.5),
        .v_y   = -1.0,
        .pos_x = -sqrt(1000),
        .pos_y = -sqrt(600)
    };
    cuerpo2d planetas_t0[4] = {Sol, Tierra, Luna, Marte};

    /*
    //printf("Simulacion euler empiezada\n");
    cuerpos_simular_euler_2(planetas3, planetas_number, planetas_t0, frames, dt);
    //test_mechanical_energy(planetas3, planetas_number, frames);
    //printf("Simulacion euler acabada\n");
    */

    //printf("Simulacion verlet empezada\n");
    cuerpos_simular_verlet_2(planetas3, planetas_number, planetas_t0, frames, dt);
    //test_mechanical_energy(planetas3, planetas_number, frames);
    //printf("Simulacion verlet acabada\n");

    vector2* CM = malloc(sizeof(vector2) * frames);
    if (CM == NULL) {
        printf("Error malloc CM\n");
        return;
    }

    Center_mass_calc(CM, planetas3, planetas_number, frames);

    for (int f = 0; f < frames; f++) {
        for (int p = 0; p < planetas_number; p++) {
            printf("%f %f\n",
                   planetas3[f * planetas_number + p].pos_x,
                   planetas3[f * planetas_number + p].pos_y);
        }
        printf("%f %f\n", CM[f].x, CM[f].y);
        printf("\n");
    }

    //printf("&planetas = %p\n", planetas3);
    free(planetas3);
    free(CM);
}
