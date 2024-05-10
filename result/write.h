#ifndef WRITE
#define WRITE

#include "../cuerpos/cuerpos.h"
#include "../points/points.h"

void escribir_planetas_archivo(cuerpo2d *resultados, int cuerpos_num, int frames);
void escribir_puntos_archivo(point2d_64 *resultado, int resultado_size, grid* grid, int frames);

#endif
