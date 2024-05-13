#include <math.h>
#include <stdio.h>
#include "gravity.h"

#define G 1
#define EPSILON 0.00000001


//Esta funcion quizas la quiero en otro archivo
double vector2_module(double x, double y) {
    double mod = sqrt(x * x + y * y);
    return mod;
}

void points_simular_secuencial_1(point2d_64* puntos, int puntos_number, cuerpo2d* planetas, int planetas_number, int frames) {

    for (int f = 0; f < frames; f++) {
        //printf("g.Frame = %d\n", f);
        // Dentro de cada frame, recorro todos los puntos
        for (int i = 0; i < puntos_number; i++) {

            //printf("    g.Punto: %d", i);
            // En principio merece la pena intentar mantener esto en el stack
          
            // De cada punto queremos calcular el vector gravedad, el modulo del vector gravedad y el vector gravedad normalizado

            double modulo_vector_gravedad;

            // Componentes del Vector Gravedad normalizado
            double GX_n;
            double GY_n;

            // Componentes del Vector Gravedad
            double GX = 0;
            double GY = 0;

          
            int frame_offset = f * puntos_number;
          
            // Para un solo punto, por cada planeta un vector
            for (int j = 0; j < planetas_number; j++) {
            
                // Empezamos calculando el vector que une el punto con el planeta
                double rx = planetas[f * planetas_number + j].pos_x - puntos[frame_offset + i].pos_x;
                double ry = planetas[f * planetas_number + j].pos_y - puntos[frame_offset + i].pos_y;

                // Ahora el modulo del vector
                double dist = vector2_module(rx, ry);

                if (dist < EPSILON) {
                    // Lo ponemos a cero y continuamos
                    //GX = 0;
                    //GY = 0;
                    continue;
                }

                double dist_cube = dist * dist * dist;

                // Ahora calculamos las componentes de la gravedad entre el punto y este planeta
                double Gm = G * planetas[f* planetas_number + j].m;
                double Gm_entre_r3 = Gm / dist_cube;
                GX += Gm_entre_r3 * rx;
                GY += Gm_entre_r3 * ry;
            }

            modulo_vector_gravedad = vector2_module(GX, GY);
            //printf(" g.modulo = %f\n", modulo_vector_gravedad);
            GX_n = GX / modulo_vector_gravedad;
            GY_n = GY / modulo_vector_gravedad;

    //        puntos[frame_offset + i].vector_x = GX;
    //        puntos[frame_offset + i].vector_y = GX;
            puntos[frame_offset + i].mod_vector = modulo_vector_gravedad;
            puntos[frame_offset + i].norm_vec_x = GX_n;
            puntos[frame_offset + i].norm_vec_y = GY_n;
           
            /*
            //Debugging
            printf("%12.10f %12.10f %12.10f %12.10f %12.10f\n",
                puntos[i + frame_offset].pos_x,
                puntos[i + frame_offset].pos_y,
        //                    resultado[i + f * grid_size].vector_x,
        //                    resultado[i + f * grid_size].vector_y,
                puntos[i + frame_offset].mod_vector,
                puntos[i + frame_offset].norm_vec_x,
                puntos[i + frame_offset].norm_vec_y);
            */
        }
    //printf("\n");
    }
}
