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
#include "test/three_body.h"

#define MEASURE_TIME 0

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

int main() {

    //TESTING
    test_simulacion_three_bodies_verlet(40, 1);
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
