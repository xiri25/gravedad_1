#include "math_utils.h"

// x1 = F(x + h)
// x2 = F(x)
double math_derivada(double x1, double x2, double h) {
    double numerator = x1 - x2;
    return numerator / h;
}

// Devuelve un nuevo array, que tiene un elemento menos
// Cada vez que derivo hago el array mas pequeño
double *math_derivada_arr(double *F, double *x, int size) {
    double *result = (double*)malloc(sizeof(double)* (size - 1));
    for (int i = 0; i < size - 1; i++) {
        double h = x[i + 1] - x[i];
        double d = math_derivada(F[i + 1], F[i], h);
        result[i] = d;
    }
    return result;
}

double vector2_module(double x, double y) {
    double mod = sqrt(x * x + y * y);
    return mod;
}
