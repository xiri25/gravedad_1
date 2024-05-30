#ifndef TWO_B_PROBLEM
#define TWO_B_PROBLEM

#include "../cuerpos/cuerpos.h"
#include "../gravity/gravity.h"

double *dist_verlet_2_body(cuerpo2d *planetas_t0, int frames, double dt);
cuerpo2d *condiciones_iniciales_2_body(double r_orbita, double v_factor);

#endif
