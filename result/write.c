#include <stdio.h>
#include "write.h"

void escribir_planetas_archivo(cuerpo2d *resultados, int size) {
  FILE *fptr;
  fptr = fopen("resultados.txt", "w");
 
  //Este loop hay que hacerlo de 3 en 3
  for (int i = 0; i < size; i += 3) {
    fprintf(fptr, "%.10f %.10f %.10f %.10f %.10f %.10f\n",
            resultados[i].pos_x, resultados[i].pos_y,
            resultados[i+1].pos_x, resultados[i+1].pos_y,
            resultados[i+2].pos_x, resultados[i+2].pos_y);
  }

  fclose(fptr);
}
