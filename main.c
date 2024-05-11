#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "points/points.h"
#include "cuerpos/cuerpos.h"
#include "result/write.h"
#include "gravity/gravity.h"
#include "setup/setup.h"

#define ITERATIONS 100

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

    sim_info info = {
        .total_iter = 100,
        .point_size = 1,
        .point_number_per_frame = 1,
        .body_size = 1,
        .body_number_per_frame = 1,
        .max_mem = 95
    };
    
    int divisions = SETUP_divisions(&info);
    printf("Divisions = %d\n", divisions);

    int size_array_indexes = sizeof(int) * (divisions + 1);
    printf("size_array_indexes : %d\n", size_array_indexes);

    int *indexes = malloc(size_array_indexes);
    SETUP_frames_division_indexes(indexes, divisions,  info.total_iter);

    return 0;

    int size = sizeof(double);
    printf("Un double mide: %d bytes, %d bits\n", size, size * 8);

    // Primero vamos a hacer que los "planetas" se muevan
    cuerpo2d sol = {0, 0, 100, 100};
    cuerpo2d tierra = {10, 10, 2, 2};
    cuerpo2d luna = {11, 11, 1, 1};

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
  
    double r_orbita_tierra_sol = sqrt(10*10+10*10);
  
    // (11 - 10)**2 + (11 - 10)**2
    double r_orbita_luna_tierra = sqrt(2);
    for (int i = 0; i < planetas_number; i += 3) {
        //printf("%d\n", i);
        planetas[i] = sol;
        planetas[i+1] = mover_tierra(r_orbita_tierra_sol, 0.01, i);
        planetas[i+2] = mover_luna(planetas[i+1], r_orbita_luna_tierra, 0.05, i);
    }

    escribir_planetas_archivo(planetas, 3, ITERATIONS);

    //Points, try 1
    grid grid = {10, 10};

    int puntos_size = points_size(grid, ITERATIONS);
    printf("puntos_size = %d bytes\n", puntos_size);

    // El cast del ptr se hace implicitamente, creo
    point2d_64 *puntos = malloc(puntos_size);

    if (puntos == NULL) {
        printf("Malloc failed alojando los puntos\n");
        return 1;
    }
    printf("D. de memoria de puntos: %p\n", puntos);

    inicilizar_points(puntos, grid, ITERATIONS);

    int puntos_number = grid.height * grid.length * ITERATIONS;
    printf("Hopefully sizeof puntos: %lu\n", puntos_number * sizeof(point2d_64));

    //Claramente hay que mejorar the planetas_number situation
    points_simular_secuencial_1(puntos, puntos_number / ITERATIONS, planetas, 3, ITERATIONS); 

    escribir_puntos_archivo(puntos, puntos_size, &grid, ITERATIONS);

    //DO NOT FORGET
    free(planetas); // Necesito los planetas para calcular los puntos
    free(puntos);

    return 0;
}
