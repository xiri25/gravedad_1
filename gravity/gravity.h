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
void cuerpos_simular_euler_2(cuerpo2d *planetas, int planetas_number, cuerpo2d *planetas_t0, int frames, double dt);
void cuerpos_simular_verlet_2(cuerpo2d *planetas, int planetas_number, cuerpo2d *planetas_t0, int frames, double dt);

int calc_buffer_size(int planetas_number);
vector2 F_gravedad(double G_cte, double mi, double mj, double dx, double dy);
vector3 F_gravedad_3d(const double G_cte, const double mi, const double mj,const vector3* r);
#endif
