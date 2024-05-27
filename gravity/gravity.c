#include <math.h>
#include <stdio.h>
#include "gravity.h"

#define G 1.0
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

/*
    Las distancias que nos interesan calcular son
    |  |p1|p2|p3|p4|p5|
    |p1|  |xx|xx|xx|xx|
    |p2|  |  |xx|xx|xx|
    |p3|  |  |  |xx|xx|
    |p4|  |  |  |  |xx|
    |p5|  |  |  |  |  |

    Numero planetas -> distnacias que interesan
    2 -> 1  = 1
    3 -> 3  = 2 + 1
    4 -> 6  = 3 + 2 + 1
    5 -> 10 = 4 + 3 + 2 + 1
    ...

    distancias = (planetas - 1) * planetas / 2
*/

int calc_buffer_size(int planetas_number){
    if (planetas_number < 2) {
        printf("El numero de planetas es menor de 2\n");
        return 0;
    }
    return (planetas_number - 1) * planetas_number / 2;
}

//El array planetas_t0 contiene las condiciones iniciales y por lo tanto tiene {planetas_number} elementos
void cuerpos_simular(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt) {

    
    printf("gravity/gravity.c:cuerpos_simular()\n");
    printf("    planetas: %p\n", (void *)planetas);
    printf("    planetas_number: %d\n", planetas_number);
    printf("    planetas_t0: %p\n", (void *)planetas_t0);
    printf("    frames: %d\n", frames);
    printf("    dt: %f\n", dt);
    

    //Condiciones iniciales
    for (int i = 0; i < planetas_number; i++) {
        planetas[i] = planetas_t0[i];
    
        printf("    Seteando CI, planetas[%d].m = %f\n", i, planetas[i].m);
        printf("    Seteando CI, planetas[%d].pos_x = %f\n", i, planetas[i].pos_x);
        printf("    Seteando CI, planetas[%d].pos_y = %f\n", i, planetas[i].pos_y);
        printf("    Seteando CI, planetas[%d].v_x = %f\n", i, planetas[i].v_x);
        printf("    Seteando CI, planetas[%d].v_y = %f\n", i, planetas[i].v_y);
    }

    //buffers, TODO: teniendo en cuenta que al final no estoy usando ningun buffer, buscar otro nombre
    int buffer_size = calc_buffer_size(planetas_number);
    
    printf("    buffer_size: %d\n", buffer_size);

    //Simulacion
    //Empezamos en el frame 1, en lugar del 0, porque el 0 son las condiciones iniciales
    for (int f = 1; f < frames; f++) {

        printf("        f: %d\n", f);

        //Primero quiero calcular los vectores entre los planetas
        /*
            |  |p1|p2|p3|p4|p5|
            |p1|  |01|02|03|04|
            |p2|  |  |05|06|07|
            |p3|  |  |  |08|09|
            |p4|  |  |  |  |10|
            |p5|  |  |  |  |  |

            |  |p0|p1|p2|p3|p4|
            |p0|  |00|01|02|03|
            |p1|  |  |04|05|06|
            |p2|  |  |  |07|08|
            |p3|  |  |  |  |09|
            |p4|  |  |  |  |  |

            la i es el eje x
            la j es el eje y

            Calculamos los vactores en el sentido planeta[j] - planeta[i]
            Es decir estos vectores apuntan a planeta[j] desde planeta[i]
        */
        int index_frames_anterior = f * planetas_number - planetas_number;
        for (int j = 0; j < buffer_size; j++) {
            
            printf("                j: %d\n", j);
            
            for (int i = j + 1; i < buffer_size + 1; i++) {
                
                printf("                i: %d\n", i);

                //Estado del frame anterior
                cuerpo2d* planeta_i_frame_anterior = &planetas[index_frames_anterior + i];
                cuerpo2d* planeta_j_frame_anterior = &planetas[index_frames_anterior + j];
                
                
                printf("                    Index planetas_anterior i: %d\n", index_frames_anterior + i);
                printf("                    Masa_i_frame_anterior: %f\n", planeta_i_frame_anterior->m);
                printf("                    Index planetas_anterior j: %d\n", index_frames_anterior + j);
                printf("                    Masa_j_frame_anterior: %f\n", planeta_j_frame_anterior->m);
                
                
                //Calculamos el vector
                double dx = planeta_j_frame_anterior->pos_x - planeta_i_frame_anterior->pos_x;
                double dy = planeta_j_frame_anterior->pos_y - planeta_i_frame_anterior->pos_y;

                printf("                    (dx,dy) = (%f,%f)\n", dx, dy);
                
                //Calculamos la distancia
                double dist = vector2_module(dx, dy);
                
                printf("\n");
                printf("                    Distancia: %f\n", dist);

                //Calculo de Gm1m2
                double m1 = planeta_j_frame_anterior->m;
                
                printf("                    mj: %f\n", m1);
                
                double m2 = planeta_i_frame_anterior->m;
                
                printf("                    mi: %f\n", m2);
                printf("                    G: %f\n", G);
                
                double Gm1m2 = G * m1 * m2;
                
                printf("                    Gm1m2: %f\n", Gm1m2);

                double dist_2 = dist * dist;

                //Calculamos la fuerza
                double F_x = (Gm1m2 * dx) / dist_2;
                double F_y = (Gm1m2 * dy) / dist_2;

                printf("                    Fuerza: (%f, %f)\n", F_x, F_y);

                //Aceleraciones
                double ai_x = F_x / m2;
                double ai_y = F_y / m2;
                double aj_x = -F_x / m1;
                double aj_y = -F_y / m1;

                cuerpo2d* planeta_i = &planetas[index_frames_anterior + planetas_number + i];
                cuerpo2d* planeta_j = &planetas[index_frames_anterior + planetas_number + j];

                printf("                    Planetas que se estan calculando (i,j): (%d, %d)\n",
                       index_frames_anterior + planetas_number + i,
                       index_frames_anterior + planetas_number + j);

                //Estado antes de los deltas
                planeta_i->v_x = planeta_i_frame_anterior->v_x;
                planeta_i->v_y = planeta_i_frame_anterior->v_y;
                planeta_i->pos_x = planeta_i_frame_anterior->pos_x;
                planeta_i->pos_y = planeta_i_frame_anterior->pos_y;
                planeta_j->v_x = planeta_j_frame_anterior->v_x;
                planeta_j->v_y = planeta_j_frame_anterior->v_y;
                planeta_j->pos_x = planeta_j_frame_anterior->pos_x;
                planeta_j->pos_y = planeta_j_frame_anterior->pos_y;

                //Velocidades
                planeta_i->v_x += ai_x * dt;
                planeta_i->v_y += ai_y * dt;
                planeta_j->v_x += aj_x * dt;
                planeta_j->v_y += aj_y * dt;

                printf("                    vix: %f\n", planeta_i->v_x);
                printf("                    viy: %f\n", planeta_i->v_y);
                printf("                    vjx: %f\n", planeta_j->v_x);
                printf("                    vjy: %f\n", planeta_j->v_y);

                //Posiciones TODO: variables para tener las velocidades en el stack y no tener que acceder al array
                planeta_i->pos_x += planeta_i->v_x * dt + 0.5 * ai_x * dt * dt;
                planeta_i->pos_y += planeta_i->v_y * dt + 0.5 * ai_y * dt * dt;
                planeta_j->pos_x += planeta_j->v_x * dt + 0.5 * aj_x * dt * dt;
                planeta_j->pos_y += planeta_j->v_y * dt + 0.5 * aj_y * dt * dt;

                printf("                    pix: %f\n", planeta_i->pos_x);
                printf("                    piy: %f\n", planeta_i->pos_y);
                printf("                    pjx: %f\n", planeta_j->pos_x);
                printf("                    pjy: %f\n", planeta_j->pos_y);

                //Las constantes las voy a pasar por aqui por ahora
                planeta_i->m = planeta_i_frame_anterior->m;
                planeta_i->r = planeta_i_frame_anterior->r;
                planeta_j->m = planeta_j_frame_anterior->m;
                planeta_j->r = planeta_j_frame_anterior->r;
            }
        }
        printf("\n\n\n");
    }
}
