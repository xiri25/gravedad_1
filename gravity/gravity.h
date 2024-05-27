#ifndef GRAVITY
#define GRAVITY

#include "../cuerpos/cuerpos.h"
#include "../points/points.h"

void points_simular_secuencial_1(point2d_64* puntos, int puntos_number, cuerpo2d* planetas, int planetas_number, int frames); 
void cuerpos_simular(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt);

#endif
