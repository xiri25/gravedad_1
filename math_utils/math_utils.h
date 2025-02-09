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

typedef struct {
    double m[3][3];
}cMatrix3;

// Devuelve un nuevo array, que tiene un elemento menos
// Cada vez que derivo hago el array mas pequeño
double *math_derivada_arr(double *F, double *x, int size);

double vector2_module(double x, double y);

double vector3_dot_product(const vector3* a, const vector3* b);
double vector3_self_dot_product(const vector3* a);
double vector3_module(const vector3* a);
vector3 vector3_cross_product(const vector3* a, const vector3* b);
vector3 vector3_subtract(const vector3* a, const vector3* b);
vector3 vector3_add(const vector3* a, const vector3* b);
void vector3_escale_in_place(vector3* a, const double b);
vector3 vector3_normalize(const vector3* a);
void vector3_normalize_in_place(vector3* a);
double matrix3_by_col_det(const vector3 col1, const vector3 col2, const vector3 col3);

#endif
