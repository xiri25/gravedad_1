#ifndef GRAVITY
#define GRAVITY

#include "../cuerpos/cuerpos.h"
#include "../points/points.h"
#include "../math_utils/math_utils.h"

#define G 1.0

void points_simular_secuencial_1(point2d_64* puntos, int puntos_number, cuerpo2d* planetas, int planetas_number, int frames); 
void cuerpos_simular(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);
void cuerpos_simular_kahan(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);
void cuerpos_simular_verlet(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);
void cuerpos_simular_verlet_j_fijo(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);
void cuerpos_simular_j_fijo(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);

vector2 F_gravedad(double G_cte, double mi, double mj, double dx, double dy);
#endif
