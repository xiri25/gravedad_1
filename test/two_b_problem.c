// Vamos a probar con distintas condiciones inicialess para poder determinar la fuente de nuestros errores

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "two_b_problem.h"

// ESTA FUNCION ALOJA EL ARRAY DE LAS DISTANCIAS, PERO NO LO LIBERA
double *dist_verlet_2_body(cuerpo2d *planetas_t0, int frames, double dt) {
    /*
    printf("Iniciando el test con:\n");
    printf("    %d frames\n", frames);
    printf("    %d planetas\n", planetas_number);
    printf("    El tamaño de un planeta es: %d\n", (int)sizeof(cuerpo2d));
    */

    int planetas_arr_size = sizeof(cuerpo2d) * frames * 2;
    cuerpo2d *planetas = (cuerpo2d *)malloc(planetas_arr_size);
    if (planetas == NULL) {
        printf("Malloc failed alojando el array planetas\n");
        return NULL;
    }
    //printf("El array de planetas esta en: %p y tiene un tamaño de %d\n", (void *)planetas, planetas_arr_size);

    //printf("El array de planetas_t0 esta en: %p y tiene un tamaño de %d\n", (void *)planetas_t0, planetas_t0_arr_size);
    
    /*
    printf("Condiciones inciales listas\n");

    printf("Simulacion\n");
*/
    cuerpos_simular_verlet(planetas, 2, planetas_t0, frames, dt);

    //Es cutre pero voy a recalcular aqui las distancias por no tocar mucho el codigo ya escrito
    int dists_len = frames;
    int dists_size = sizeof(double) * dists_len;
    double *dists = (double*)malloc(dists_size);

    // Solo hay dos planetas, la distancia entre ellos se calcula una vez por frame
    for (int f = 0; f < frames; f++) {

        double dx = planetas[f * 2].pos_x - planetas[f * 2 + 1].pos_x;
        double dy = planetas[f * 2].pos_y - planetas[f * 2 + 1].pos_y;
        
        dists[f] = vector2_module(dx, dy);
    }

    free(planetas);
    
    return dists;
}

// DEVUELVE UN ARRAY ALOJADO, NO LO LIBERA
cuerpo2d *condiciones_iniciales_2_body(double r_orbita, double v_factor) {

    int planetas_t0_arr_size = sizeof(cuerpo2d) * 2;
    cuerpo2d *planetas_t0 = (cuerpo2d *)malloc(planetas_t0_arr_size);
    if (planetas_t0 == NULL) {
        printf("Malloc failed alojando el array planetas_t0\n");
        return NULL;
    }

    //en principio vamos a usar solo dos planetas, asi que voy a hardcodear esto
    cuerpo2d Sol = {
        .m = 100.0,
        .r = 100.0,
        .v_x = 0.0,
        .v_y = 0.0,
        .pos_x = 0.0,
        .pos_y = 0.0
    };

    //Podemos calcular la velocidad de la Tierra suponiendo una orbital circular
    double tierra_v_2 = 1.0 * Sol.m / r_orbita;
    double tierra_v = sqrt(tierra_v_2);

    //La velocidad es perpendicular al radio
    cuerpo2d Tierra = {
        .m = 1.0,
        .r = 1.0,
        .v_x = tierra_v * v_factor,
        .v_y = 0.0,
        .pos_x = 0.0,
        .pos_y = r_orbita
    };

    planetas_t0[0] = Sol;
    planetas_t0[1] = Tierra;

    return planetas_t0;
}

// Devuelve un array de resultados alojado, hay que liberarlo.
// Los arrays con las distancias son devueltos, por lo que no se liberan.
test_result *test_two_b_p_v_factor(double *v_factors, int v_factors_len) {

    //Vamos a empezar variando el factor de la velocidad
    double orbita_radio = 100;

    // Con un dt bajito para mayor precision y muchos frames para sacar tener mas datos
    double dt = 0.1;
    int frames = 200000;

    test_result *resultados = (test_result*)malloc(sizeof(test_result) * v_factors_len);
    if (resultados == NULL) {
        printf("Error alojando el array para los resultados\n");
        return NULL;
    }

    //Variamos el factor velocidad
    for (int i = 0; i < v_factors_len; i++) {
        cuerpo2d *cond_iniciales = condiciones_iniciales_2_body(orbita_radio, v_factors[i]);
        double *dists = dist_verlet_2_body(cond_iniciales, frames, dt);

        free(cond_iniciales);
        // Hacer los calculos
        // Empezamos por calcular la desviacion de la orbita
        for (int f = 0; f < frames; f++) {
            dists[f] = dists[f] - orbita_radio;
        }

        //Calculamos el minimo, maximo y la media de las diferencias de distancias
        double min = DBL_MAX;
        for (int f = 0; f < frames; f++) {
            if (dists[f] < min) {
                min = dists[f];
            }
        }

        double max = DBL_MIN;
        for (int f = 0; f < frames; f++) {
            if (dists[f] > max) {
                max = dists[f];
            }
        }

        double media = 0;
        for (int f = 0; f < frames; f++) {
            media += dists[f];
        }
        media = media / frames;

        test_result resultado = {
            .min = min,
            .max = max,
            .media = media,
            .arr = dists
        };

        resultados[i] = resultado;
    }
    return resultados;
}
