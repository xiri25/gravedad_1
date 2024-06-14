#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math_utils/math_utils.h"
#include "points/points.h"
#include "cuerpos/cuerpos.h"
#include "result/write.h"
#include "gravity/gravity.h"
#include "setup/setup.h"
#include "test/test.h"
#include "test/two_b_problem.h"
#include "test/test_desmos.h"

#define MEASURE_TIME 1

#if MEASURE_TIME
#include <time.h>

#define UNO_K_MILLONES 1000000000.0

double now(void) {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  return current_time.tv_sec + (current_time.tv_nsec / UNO_K_MILLONES);
}
#endif


#define ITERATIONS 500

cuerpo2d mover_tierra(double radio_orbita, double w, int t) {
    // Por ahora un  movimiento circular
    double x, y;
    x = radio_orbita * cos(w*t);
    y = radio_orbita * sin(w*t);

    cuerpo2d cuerpo;

    cuerpo.pos_x = x;
    cuerpo.pos_y = y;
    cuerpo.r = 2;
    cuerpo.m = 2;
    return cuerpo;
}

cuerpo2d mover_luna(cuerpo2d tierra, double radio_orbita, double w, int t) {
    double dx, dy;
    dx = radio_orbita * cos(w*t);
    dy = radio_orbita * sin(w*t);

    double x, y;
    x = tierra.pos_x + dx;
    y = tierra.pos_y + dy;

    cuerpo2d cuerpo;
    cuerpo.pos_x = x;
    cuerpo.pos_y = y;
    cuerpo.r = 1;
    cuerpo.m = 1;

    return cuerpo;
}

int main() {

    //TESTING

    test_desmos_simulacion(100000);

    return 0;

    int v_factors_len = 21;
    // El 0 es una mala idea, porque no hay colisiones
    double v_factors[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    test_result *variableInd_vfactors = test_two_b_p_v_factor(v_factors, v_factors_len);

    int frames_harcodeado = 200000; //Harcodeado en la funcion test_two_b_p_v_factor
    double radio_harcodeado = 100; //Tambien harcodeado

    //printf("Resultados respecto a la orbita circular XD: \n");

    //Vamos a crear los dos arrays para poder diferenciar
    double medias[v_factors_len];
    for (int i = 0; i < v_factors_len; i++) {
        test_result test = variableInd_vfactors[i];
        
        //printf("V_factor = %f\n", v_factors[i]);
        //printf("Desviacion Media = %f\n", test.media);
        medias[i] = test.media;
        //printf("Desviacion Maxima = %f\n", test.max);
        //printf("Desviacion Minima = %f\n", test.min);
        //printf("Pointer resultados: %p\n", test.arr);
        
        /*
        for (int f = 0; f < frames_harcodeado; f++) {
            printf("%f\n", test.arr[f]);
        }
        */
        //printf("\n");
    }

    double *d_medias = math_derivada_arr(medias, v_factors, v_factors_len);

    printf("Derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 1; i++) {
        printf("%f %f\n", d_medias[i], v_factors[i]);
    }

    double *dd_medias = math_derivada_arr(d_medias, v_factors, v_factors_len - 1);
    free(d_medias);

    printf("Segunda derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 2; i++) {
        printf("%f %f\n", dd_medias[i], v_factors[i]);
    }

    double *ddd_medias = math_derivada_arr(dd_medias, v_factors, v_factors_len - 2);
    free(dd_medias);

    printf("Tercera derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 3; i++) {
        printf("%f %f\n", ddd_medias[i], v_factors[i]);
    }

    double *dddd_medias = math_derivada_arr(ddd_medias, v_factors, v_factors_len - 3);
    free(ddd_medias);

    printf("Cuarta derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 4; i++) {
        printf("%f %f\n", dddd_medias[i], v_factors[i]);
    }

    double *d5_medias = math_derivada_arr(dddd_medias, v_factors, v_factors_len - 4);
    free(dddd_medias);

    printf("Quinta derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 5; i++) {
        printf("%f %f\n", d5_medias[i], v_factors[i]);
    }

    double *d6_medias = math_derivada_arr(d5_medias, v_factors, v_factors_len - 5);
    free(d5_medias);

    printf("Sexta derivada de desviacion media respecto a v_factor\ndx/dy x\n");
    for (int i = 0; i < v_factors_len - 6; i++) {
        printf("%f %f\n", d6_medias[i], v_factors[i]);
    }

    free(d6_medias);
    
    // Al menos por ahora en un bucle distinto, no se muy bien si es esto lo que hay que hacer
    for (int i = 0; i < v_factors_len; i++) {
        free(variableInd_vfactors[i].arr);
    }

    free(variableInd_vfactors);

    return 0;

    //test_simulacion_cuerpos(20, 2, 1);
    test_simulacion_cuerpos_verlet(20000, 2, 0.01);
    //test_simulacion_cuerpos_kahan(20, 2, 1);
    //Literalemente los mismos resultados probado con (20, 2, 1) y (200, 1, 0.1)
//    
//    test_simulacion_cuerpos(200, 2, 0.1);
//    test_simulacion_cuerpos(2000, 2, 0.01);
//    test_simulacion_cuerpos(20000, 2, 0.001);

    /*

    cuerpo2d Sol = {
        .m = 100.0,
        .r = 100.0,
        .v_x = 0.0,
        .v_y = 0.0,
        .pos_x = 0.0,
        .pos_y = 0.0
    };

    //Podemos calcular la velocidad de la Tierra suponiendo una orbital circular
    double r_orbita = 1000;
    double tierra_v_2 = 1.0 * Sol.m / r_orbita;
    double tierra_v = sqrt(tierra_v_2);

    //La velocidad es perpendicular al radio
    cuerpo2d Tierra = {
        .m = 1.0,
        .r = 1.0,
        .v_x = tierra_v,
        .v_y = 0.0,
        .pos_x = 0.0,
        .pos_y = r_orbita
    };


     20frames,    2cuerpos, 1dt     1.88711606, 981.776166
     200frames,   2cuerpos, 0.1dt   1.97941911, 979.953918
     2000frames,  2cuerpos, 0.01dt  1.98863592, 979.771846
     20000frames, 2cuerpos, 0.001dt 1.98955712, 979.753641
    */
    return 0;

    #if MEASURE_TIME
    double start_time_main = now();
    #endif

    sim_info info = {
        .total_iter = 500,
        .point_size = sizeof(point2d_64),
        .point_number_per_frame = 150 * 150, //Deberia tener el grid ya calculado para poder poner este valor
        .body_size = sizeof(cuerpo2d),
        .body_number_per_frame = 3,
        .max_mem = 100 * 1024 * 1024 //100Mbytes
    };
    
    int divisions = SETUP_divisions(&info);
    printf("Divisions = %d\n", divisions);

    int size_array_indexes = sizeof(int) * (divisions + 1);
    printf("size_array_indexes : %d\n", size_array_indexes);

    int *indexes = malloc(size_array_indexes);
    indexes = SETUP_frames_division_indexes(indexes, divisions,  info.total_iter);

    for (int i = 0; i < (divisions + 1); i++) {
        printf("Frame_divisiones_indices[%d] = %d\n", i, indexes[i]);
    }
        
    free(indexes);
    //return 0;

    int size = sizeof(double);
    printf("Un double mide: %d bytes, %d bits\n", size, size * 8);

    #if MEASURE_TIME
    double start_time_planetas = now();
    #endif

    // Primero vamos a hacer que los "planetas" se muevan
    cuerpo2d sol = {0, 0, 50, 100};
    cuerpo2d tierra = {5, 5, 2, 20};
    cuerpo2d luna = {6, 6, 1, 10};

    int planetas_size = sizeof(cuerpo2d) * ITERATIONS * 3;
    cuerpo2d *planetas = (cuerpo2d *)malloc(planetas_size);
    if (planetas == NULL) {
        printf("Malloc failed alojando el array planetas\n");
        return 1;
    }
    planetas[0] = sol;
    planetas[1] = tierra;
    planetas[2] = luna;

    printf("D. Memoria de planetas: %p\n", (void *)planetas);

    //En este toy problem el sol es estático y punto de referencia
    printf("planetas_size: %d\n", planetas_size);
    int planetas_number = ITERATIONS * 3;
    printf("planetas_number: %d\n", planetas_number);
  
    double r_orbita_tierra_sol = sqrt(10*10);
  
    // (11 - 10)**2 + (11 - 10)**2
    double r_orbita_luna_tierra = sqrt(2);
    for (int i = 0; i < planetas_number; i += 3) {
        //printf("%d\n", i);
        planetas[i] = sol;
        planetas[i+1] = mover_tierra(r_orbita_tierra_sol, 0.01, i);
        planetas[i+2] = mover_luna(planetas[i+1], r_orbita_luna_tierra, 0.05, i);
    }

    #if MEASURE_TIME
    double stop_time_planetas = now();
    double start_time_planetas_escribir = stop_time_planetas;
    #endif

    escribir_planetas_archivo(planetas, 3, ITERATIONS);

    #if MEASURE_TIME
    double stop_time_planetas_escribir = now();
    double start_time_puntos_inicializar = stop_time_planetas_escribir;
    #endif

    //Points, try 1
    grid grid = {150, 150};

    int puntos_size = points_size(grid, ITERATIONS);
    printf("puntos_size = %d bytes\n", puntos_size);
    printf("puntos_size = %f Kbytes\n", (double)puntos_size / 1024);
    printf("puntos_size = %f Mbytes\n", (double)puntos_size / 1024 / 1024);
    printf("puntos_size = %f Gbytes\n", (double)puntos_size / 1024 / 1024 / 1024);

    // El cast del ptr se hace implicitamente, creo
    point2d_64 *puntos = malloc(puntos_size);

    if (puntos == NULL) {
        printf("Malloc failed alojando los puntos\n");
        return 1;
    }
    printf("D. de memoria de puntos: %p\n", puntos);

    inicilizar_points(puntos, grid, ITERATIONS);

    #if MEASURE_TIME
    double stop_time_puntos_inicializar = now();
    double start_time_puntos_simular = stop_time_puntos_inicializar;
    #endif

    int puntos_number = grid.height * grid.length * ITERATIONS;
    printf("Hopefully sizeof puntos: %lu\n", puntos_number * sizeof(point2d_64));

    //Claramente hay que mejorar the planetas_number situation
    points_simular_secuencial_1(puntos, puntos_number / ITERATIONS, planetas, 3, ITERATIONS); 

    #if MEASURE_TIME
    double stop_time_puntos_simular = now();
    double start_time_puntos_escribir = stop_time_puntos_simular;
    #endif

    escribir_puntos_archivo(puntos, puntos_size, &grid, ITERATIONS);

    #if MEASURE_TIME
    double stop_time_puntos_escribir = now();
    #endif

    //DO NOT FORGET
    free(planetas); // Necesito los planetas para calcular los puntos
    free(puntos);

    #if MEASURE_TIME

    double stop_time_main = now();
    double time_main = stop_time_main - start_time_main;

    double time_planetas = stop_time_planetas - start_time_planetas;
    printf("Time planetas_calc:      %0.10f s / %0.10f\n",
           time_planetas, time_planetas / time_main);

    double time_planetas_escribir = stop_time_planetas_escribir - start_time_planetas_escribir;
    printf("Time escribir planetas:  %0.10f s / %0.10f\n",
           time_planetas_escribir, time_planetas_escribir / time_main);

    double time_puntos_inicializar = stop_time_puntos_inicializar - start_time_puntos_inicializar;
    printf("Time inicializar puntos: %0.10f s / %0.10f\n",
           time_puntos_inicializar, time_puntos_inicializar / time_main);

    double time_puntos_simular = stop_time_puntos_simular - start_time_puntos_simular;
    printf("Time simular puntos:     %0.10f s / %0.10f\n",
           time_puntos_simular, time_puntos_simular / time_main);

    double time_puntos_escribir = stop_time_puntos_escribir - start_time_puntos_escribir;
    printf("Time escribir puntos:    %0.10f s / %0.10f\n",
           time_puntos_escribir, time_puntos_escribir / time_main);

    printf("Time main:               %.10f s\n",
           time_main);
    #endif

    return 0;
}
