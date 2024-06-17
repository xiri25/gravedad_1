#ifndef MATH_UTILS
#define MATH_UTILS

#include <math.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} vector2;

// Devuelve un nuevo array, que tiene un elemento menos
// Cada vez que derivo hago el array mas pequeño
double *math_derivada_arr(double *F, double *x, int size);
double vector2_module(double x, double y);

#endif
