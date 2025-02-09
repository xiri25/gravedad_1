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
double vector3_dot_product(const vector3* a, const vector3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// Esta versión hace menos accesos a memoria el caso de un solo vector
double vector3_self_dot_product(const vector3* a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

double vector3_module(const vector3* a)
{
    return sqrt(self_dot_product_3d(a));
}

// anticommutative a x b = - b x a
vector3 vector3_cross_product(const vector3* a, const vector3* b)
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

// a - b
vector3 vector3_subtract(const vector3* a, const vector3* b)
{
    vector3 c = {
        a->x - b->x,
        a->y - b->y,
        a->z - b->z,
    };
    return c;
}

// a + b
vector3 vector3_add(const vector3* a, const vector3* b)
{
    vector3 c = {
        a->x + b->x,
        a->y + b->y,
        a->z + b->z,
    };
    return c;
}

void vector3_escale_in_place(vector3* a, const double b)
{
    a->x *= b;
    a->y *= b;
    a->z *= b;
}

vector3 vector3_normalize(const vector3* a)
{
    double mod = vector3_module(a);
    double mod_inv = 1.0 / mod;
    return (vector3){a->x * mod_inv, a->y * mod_inv, a->z * mod_inv};
}

void vector3_normalize_in_place(vector3* a)
{
    double mod = vector3_module(a);
    double mod_inv = 1.0 / mod;
    vector3_escale_in_place(a, mod_inv);
}

// Como valor para poder escribir las columnas directamente en los parametros
double matrix3_by_col_det(const vector3 col1, const vector3 col2, const vector3 col3)
{
    /*
     *     |a b c|
     * det |d e f| = aei + dhc + bfg - ceg - bdi - fha
     *     |g h i|
     */

    double aei = col1.x * col2.y * col3.z;
    double dhc = col1.y * col2.z * col3.x;
    double bfg = col2.x * col3.y * col1.z;
    double ceg = col3.x * col2.y * col1.z;
    double bdi = col2.x * col1.y * col3.z;
    double fha = col3.y * col2.z * col1.x;

    return aei + dhc + bfg - ceg - bdi - fha;
}
