#include "test_desmos.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * En lo que he encontrado en Desmos hay un sistema que parece estable
 * Vamos a probar como se comporta aqui
*/

void test_desmos_simulacion(int frames) {
    //Array planetas
    int planetas_arr_size = sizeof(cuerpo2d) * 2 * frames;
    cuerpo2d *planetas = (cuerpo2d*) malloc(planetas_arr_size);
    if (planetas == NULL) {
        printf("Malloc failed alojando el array planetas\n");
        return;
    }

    //Condiciones iniciales
/*
    cuerpo2d p1 = {
        .m = 20.0,
        .r = 1.0,
        .v_x = 5.0,
        .v_y = -5.0,
        .pos_x = -sqrt(2.0),
        .pos_y = -sqrt(2.0)
    };
    
    cuerpo2d p2 = {
        .m = 20.0, 
        .r = 1.0,
        .v_x = -5.0,
        .v_y = 5.0,
        .pos_x = sqrt(2.0),
        .pos_y = sqrt(2.0)
    };
*/

    
    cuerpo2d p1 = {
        .m = 2000.0,
        .r = 1.0,
        .v_x = 0.0,
        .v_y = 0.0,
        .pos_x = 0.0,
        .pos_y = 0.0
    };
    
    /*
    orbita circular
    radio v_factor
    1.0 1.0
    2.0 1.4
    3.0 1.75
    4.0 2.0
    5.0 2.25
    10.0 3.175

    v_factor(radio) = sqrt(radio)
    */

    double radio = 100.0;
    double v_orbital = sqrt(p1.m/radio);
    double v_factor = sqrt(radio);
    
    cuerpo2d p2 = {
        .m = 20.0, 
        .r = 1.0,
        .v_x = 0.0,
        //.v_y = v_orbital * v_factor,
        .v_y = sqrt(p1.m), //WTF
        .pos_x = radio,
        .pos_y = 0.0
    };
    
    cuerpo2d planetas_t0[2] = {p1, p2};

    //Simulacion
    //cuerpos_simular_verlet(planetas, 2, planetas_t0, frames, 0.01);
    cuerpos_simular_verlet_j_fijo(planetas, 2, planetas_t0, frames, 0.01);

    //Frame por frame, printf distancia
    for (int f = 0; f < frames; f++) {
        //Vector que los une
        double dx = planetas[f * 2].pos_x - planetas[f * 2 + 1].pos_x;
        double dy = planetas[f * 2].pos_y - planetas[f * 2 + 1].pos_y;
        double dist = vector2_module(dx, dy);
        //printf("Dist= %f\n", dist);
        printf("%f %f\n", planetas[f * 2].pos_x, planetas[f * 2].pos_y);
        printf("%f %f\n", planetas[f * 2 + 1].pos_x, planetas[f * 2 + 1].pos_y);
        printf("\n");
    }

    free(planetas);
}
