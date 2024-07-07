#include <stdio.h>

#include "energy.h"

void U_calc(const double G_cte, cuerpo2d* frame, int frame_len, double* buffer, int buffer_size) {
    int counter = 0;
    for (int j = 0; j < frame_len - 1; j++) {
        for (int i = j + 1; i < frame_len; i++) {
            
            double dx = frame[j].pos_x - frame[i].pos_x;
            double dy = frame[j].pos_y - frame[i].pos_y;
            double dist = vector2_module(dx, dy);
           
            double mi = frame[i].m;
            double mj = frame[j].m;

            double U = G_cte * mi * mj / dist;
            
            buffer[counter] = U;
            counter++;
        }
    }
}

void Us_to_matrix(double* buffer, int buffer_size, int planetas_number, double U_matrix[planetas_number][planetas_number]) {
    /*
        |  |p1|p2|p3|p4|
        |p1|  |00|01|02|
        |p2|0'|  |03|04|
        |p3|1'|3'|  |05|
        |p4|2'|4'|5'|  |
    */
    int counter = 0;

    //Inicializar a cero
    for (int j = 0; j < planetas_number; j++) {
        for (int i = 0; i < planetas_number; i++) {
            U_matrix[j][i] = 0.0;
        }
    }

    // Este -1 no se puede quitar, por lo que los "valores espejo" hay que añadirlos despues CREO
    for (int j = 0; j < planetas_number - 1; j++) {
        for (int i = j + 1; i < planetas_number; i++) {
            if (i > j) {
                //Original
                U_matrix[j][i] = buffer[counter];
                counter++;
            } else if (i == j) {
                // TODO: Mientras haga la inicializacion ahi arriba esto en realidad no haria falta
                U_matrix[j][i] = 0.0;
            } else {
                // TODO: Esto es provisional, quitar de aqui o añadir algo para el preprocesador
                // este no se como lo voy a quitar por ahora
                printf("Esto no deberia pasar Us_to_matrix()\n");
                return;
            }
        }
    }

    // Espejo
    for (int j = 0; j < planetas_number; j++) {
        for (int i = 0; i < planetas_number; i++) {
            if (i < j) {
                U_matrix[j][i] = U_matrix[i][j];
            }
        }
    }

}

void U_matrix_to_U_sum(int planetas_number, double matrix[planetas_number][planetas_number], double* U_sum) {
    for (int j = 0; j < planetas_number; j++) {
        U_sum[j] = 0.0;
        for (int i = 0; i < planetas_number; i++) {
            U_sum[j] += matrix[j][i];
        }
    }
}

double potential_energy_frame(cuerpo2d* frame, int frame_len) {
    
    int buffer_size = calc_buffer_size(frame_len);

    double buffer[buffer_size];
    double matrix[frame_len][frame_len];
    double sum[frame_len];

    U_calc(G, frame, frame_len, buffer, buffer_size);
    Us_to_matrix(buffer, buffer_size, frame_len, matrix);
    U_matrix_to_U_sum(frame_len, matrix, sum);

    double U = 0;
    for (int p = 0; p < frame_len; p++) {
        U += sum[p];
    }
    return U;
}

double cinetic_energy_frame(cuerpo2d* frame, int frame_len) {
    double T = 0;
    for (int p = 0; p < frame_len; p++) {
        double vmod = vector2_module(frame[p].v_x, frame[p].v_y);
        double m = frame[p].m;

        T += 0.5 * m * vmod * vmod;
    }

    return T;
}

void test_mechanical_energy(cuerpo2d* planetas, int planetas_number, int frames) {
    for (int f = 0; f < frames; f++) {
        double T = cinetic_energy_frame(&planetas[f * planetas_number], planetas_number);
        double U = potential_energy_frame(&planetas[f * planetas_number], planetas_number);
        double Em = T + U;

        printf(" T = %f\n U = %f\n Em = %f\n", T, U, Em);

        //Podriamos sacar T - U = L, para integrar y sacar la accion XD
    }
}
