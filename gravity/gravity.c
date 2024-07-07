#include <stdio.h>
#include "gravity.h"

#define GRAVITY_DEBUG 0

#define EPSILON 0.00000001

vector2 F_gravedad(double G_cte, double mi, double mj, double dx, double dy) {

    double Gmimj = G_cte * mi * mj;
    double r_mod = vector2_module(dx, dy);
    double r_mod_3 = r_mod * r_mod * r_mod;
    double g = Gmimj / r_mod_3;

    vector2 gravedad = {-g * dx, -g * dy};

    return gravedad;
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

double kahan_sum(double *nums, int len) {
    double sum = 0.0f;
    double c = 0.0f;
    double t, y;

    for (int i = 0; i < len; i++) {
        y = nums[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

//El array planetas_t0 contiene las condiciones iniciales y por lo tanto tiene {planetas_number} elementos
void cuerpos_simular_kahan(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt) {

    
    printf("gravity/gravity.c:cuerpos_simular_kahan()\n");
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
                double sum_vix[] = {planeta_i->v_x, ai_x * dt};
                double sum_viy[] = {planeta_i->v_y, ai_y * dt};
                double sum_vjx[] = {planeta_j->v_x, aj_x * dt};
                double sum_vjy[] = {planeta_j->v_y, aj_y * dt};
                
                planeta_i->v_x = kahan_sum(sum_vix, 2);
                planeta_i->v_y = kahan_sum(sum_viy, 2);
                planeta_j->v_x = kahan_sum(sum_vjx, 2);
                planeta_j->v_y = kahan_sum(sum_vjy, 2);

                printf("                    vix: %f\n", planeta_i->v_x);
                printf("                    viy: %f\n", planeta_i->v_y);
                printf("                    vjx: %f\n", planeta_j->v_x);
                printf("                    vjy: %f\n", planeta_j->v_y);

                //Posiciones TODO: variables para tener las velocidades en el stack y no tener que acceder al array
                double sum_pix[] = {planeta_i->pos_x, planeta_i->v_x * dt, 0.5 * ai_x * dt * dt};
                double sum_piy[] = {planeta_i->pos_y, planeta_i->v_y * dt, 0.5 * ai_y * dt * dt};
                double sum_pjx[] = {planeta_j->pos_x, planeta_j->v_x * dt, 0.5 * aj_x * dt * dt};
                double sum_pjy[] = {planeta_j->pos_y, planeta_j->v_y * dt, 0.5 * aj_y * dt * dt};

                planeta_i->pos_x = kahan_sum(sum_pix, 3);
                planeta_i->pos_y = kahan_sum(sum_piy, 3);
                planeta_j->pos_x = kahan_sum(sum_pjx, 3);
                planeta_j->pos_y = kahan_sum(sum_pjy, 3);

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

//El array planetas_t0 contiene las condiciones iniciales y por lo tanto tiene {planetas_number} elementos
void cuerpos_simular_verlet(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt) {
    // En principio verlet no usa la velocidad para actualizar la posicion, por lo que no haria falta
    // guardarla, pero por mantener paridad y porque la puede calcular (con algo mas de error que la pos)
    // la voy a calcular
    

    //buffers, TODO: teniendo en cuenta que al final no estoy usando ningun buffer, buscar otro nombre
    int buffer_size = calc_buffer_size(planetas_number);
    printf("    Cuerpos_simular_verlet. buffer_size = %d\n", buffer_size);
    printf("    El frame 0 son las condiciones iniciales, por eso no se loopea por el\n");
    
    //Condiciones iniciales
    
    // Si lo he entendido bien Verlet necesita los 2 deltaT anteriores para calcualar el siguiente
    for (int i = 0; i < planetas_number; i++) {
        planetas[i] = planetas_t0[i];
    }

    //TODO: Frame 1 mejorar
    for (int f = 1; f < 2; f++) {

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

            for (int i = j + 1; i < buffer_size + 1; i++) {

                printf("    f: %d, j: %d, i: %d\n", f, j, i);
                //Estado del frame anterior
                cuerpo2d* planeta_i_frame_anterior = &planetas[index_frames_anterior + i];
                cuerpo2d* planeta_j_frame_anterior = &planetas[index_frames_anterior + j];
                
                printf("        idx_f_n-1 + i: %d\n", index_frames_anterior + i);
                printf("        idx_f_n-1 + j: %d\n", index_frames_anterior + j);
                
                //Calculamos el vector
                double dx = planeta_j_frame_anterior->pos_x - planeta_i_frame_anterior->pos_x;
                double dy = planeta_j_frame_anterior->pos_y - planeta_i_frame_anterior->pos_y;

                double mj = planeta_j_frame_anterior->m; 
                double mi = planeta_i_frame_anterior->m;

                //Calculamos la fuerza
                vector2 gravedad = F_gravedad(G, mj, mi, dx, dy);

                //Aceleraciones
                double ai_x = gravedad.x / mi;
                double ai_y = gravedad.y / mi;
                double aj_x = -gravedad.x / mj;
                double aj_y = -gravedad.y / mj;

                cuerpo2d* planeta_i = &planetas[index_frames_anterior + planetas_number + i];
                cuerpo2d* planeta_j = &planetas[index_frames_anterior + planetas_number + j];
 
                printf("        idx_f_n + i: %d\n", index_frames_anterior + planetas_number + i);
                printf("        idx_f_n + j: %d\n", index_frames_anterior + planetas_number + j);
                
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
                double sum_vix[] = {planeta_i->v_x, ai_x * dt};
                double sum_viy[] = {planeta_i->v_y, ai_y * dt};
                double sum_vjx[] = {planeta_j->v_x, aj_x * dt};
                double sum_vjy[] = {planeta_j->v_y, aj_y * dt};
                
                planeta_i->v_x = kahan_sum(sum_vix, 2);
                planeta_i->v_y = kahan_sum(sum_viy, 2);
                planeta_j->v_x = kahan_sum(sum_vjx, 2);
                planeta_j->v_y = kahan_sum(sum_vjy, 2);

                //Posiciones TODO: variables para tener las velocidades en el stack y no tener que acceder al array
                double sum_pix[] = {planeta_i->pos_x, planeta_i->v_x * dt, 0.5 * ai_x * dt * dt};
                double sum_piy[] = {planeta_i->pos_y, planeta_i->v_y * dt, 0.5 * ai_y * dt * dt};
                double sum_pjx[] = {planeta_j->pos_x, planeta_j->v_x * dt, 0.5 * aj_x * dt * dt};
                double sum_pjy[] = {planeta_j->pos_y, planeta_j->v_y * dt, 0.5 * aj_y * dt * dt};

                planeta_i->pos_x = kahan_sum(sum_pix, 3);
                planeta_i->pos_y = kahan_sum(sum_piy, 3);
                planeta_j->pos_x = kahan_sum(sum_pjx, 3);
                planeta_j->pos_y = kahan_sum(sum_pjy, 3);

                //Las constantes las voy a pasar por aqui por ahora
                planeta_i->m = planeta_i_frame_anterior->m;
                planeta_i->r = planeta_i_frame_anterior->r;
                planeta_j->m = planeta_j_frame_anterior->m;
                planeta_j->r = planeta_j_frame_anterior->r;
            }
        }
    }

    //Simulacion
    //Empezamos en el frame 2, en lugar del 0, porque el 0 y el 1 son las condiciones iniciales
    for (int f = 2; f < frames; f++) {

        //printf("        f: %d\n", f);

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
        int index_frame_n_menos_1 = f * planetas_number - planetas_number;
        int index_frame_n_menos_2 = (f - 1) * planetas_number - planetas_number;
        for (int j = 0; j < buffer_size; j++) {
            
            //printf("                j: %d\n", j);
            
            for (int i = j + 1; i < buffer_size + 1; i++) {
                
                //printf("                i: %d\n", i);
                //printf("\n");

                printf("    f: %d, j: %d, i: %d\n", f, j, i);
                //Estado del frame n-2
                cuerpo2d* planeta_i_frame_n_menos_2 = &planetas[index_frame_n_menos_2 + i];
                cuerpo2d* planeta_j_frame_n_menos_2 = &planetas[index_frame_n_menos_2 + j];

                printf("        idx_f_n-2 + i: %d\n", index_frame_n_menos_2 + i);
                printf("        idx_f_n-2 + j: %d\n", index_frame_n_menos_2 + j);

                double i_posx_n2 = planeta_i_frame_n_menos_2->pos_x;
                double i_posy_n2 = planeta_i_frame_n_menos_2->pos_y;
                double j_posx_n2 = planeta_j_frame_n_menos_2->pos_x;
                double j_posy_n2 = planeta_j_frame_n_menos_2->pos_y;


                //Estado del frame n-1
                cuerpo2d* planeta_i_frame_n_menos_1 = &planetas[index_frame_n_menos_1 + i];
                cuerpo2d* planeta_j_frame_n_menos_1 = &planetas[index_frame_n_menos_1 + j];

                printf("        idx_f_n-1 + i: %d\n", index_frame_n_menos_1 + i);
                printf("        idx_f_n-1 + j: %d\n", index_frame_n_menos_1 + j);
                
                double i_posx_n1 = planeta_i_frame_n_menos_1->pos_x;
                double i_posy_n1 = planeta_i_frame_n_menos_1->pos_y;
                double j_posx_n1 = planeta_j_frame_n_menos_1->pos_x;
                double j_posy_n1 = planeta_j_frame_n_menos_1->pos_y;
                double i_vx_n1 = planeta_i_frame_n_menos_1->v_x;
                double i_vy_n1 = planeta_i_frame_n_menos_1->v_y;
                double j_vx_n1 = planeta_j_frame_n_menos_1->v_x;
                double j_vy_n1 = planeta_j_frame_n_menos_1->v_y;
 
                //Calculamos el vector n-2
                double dx_n2 = j_posx_n2 - i_posx_n2;
                double dy_n2 = j_posy_n2 - i_posy_n2;

                //Calculamos el vector n-1
                double dx_n1 = j_posx_n1 - i_posx_n1;
                double dy_n1 = j_posy_n1 - i_posy_n1;

                double mj_n1 = planeta_j_frame_n_menos_1->m;
                double mi_n1 = planeta_i_frame_n_menos_1->m;

                double mj_n2 = planeta_j_frame_n_menos_2->m;
                double mi_n2 = planeta_i_frame_n_menos_2->m;

                //Calculamos la fuerza
                vector2 gravedad_n1 = F_gravedad(G, mi_n1, mj_n1, dx_n1, dy_n1);
                vector2 gravedad_n2 = F_gravedad(G, mi_n2, mj_n2, dx_n2, dy_n2);

                //Aceleraciones
                double ai_x_n1 =  gravedad_n1.x / mi_n1;
                double ai_y_n1 =  gravedad_n1.y / mi_n1;
                double aj_x_n1 = -gravedad_n1.x / mj_n1;
                double aj_y_n1 = -gravedad_n1.y / mj_n1;

                double ai_x_n2 =  gravedad_n2.x / mi_n2;
                double ai_y_n2 =  gravedad_n2.y / mi_n2;
                double aj_x_n2 = -gravedad_n2.x / mj_n2;
                double aj_y_n2 = -gravedad_n2.y / mj_n2;

                //Planeta actual
                cuerpo2d* planeta_i = &planetas[index_frame_n_menos_1 + planetas_number + i];
                cuerpo2d* planeta_j = &planetas[index_frame_n_menos_1 + planetas_number + j];
                
                printf("        idx_f_n + i: %d\n", index_frame_n_menos_1 + planetas_number + i);
                printf("        idx_f_n + j: %d\n", index_frame_n_menos_1 + planetas_number + j);
                
                //Esto por ahora asi XD TODO: Quizas buscar otra manera
                double mj_n = mj_n1;
                double mi_n = mi_n1;

                // x(n) = 2x(n-1) - x(n-2) + a(n-1)dt2
                double i_posx_n = 2 * i_posx_n1 - i_posx_n2 + ai_x_n1 * dt * dt;
                double i_posy_n = 2 * i_posy_n1 - i_posy_n2 + ai_y_n1 * dt * dt;
                double j_posx_n = 2 * j_posx_n1 - j_posx_n2 + aj_x_n1 * dt * dt;
                double j_posy_n = 2 * j_posy_n1 - j_posy_n2 + aj_y_n1 * dt * dt;

                //Por mantener consistencia, vamos a calcular velocidad ES UN PUTO COÑAZO
                // v(n) = v(n-1) * 1/6 * (2a(n) + 5a(n-1) - a(n-2)) * dt
                // Primero hay que calcular a(n)
                double dx_n = j_posx_n - i_posx_n;
                double dy_n = j_posy_n - i_posy_n;

                vector2 gravedad_n = F_gravedad(G, mi_n, mj_n, dx_n, dy_n);

                double ai_x_n =  gravedad_n.x / mi_n;
                double ai_y_n =  gravedad_n.y / mi_n;
                double aj_x_n = -gravedad_n.x / mj_n;
                double aj_y_n = -gravedad_n.y / mj_n;

                double un_sexto = (double) 1 / 6;

                double i_vx_n = i_vx_n1 + un_sexto * (2 * ai_x_n + 5 * ai_x_n1 - ai_x_n2) * dt;
                double i_vy_n = i_vy_n1 + un_sexto * (2 * ai_y_n + 5 * ai_y_n1 - ai_y_n2) * dt;
                double j_vx_n = j_vx_n1 + un_sexto * (2 * aj_x_n + 5 * aj_x_n1 - aj_x_n2) * dt;
                double j_vy_n = j_vy_n1 + un_sexto * (2 * aj_y_n + 5 * aj_y_n1 - aj_y_n2) * dt;

                //Las constantes las voy a pasar por aqui por ahora
                planeta_i->m = planeta_i_frame_n_menos_1->m;
                planeta_i->r = planeta_i_frame_n_menos_1->r;
                planeta_j->m = planeta_j_frame_n_menos_1->m;
                planeta_j->r = planeta_j_frame_n_menos_1->r;


                planeta_i->pos_x = i_posx_n;
                planeta_i->pos_y = i_posy_n;
                planeta_i->v_x   = i_vx_n;
                planeta_i->v_y   = i_vy_n;

                planeta_j->pos_x = j_posx_n;
                planeta_j->pos_y = j_posy_n;
                planeta_j->v_x   = j_vx_n;
                planeta_j->v_y   = j_vy_n;

                /*
                printf("                (pix, piy) = (%f, %f)\n", i_posx_n, i_posy_n);
                printf("                (pjx, pjy) = (%f, %f)\n", j_posx_n, j_posy_n);
                */
            }
        }
        //printf("\n\n\n");
    }
}

//El array planetas_t0 contiene las condiciones iniciales y por lo tanto tiene {planetas_number} elementos
void cuerpos_simular_verlet_j_fijo(cuerpo2d* planetas, int planetas_number, cuerpo2d* planetas_t0, int frames, double dt) {
    // En principio verlet no usa la velocidad para actualizar la posicion, por lo que no haria falta
    // guardarla, pero por mantener paridad y porque la puede calcular (con algo mas de error que la pos)
    // la voy a calcular
    

    //buffers, TODO: teniendo en cuenta que al final no estoy usando ningun buffer, buscar otro nombre
    int buffer_size = calc_buffer_size(planetas_number);
    
    //Condiciones iniciales
    
    // Si lo he entendido bien Verlet necesita los 2 deltaT anteriores para calcualar el siguiente
    for (int i = 0; i < planetas_number; i++) {
        planetas[i] = planetas_t0[i];
    }

    //TODO: Frame 1 mejorar
    for (int f = 1; f < 2; f++) {

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

            for (int i = j + 1; i < buffer_size + 1; i++) {

                //Estado del frame anterior
                cuerpo2d* planeta_i_frame_anterior = &planetas[index_frames_anterior + i];
                cuerpo2d* planeta_j_frame_anterior = &planetas[index_frames_anterior + j];
                
                //Calculamos el vector
                double dx = 0.0 - planeta_i_frame_anterior->pos_x;
                double dy = 0.0 - planeta_i_frame_anterior->pos_y;

                //Calculamos la distancia
                double dist = vector2_module(dx, dy);

                //Calculo de Gm1m2
                double m1 = planeta_j_frame_anterior->m;
                
                double m2 = planeta_i_frame_anterior->m;

                double Gm1m2 = G * m1 * m2;
                
                double dist_2 = dist * dist;

                //Calculamos la fuerza
                vector2 gravedad = F_gravedad(G, m1, m2, dx, dy);
                double F_x = gravedad.x;
                double F_y = gravedad.y;

                //Aceleraciones
                double ai_x = F_x / m2;
                double ai_y = F_y / m2;
                double aj_x = 0.0;
                double aj_y = 0.0;

                cuerpo2d* planeta_i = &planetas[index_frames_anterior + planetas_number + i];
                cuerpo2d* planeta_j = &planetas[index_frames_anterior + planetas_number + j];
 
                //Estado antes de los deltas
                planeta_i->v_x = planeta_i_frame_anterior->v_x;
                planeta_i->v_y = planeta_i_frame_anterior->v_y;
                planeta_i->pos_x = planeta_i_frame_anterior->pos_x;
                planeta_i->pos_y = planeta_i_frame_anterior->pos_y;
                planeta_j->v_x = 0.0;
                planeta_j->v_y = 0.0;
                planeta_j->pos_x = 0.0;
                planeta_j->pos_y = 0.0;

                //Velocidades
                double sum_vix[] = {planeta_i->v_x, ai_x * dt};
                double sum_viy[] = {planeta_i->v_y, ai_y * dt};
                double sum_vjx[] = {planeta_j->v_x, aj_x * dt};
                double sum_vjy[] = {planeta_j->v_y, aj_y * dt};
                
                planeta_i->v_x = kahan_sum(sum_vix, 2);
                planeta_i->v_y = kahan_sum(sum_viy, 2);
                planeta_j->v_x = 0.0;
                planeta_j->v_y = 0.0;

                //Posiciones TODO: variables para tener las velocidades en el stack y no tener que acceder al array
                double sum_pix[] = {planeta_i->pos_x, planeta_i->v_x * dt, 0.5 * ai_x * dt * dt};
                double sum_piy[] = {planeta_i->pos_y, planeta_i->v_y * dt, 0.5 * ai_y * dt * dt};
                double sum_pjx[] = {planeta_j->pos_x, planeta_j->v_x * dt, 0.5 * aj_x * dt * dt};
                double sum_pjy[] = {planeta_j->pos_y, planeta_j->v_y * dt, 0.5 * aj_y * dt * dt};

                planeta_i->pos_x = kahan_sum(sum_pix, 3);
                planeta_i->pos_y = kahan_sum(sum_piy, 3);
                planeta_j->pos_x = 0.0;
                planeta_j->pos_y = 0.0;

                //Las constantes las voy a pasar por aqui por ahora
                planeta_i->m = planeta_i_frame_anterior->m;
                planeta_i->r = planeta_i_frame_anterior->r;
                planeta_j->m = planeta_j_frame_anterior->m;
                planeta_j->r = planeta_j_frame_anterior->r;
            }
        }
    }

    //Simulacion
    //Empezamos en el frame 2, en lugar del 0, porque el 0 y el 1 son las condiciones iniciales
    for (int f = 2; f < frames; f++) {

        //printf("        f: %d\n", f);

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
        int index_frame_n_menos_1 = f * planetas_number - planetas_number;
        int index_frame_n_menos_2 = (f - 1) * planetas_number - planetas_number;
        for (int j = 0; j < buffer_size; j++) {
            
            //printf("                j: %d\n", j);
            
            for (int i = j + 1; i < buffer_size + 1; i++) {
                
                //printf("                i: %d\n", i);
                //printf("\n");

                //Estado del frame n-2
                cuerpo2d* planeta_i_frame_n_menos_2 = &planetas[index_frame_n_menos_2 + i];
                cuerpo2d* planeta_j_frame_n_menos_2 = &planetas[index_frame_n_menos_2 + j];

                double i_posx_n2 = planeta_i_frame_n_menos_2->pos_x;
                double i_posy_n2 = planeta_i_frame_n_menos_2->pos_y;
                double j_posx_n2 = 0.0;
                double j_posy_n2 = 0.0;

                //printf("                (pix_n2, piy_n2) = (%f, %f)\n", i_posx_n2, i_posy_n2);
                //printf("                (pjx_n2, pjy_n2) = (%f, %f)\n", j_posx_n2, j_posy_n2);
                //printf("\n");

                //Estado del frame n-1
                cuerpo2d* planeta_i_frame_n_menos_1 = &planetas[index_frame_n_menos_1 + i];
                cuerpo2d* planeta_j_frame_n_menos_1 = &planetas[index_frame_n_menos_1 + j];

                double i_posx_n1 = planeta_i_frame_n_menos_1->pos_x;
                double i_posy_n1 = planeta_i_frame_n_menos_1->pos_y;
                double j_posx_n1 = 0.0;
                double j_posy_n1 = 0.0;
                double i_vx_n1 = planeta_i_frame_n_menos_1->v_x;
                double i_vy_n1 = planeta_i_frame_n_menos_1->v_y;
                double j_vx_n1 = 0.0;
                double j_vy_n1 = 0.0;
 
                //printf("                (pix_n1, piy_n1) = (%f, %f)\n", i_posx_n1, i_posy_n1);
                //printf("                (pjx_n1, pjy_n1) = (%f, %f)\n", j_posx_n1, j_posy_n1);
                //printf("\n");
                /*
                printf("                (vix_n1, viy_n1) = (%f, %f)\n", i_vx_n1, i_vy_n1);
                printf("                (vjx_n1, vjy_n1) = (%f, %f)\n", j_vx_n1, j_vy_n1);
                printf("\n");               
                */

                //Calculamos el vector n-2
                double dx_n2 = j_posx_n2 - i_posx_n2;
                double dy_n2 = j_posy_n2 - i_posy_n2;

                //Calculamos el vector n-1
                double dx_n1 = j_posx_n1 - i_posx_n1;
                double dy_n1 = j_posy_n1 - i_posy_n1;
               
                //Calculamos la distancia
                double dist_n1 = vector2_module(dx_n1, dy_n1);
                double dist_n2 = vector2_module(dx_n2, dy_n2);

                /*
                printf("                Distancia_n1: %f\n", dist_n1);
                printf("\n");
                printf("                Distancia_n2: %f\n", dist_n2);
                printf("\n");
                */

                //Calculo de Gmjmi
                double mj_n1 = planeta_j_frame_n_menos_1->m;
                double mi_n1 = planeta_i_frame_n_menos_1->m;
                double Gmjmi_n1 = G * mj_n1 * mi_n1;
                double dist_2_n1 = dist_n1 * dist_n1;

                double mj_n2 = planeta_j_frame_n_menos_2->m;
                double mi_n2 = planeta_i_frame_n_menos_2->m;
                double Gmjmi_n2 = G * mj_n2 * mi_n2;
                double dist_2_n2 = dist_n2 * dist_n2;


                //Calculamos la fuerza
                vector2 gravedad_n1 = F_gravedad(G, mi_n1, mj_n1, dx_n1, dy_n1);
                double F_x_n1 = gravedad_n1.x;
                double F_y_n1 = gravedad_n1.y;

                vector2 gravedad_n2 = F_gravedad(G, mi_n2, mj_n2, dx_n2, dy_n2);
                double F_x_n2 = gravedad_n2.x;
                double F_y_n2 = gravedad_n2.y;


                //Aceleraciones
                double ai_x_n1 =  F_x_n1 / mi_n1;
                double ai_y_n1 =  F_y_n1 / mi_n1;
                double aj_x_n1 = 0.0;
                double aj_y_n1 = 0.0;

                double ai_x_n2 =  F_x_n2 / mi_n2;
                double ai_y_n2 =  F_y_n2 / mi_n2;
                double aj_x_n2 = 0.0;
                double aj_y_n2 = 0.0;

                //Planeta actual
                cuerpo2d* planeta_i = &planetas[index_frame_n_menos_1 + planetas_number + i];
                cuerpo2d* planeta_j = &planetas[index_frame_n_menos_1 + planetas_number + j];
                
                //Esto por ahora asi XD TODO: Quizas buscar otra manera
                double mj_n = mj_n1;
                double mi_n = mi_n1;

                // x(n) = 2x(n-1) - x(n-2) + a(n-1)dt2
                
                double i_posx_n = 2 * i_posx_n1 - i_posx_n2 + ai_x_n1 * dt * dt;
                double i_posy_n = 2 * i_posy_n1 - i_posy_n2 + ai_y_n1 * dt * dt;
                double j_posx_n = 0.0;
                double j_posy_n = 0.0;

                //printf("                (pix_n, piy_n) = (%f, %f)\n", i_posx_n, i_posy_n);
                //printf("                (pjx_n, pjy_n) = (%f, %f)\n", j_posx_n, j_posy_n);


                //Por mantener consistencia, vamos a calcular velocidad ES UN PUTO COÑAZO
                // v(n) = v(n-1) * 1/6 * (2a(n) + 5a(n-1) - a(n-2)) * dt
                // Primero hay que calcular a(n)
                double dx_n = j_posx_n - i_posx_n;
                double dy_n = j_posy_n - i_posy_n;

                double dist_n = vector2_module(dx_n, dy_n);
                double dist_n_2 = dist_n * dist_n;
                
                //printf("                Distancia: %f\n", dist_n);
                //printf("\n");
                //printf("%f\n", dist_n);

                double Gmjmi_n = G * mj_n * mi_n;

                vector2 gravedad_n = F_gravedad(G, mi_n, mj_n, dx_n, dy_n);
                double F_x_n = gravedad_n.x;
                double F_y_n = gravedad_n.y;

                double ai_x_n =  F_x_n / mi_n;
                double ai_y_n =  F_y_n / mi_n;
                double aj_x_n = 0.0;
                double aj_y_n = 0.0;

                double un_sexto = (double) 1 / 6;

                double i_vx_n = i_vx_n1 + un_sexto * (2 * ai_x_n + 5 * ai_x_n1 - ai_x_n2) * dt;
                double i_vy_n = i_vy_n1 + un_sexto * (2 * ai_y_n + 5 * ai_y_n1 - ai_y_n2) * dt;
                double j_vx_n = 0.0;
                double j_vy_n = 0.0;

                //Las constantes las voy a pasar por aqui por ahora
                planeta_i->m = planeta_i_frame_n_menos_1->m;
                planeta_i->r = planeta_i_frame_n_menos_1->r;
                planeta_j->m = planeta_j_frame_n_menos_1->m;
                planeta_j->r = planeta_j_frame_n_menos_1->r;


                planeta_i->pos_x = i_posx_n;
                planeta_i->pos_y = i_posy_n;
                planeta_i->v_x   = i_vx_n;
                planeta_i->v_y   = i_vy_n;

                planeta_j->pos_x = 0.0;
                planeta_j->pos_y = 0.0;
                planeta_j->v_x   = 0.0;
                planeta_j->v_y   = 0.0;

                /*
                printf("                (pix, piy) = (%f, %f)\n", i_posx_n, i_posy_n);
                printf("                (pjx, pjy) = (%f, %f)\n", j_posx_n, j_posy_n);
                */
            }
        }
        //printf("\n\n\n");
    }
}

// Calcula la gravedad que sentirian cada pareja de planetas si estuvieran solos
// TODO: Esta funcion deberia tener G como input, probablemente const double G
void gravedades_calc(cuerpo2d* frame, int frame_len, vector2* buffer, int buffer_size) {
    int counter = 0;
    for (int j = 0; j < frame_len - 1; j++) {
        for (int i = j + 1; i < frame_len; i++) {
            
            #if GRAVITY_DEBUG
            if (counter > buffer_size) {
                printf("Se estan realizando más calculos que iteracciones hay en el sistema en gravedades_calc()\n");
                return;
            }
            #endif

            double dx = frame[j].pos_x - frame[i].pos_x;
            double dy = frame[j].pos_y - frame[i].pos_y;
           
            #if GRAVITY_DEBUG 
            printf("    dist = %f\n", vector2_module(dx,dy));
            #endif

            double mi = frame[i].m;
            double mj = frame[j].m;
            vector2 gra = F_gravedad(G, mi, mj, dx, dy);
            
            #if GRAVITY_DEBUG
            printf("    gra = (%f, %f)\n", gra.x, gra.y);
            #endif

            buffer[counter] = gra;
            counter++;
        }
    }
}

// Double pointer porque un array2d es un array de arrays
void gravedades_to_gra_matrix(vector2 *buffer, int buffer_size, int planetas_number, vector2 gra_matrix[planetas_number][planetas_number]) {
    /*
        |  |p1|p2|p3|p4|
        |p1|  |00|01|02|
        |p2|0'|  |03|04|
        |p3|1'|3'|  |05|
        |p4|2'|4'|5'|  |
    */
    int counter = 0;
    vector2 cero = {0.0, 0.0};

    for (int j = 0; j < planetas_number; j++) {
        for (int i = 0; i < planetas_number; i++) {
            gra_matrix[j][i] = cero;
        }
    }

    // Este -1 no se puede quitar, por lo que los "valores espejo" hay que añadirlos despues CREO
    for (int j = 0; j < planetas_number - 1; j++) {
        for (int i = j + 1; i < planetas_number; i++) {
            
            #if GRAVITY_DEBUG           
            if (counter > buffer_size) {
                printf("Se estan asignados mas valores la matrix de los que hay en el buffer en gravedades_to_gra_matrix()\n");
                return;
            }
            #endif

            if (i > j) {
                //Original
                gra_matrix[j][i] = buffer[counter];
                counter++;
            } else if (i == j) {
                gra_matrix[j][i] = cero;
            } else {
                // TODO: Esto es provisional, quitar de aqui o añadir algo para el preprocesador
                // este no se como lo voy a quitar por ahora
                printf("Esto no deberia pasar gravedades_to_gra_matrix()\n");
                return;
            }
        }
    }

    for (int j = 0; j < planetas_number; j++) {
        for (int i = 0; i < planetas_number; i++) {
            if (i < j) {
                gra_matrix[j][i].x = -gra_matrix[i][j].x;
                gra_matrix[j][i].y = -gra_matrix[i][j].y;
            }
        }
    }
}

void gra_matrix_to_gra_sum(int planetas_number, vector2 gra_matrix[planetas_number][planetas_number], vector2* gra_sum) {
    vector2 cero = {0.0, 0.0};
    for (int j = 0; j < planetas_number; j++) {
        gra_sum[j] = cero;
        for (int i = 0; i < planetas_number; i++) {
            gra_sum[j].x += gra_matrix[j][i].x;
            gra_sum[j].y += gra_matrix[j][i].y;
        }
    }
}

void gra_to_aceleracion(cuerpo2d* frame, vector2* gra_sum, int planetas_number) {
    for (int p = 0; p < planetas_number; p++) {
        gra_sum[p].x = gra_sum[p].x / frame[p].m;
        gra_sum[p].y = gra_sum[p].y / frame[p].m;
    }
}

// v(n) = v(n-1) + a(n) * dt
// v(n-1) se puede pensar como la velocidad inicial
vector2 euler_velocity(vector2 v_0, vector2 a, double dt) {
    vector2 vel = {
        .x = v_0.x + a.x * dt,
        .y = v_0.y + a.y * dt,
    };
    return vel;
}

// x(n) = x(n-1) + v(n-1) * dt + 1/2 * a(n) * dt * dt
// De nuevo los valores x(n-1) y v(n-1) se pueden considerar los iniciales
vector2 euler_pos(vector2 x_0, vector2 v_0, vector2 a, double dt) {
    vector2 pos = {
        .x = x_0.x + v_0.x * dt + (double)1/2 * a.x * dt * dt,
        .y = x_0.y + v_0.y * dt + (double)1/2 * a.y * dt * dt,
    };
    return pos;
}

// En teoria solo con una frame deberia funcionar (el frame es un pointer al array planetas), pero prefiero los dos
void euler_integration(cuerpo2d* frame_n_1, cuerpo2d* frame, int frame_len, vector2* a, double dt) {
    for (int p = 0; p < frame_len; p++) {
        vector2 v_n_1 = {frame_n_1[p].v_x, frame_n_1[p].v_y};
        vector2 pos_n_1 = {frame_n_1[p].pos_x, frame_n_1[p].pos_y};

        vector2 v = euler_velocity(v_n_1, a[p], dt);
        vector2 pos = euler_pos(pos_n_1, v_n_1, a[p], dt);

        frame[p].v_x = v.x;
        frame[p].v_y = v.y;
        frame[p].pos_x = pos.x;
        frame[p].pos_y = pos.y;
    }
}

void cuerpos_simular_euler_2(cuerpo2d *planetas, int planetas_number, cuerpo2d *planetas_t0, int frames, double dt) {

    printf("cuerpos_simular_euler_2(), inicio\n");

    //Condiciones Iniciales
    for (int p = 0; p < planetas_number; p++) {
        planetas[p] = planetas_t0[p];
    }
    printf("    Despues de aplicar las Condiciones Iniciales\n");

    // Darle valor al array entero para las comprobaciones
    for (int p = 0; p < planetas_number * frames; p++) {
        planetas[p] = planetas_t0[p % planetas_number];
    }
    printf("    Despues de darle valor al array entero\n");

    // Creo que el buffer lo quiero fuera para que no se genere uno nuevo por frame
    // Aunque si lo piensas deberia dar igual con suficientes optimizaciones
    // los bucles no se deserrollan en enamblador a no ser que sepan de antemano
    // cuantos loops deben hacer (supongo)
    int buffer_size = calc_buffer_size(planetas_number);
    
    printf("    dist_buffer_size = %d\n", buffer_size);
    
    vector2 buffer[buffer_size];
    vector2 gra_sum[planetas_number];
    vector2 gra_matrix[planetas_number][planetas_number];

    printf("    Antes del loop de la simulacion\n");
    // Estamos con euler, deberia empezar en 1 (si esta en 0 es porque estoy probando que funciona hasta el 
    // calculo de las aceleraciones)
    for (int f = 1; f < frames; f++) {

        #if GRAVITY_DEBUG
        printf("f: %d\n", f);
        #endif

        cuerpo2d* frame_n1 = &planetas[(f - 1) * planetas_number];
        cuerpo2d* frame = &planetas[f * planetas_number];
        gravedades_calc(frame_n1, planetas_number, buffer, buffer_size);

        #if GRAVITY_DEBUG
        for (int i = 0; i < buffer_size; i++) {
            printf("    gravedad[%d] = (%f, %f)\n", i, buffer[i].x, buffer[i].y);
        }
        #endif

        gravedades_to_gra_matrix(buffer, buffer_size, planetas_number, gra_matrix);

        #if GRAVITY_DEBUG
        for (int j = 0; j < planetas_number; j++) {
            for (int i = 0; i < planetas_number; i++) {
                printf("(%f, %f)", gra_matrix[j][i].x, gra_matrix[j][i].y);
            }
            printf("\n");
        }
        #endif

        gra_matrix_to_gra_sum(planetas_number, gra_matrix, gra_sum);

        #if GRAVITY_DEBUG
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        gra_to_aceleracion(frame_n1, gra_sum, planetas_number);

        #if GRAVITY_DEBUG
        printf("\n");
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        euler_integration(frame_n1, frame, planetas_number, gra_sum, dt);
    }
    printf("    Despues del loop\n");
}

// x(n) = 2x(n-1) - x(n-2) + a(n-1) * dt * dt
vector2 verlet_pos(vector2 x_n_1, vector2 x_n_2, vector2 a_n_1, double dt) {
    vector2 pos = {
        .x = 2 * x_n_1.x - x_n_2.x + a_n_1.x * dt * dt,
        .y = 2 * x_n_1.y - x_n_2.y + a_n_1.y * dt * dt,
    };
    return pos;
}

// v(n) = v(n-1) + 1/6 * (2a(n) + 5a(n-1) - a(n-2)) * dt
vector2 verlet_velocity(vector2 v_n_1, vector2 a, vector2 a_n_1, vector2 a_n_2, double dt) {
    vector2 vel = {
        .x = v_n_1.x + (double)1/6 * (2 * a.x + 5 * a_n_1.x - a_n_2.x) * dt,
        .y = v_n_1.y + (double)1/6 * (2 * a.y + 5 * a_n_1.y - a_n_2.y) * dt,
    };
    return vel;
}

void verlet_integration(cuerpo2d* frame_n_2, cuerpo2d* frame_n_1, cuerpo2d* frame, int frame_len,
                        vector2* a, vector2* a_n_1, vector2* a_n_2, double dt)
{ 
    for (int p = 0; p < frame_len; p++) {
        vector2 pos_n1 = {frame_n_1[p].pos_x, frame_n_1[p].pos_y};
        vector2 pos_n2 = {frame_n_2[p].pos_x, frame_n_2[p].pos_y};
        vector2 a_n1 = a_n_1[p];
        vector2 a_n2 = a_n_2[p];
        vector2 a_n = a[p];
        vector2 v_n1 = {frame_n_1[p].v_x, frame_n_1[p].v_y};

        vector2 pos = verlet_pos(pos_n1, pos_n2, a_n1, dt);
        vector2 v = verlet_velocity(v_n1, a_n, a_n1, a_n2, dt);

        frame[p].pos_x = pos.x;
        frame[p].pos_y = pos.x;
        frame[p].v_x = v.x;
        frame[p].v_x = v.y;
    }
}

// La primera vez que verlet se los encuentre a_n2 deberia contener 0 (Condiciones iniciales)
// y a_n1 deberia tener los valores calculados por euler
// Despues verlet (en el loop, no en la integracion) hace uso de esos valores y asigna al final del loop
// a a_n1 el valor de a y a a_n2 el valor de a_n1
void cuerpos_simular_verlet_2(cuerpo2d *planetas, int planetas_number, cuerpo2d *planetas_t0, int frames, double dt) {

    printf("cuerpos_simular_verlet_2(), inicio\n");

    //Condiciones Iniciales
    for (int p = 0; p < planetas_number; p++) {
        planetas[p] = planetas_t0[p];
    }
    printf("    Despues de aplicar las Condiciones Iniciales\n");

    // Darle valor al array entero para las comprobaciones
    for (int p = 0; p < planetas_number * frames; p++) {
        planetas[p] = planetas_t0[p % planetas_number];
    }
    printf("    Despues de darle valor al array entero\n");

    // Creo que el buffer lo quiero fuera para que no se genere uno nuevo por frame
    // Aunque si lo piensas deberia dar igual con suficientes optimizaciones
    // los bucles no se deserrollan en enamblador a no ser que sepan de antemano
    // cuantos loops deben hacer (supongo)
    int buffer_size = calc_buffer_size(planetas_number);
    
    printf("    dist_buffer_size = %d\n", buffer_size);
    
    vector2 buffer[buffer_size];
    vector2 gra_sum[planetas_number];
    vector2 gra_matrix[planetas_number][planetas_number];
    vector2 a_n1_buffer[planetas_number];
    vector2 a_n2_buffer[planetas_number];

    printf("    Antes del frame calculado con euler\n");
    // Solo un frame, porque este se calcula con euler
    // TODO: Mejorear esto
    for (int f = 1; f < 2; f++) {

        #if GRAVITY_DEBUG
        printf("f: %d\n", f);
        #endif

        cuerpo2d* frame_n1 = &planetas[(f - 1) * planetas_number];
        cuerpo2d* frame = &planetas[f * planetas_number];
        gravedades_calc(frame_n1, planetas_number, buffer, buffer_size);

        #if GRAVITY_DEBUG
        for (int i = 0; i < buffer_size; i++) {
            printf("    gravedad[%d] = (%f, %f)\n", i, buffer[i].x, buffer[i].y);
        }
        #endif

        gravedades_to_gra_matrix(buffer, buffer_size, planetas_number, gra_matrix);

        #if GRAVITY_DEBUG
        for (int j = 0; j < planetas_number; j++) {
            for (int i = 0; i < planetas_number; i++) {
                printf("(%f, %f)", gra_matrix[j][i].x, gra_matrix[j][i].y);
            }
            printf("\n");
        }
        #endif

        gra_matrix_to_gra_sum(planetas_number, gra_matrix, gra_sum);

        #if GRAVITY_DEBUG
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        gra_to_aceleracion(frame_n1, gra_sum, planetas_number);

        #if GRAVITY_DEBUG
        printf("\n");
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        euler_integration(frame_n1, frame, planetas_number, gra_sum, dt);
    }
    printf("    Despues del frame calculado por euler\n");

    // Inicializar los bufferes
    // En este momento gra_sum tiene los valores de las aceleraciones calculadas por euler
    vector2 cero = {0, 0};
    for (int p = 0; p < planetas_number; p++) {
        a_n1_buffer[p] = gra_sum[p];
        a_n2_buffer[p] = cero;
    }

    printf("    Antes de la simulacion\n");
    // Parte simulada con verlet
    for (int f = 2; f < frames; f++) {

        #if GRAVITY_DEBUG
        printf("f: %d\n", f);
        #endif

        cuerpo2d* frame_n2 = &planetas[(f - 2) * planetas_number];
        cuerpo2d* frame_n1 = &planetas[(f - 1) * planetas_number];
        cuerpo2d* frame = &planetas[f * planetas_number];
        gravedades_calc(frame_n1, planetas_number, buffer, buffer_size);

        #if GRAVITY_DEBUG
        for (int i = 0; i < buffer_size; i++) {
            printf("    gravedad[%d] = (%f, %f)\n", i, buffer[i].x, buffer[i].y);
        }
        #endif

        gravedades_to_gra_matrix(buffer, buffer_size, planetas_number, gra_matrix);

        #if GRAVITY_DEBUG
        for (int j = 0; j < planetas_number; j++) {
            for (int i = 0; i < planetas_number; i++) {
                printf("(%f, %f)", gra_matrix[j][i].x, gra_matrix[j][i].y);
            }
            printf("\n");
        }
        #endif

        gra_matrix_to_gra_sum(planetas_number, gra_matrix, gra_sum);

        #if GRAVITY_DEBUG
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        gra_to_aceleracion(frame_n1, gra_sum, planetas_number);

        #if GRAVITY_DEBUG
        printf("\n");
        for (int i = 0; i < planetas_number; i++) {
            printf("(%f, %f)\n", gra_sum[i].x, gra_sum[i].y);
        }
        #endif

        // Verlet_integration
        verlet_integration(frame_n2, frame_n1, frame, planetas_number, gra_sum, a_n1_buffer, a_n2_buffer, dt);

        // Actualizar buffer para el siguiente frame
        for (int p = 0; p < planetas_number; p++) {
            a_n2_buffer[p] = a_n1_buffer[p];
            a_n2_buffer[p] = gra_sum[p];
        }
    }
    printf("    Despues de la simulacion\n");
}
