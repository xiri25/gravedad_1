#include <stdio.h>
#include "write.h"

void escribir_planetas_archivo(cuerpo2d *resultados, int size) {
    FILE *fptr;
    fptr = fopen("resultados_cuerpos.txt", "w");
 
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return;
    }

    //Este loop hay que hacerlo de 3 en 3 porque en este caso hay 3 cuerpos
    for (int i = 0; i < size; i += 3) {
        fprintf(fptr, "%.10f %.10f %.10f %.10f %.10f %.10f\n",
                resultados[i].pos_x, resultados[i].pos_y,
                resultados[i+1].pos_x, resultados[i+1].pos_y,
                resultados[i+2].pos_x, resultados[i+2].pos_y);
    }

    fclose(fptr);
}

void escribir_puntos_archivo(point2d_64 *resultado, int resultado_size, grid* grid, int frames) {
    FILE *fptr;
    fptr = fopen("resultados_puntos.txt", "w");
  
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return;
    }
  
    int grid_size = grid->height * grid->length;

    //Lo mejor que se ocurre por ahora
    if (grid_size * frames * (int)sizeof(point2d_64) != resultado_size) {
        fprintf(fptr, "grid_size * frames * sizeof(point2d_64) != resultado_size pasado\n");
        fprintf(fptr, "%d * %d * %lu != %d\n", grid_size, frames, sizeof(point2d_64), resultado_size);
        fclose(fptr);
        return;
    }

    //Quiero separar cada frame
    for (int f = 0; f < frames; f++) {
        //Cada punto en una linea
        for (int i = 0; i < grid_size; i++) {
            // En teoria deberian ser 12 characteres incluyendo la coma, 10 de ellos despues de la coma, pero se la pela
            fprintf(fptr, "%12.10f %12.10f %12.10f %12.10f %12.10f %12.10f %12.10f\n",
                    resultado[i + f * grid_size].pos_x,
                    resultado[i + f * grid_size].pos_y,
                    resultado[i + f * grid_size].vector_x,
                    resultado[i + f * grid_size].vector_y,
                    resultado[i + f * grid_size].mod_vector,
                    resultado[i + f * grid_size].norm_vec_x,
                    resultado[i + f * grid_size].norm_vec_y);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
}
