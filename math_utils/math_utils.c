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

//TODO: Esto deberia tomar una struct vector2, probablemente una const reference
double vector2_module(double x, double y) {
    double mod = sqrt(x * x + y * y);
    return mod;
}

// En algun momento puede ser buena idea implementar directivas de avx directamente?????? creo que esta usando SSE2 (GePeTo)
// por otro lado avx puede peder accuracy, segun GePeTo
double dot_product_3d(const vector3* a, const vector3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// Esta versión hace menos accesos a memoria el caso de un solo vector
double self_dot_product_3d(const vector3* a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

double vector3_module(const vector3* a)
{
    return sqrt(self_dot_product_3d(a));
}

// anticommutative a x b = - b x a
vector3 cross_product_3d(const vector3* a, const vector3* b)
{
    /*
     *         |  i  j  k |
     * a x b = | a1 a2 a3 |
     *         | b1 b2 b3 |
     *
     * a x b = (a2*b3 - a3*b2)*i + (a3*b1 - a1*b3)*j + (a1*b2 - a2*b1)*k
     */
    vector3 c = {
        (a->y*b->z - a->z*b->y),
        (a->z*b->x - a->x*b->z),
        (a->x*b->y - a->y*b->x),
    };
    return c;
}
