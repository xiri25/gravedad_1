#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "test.h"

void test_simulacion_cuerpos(int frames, int planetas_number) {
    printf("Iniciando el test con:\n");
    printf("    %d frames\n", frames);
    printf("    %d planetas\n", planetas_number);
    printf("    El tamaño de un planeta es: %d\n", (int)sizeof(cuerpo2d));

    int planetas_arr_size = sizeof(cuerpo2d) * frames * planetas_number;
    cuerpo2d *planetas = (cuerpo2d *)malloc(planetas_arr_size);
    if (planetas == NULL) {
        printf("Malloc failed alojando el array planetas\n");
        return;
    }
    printf("El array de planetas esta en: %p y tiene un tamaño de %d\n", (void *)planetas, planetas_arr_size);

    int planetas_t0_arr_size = sizeof(cuerpo2d) * planetas_number;
    cuerpo2d *planetas_t0 = (cuerpo2d *)malloc(planetas_t0_arr_size);
    if (planetas == NULL) {
        printf("Malloc failed alojando el array planetas_t0\n");
        return;
    }
    printf("El array de planetas_t0 esta en: %p y tiene un tamaño de %d\n", (void *)planetas_t0, planetas_t0_arr_size);
    
    //en principio vamos a usar solo dos planetas, asi que voy a hardcodear esto
    cuerpo2d Sol = {
        .m = 1000,
        .r = 1000,
        .v_x = 0,
        .v_y = 0,
        .pos_x = 0,
        .pos_y = 0
    };

    //Podemos calcular la velocidad de la Tierra suponiendo una orbital circular
    double r_orbita = 1000;
    double tierra_v_2 = 1.0 * Sol.m / r_orbita;
    double tierra_v = sqrt(tierra_v_2);

    //La velocidad es perpendicular al radio
    cuerpo2d Tierra = {
        .m = 1,
        .r = 1,
        .v_x = tierra_v,
        .v_y = 0,
        .pos_x = 0,
        .pos_y = r_orbita
    };

    planetas_t0[0] = Sol;
    planetas_t0[1] = Tierra;

    printf("Condiciones inciales listas\n");

    printf("Simulacion\n");

    cuerpos_simular(planetas, planetas_number, planetas_t0, frames, 1);

    free(planetas);
    free(planetas_t0);
}
