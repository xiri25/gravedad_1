#ifndef TWO_B_PROBLEM
#define TWO_B_PROBLEM

#include "../cuerpos/cuerpos.h"
#include "../gravity/gravity.h"

typedef struct {
    double min;
    double max;
    double media;
    double *arr;
} test_result;

double *dist_verlet_2_body(cuerpo2d *planetas_t0, int frames, double dt);
cuerpo2d *condiciones_iniciales_2_body(double r_orbita, double v_factor);
test_result *test_two_b_p_v_factor(double *v_factors, int v_factors_len);

#endif
