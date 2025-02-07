#ifndef MATH_UTILS
#define MATH_UTILS

#include <math.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} vector2;

typedef struct {
    double x;
    double y;
    double z;
} vector3;

// Devuelve un nuevo array, que tiene un elemento menos
// Cada vez que derivo hago el array mas pequeño
double *math_derivada_arr(double *F, double *x, int size);

double vector2_module(double x, double y);

double dot_product_3d(const vector3* a, const vector3* b);
double self_dot_product_3d(const vector3* a);
double vector3_module(const vector3* a);
vector3 cross_product_3d(const vector3* a, const vector3* b);
vector3 vector3_subtract(const vector3* a, const vector3* b);
vector3 vector3_add(const vector3* a, const vector3* b);
void vector3_escale(vector3* a, const double b);

#endif
