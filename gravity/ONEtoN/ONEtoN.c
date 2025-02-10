#include "ONEtoN.h"
#include <math.h>

#include <stdio.h>

// TODO: Generación de las cond. ininciales y los parametros de las orbitas

// Esta funcion se ejecuta fuera del bucle, thanksGOD
void create_local_base_ortonormal(cMatrix3* base, const vector3* N, const vector3* r)
{
    vector3 v = vector3_cross_product(N, r);
    vector3_normalize_in_place(&v);
    vector3 N_n = vector3_normalize(N);
    vector3 r_n = vector3_normalize(r);

    // No hace daño comprobarlo, ya que estoy fuera del bucle
    if (vector3_dot_product(N, r) != 0)
    {
        printf("Error en create_local_base_ortonormal()\n");
        printf("N = (%f, %f, %f) y r = (%f, %f, %f) no son perpendiculares!!!!!\n",
               N->x, N->y, N->z, r->x, r->y, r->z);
        return;
    };

    if (vector3_dot_product(N, &v) != 0)
    {
        printf("Error en create_local_base_ortonormal()\n");
        printf("N = (%f, %f, %f) y v = (%f, %f, %f) no son perpendiculares!!!!!\n",
               N->x, N->y, N->z, v.x, v.y, v.z);
        return;
    };

    if (vector3_dot_product(&v, r) != 0)
    {
        printf("Error en create_local_base_ortonormal()\n");
        printf("v = (%f, %f, %f) y r = (%f, %f, %f) no son perpendiculares!!!!!\n",
               N->x, N->y, N->z, v.x, v.y, v.z);
        return;
    };

    base->m[0][0] = v.x;   base->m[0][1] = v.y;   base->m[0][2] = v.z;
    base->m[1][0] = N_n.x; base->m[1][1] = N_n.y; base->m[1][2] = N_n.z;
    base->m[2][0] = r_n.x; base->m[2][1] = r_n.y; base->m[2][2] = r_n.z;
}

// TODO: Esto pasa por no leer la teoria XDDDDDDDDDDD
// Solo necesito que sea capaz de crear la matriz de cambio de base a la base canonica :)
void create_change_base_matrix_to_ijk(cMatrix3* mcb, const cMatrix3* from)
{
    /*
     * base_from = {(a,b,c), (d,e,f), (g,h,i)}
     * base_to   = {(1,0,0), (0,1,0), (0,0,1)}
     *
     *              |A D G|
     * mcb = matrix |B E H|
     *              |C F I|
     *
     * (1,0,0) = A * (a,b,c) + B * (d,e,f) + C * (g,h,i)
     *      1 = Aa + Bd + Cg
     *      0 = Ab + Be + Ch
     *      0 = Ac + Bf + Ci
     *
     *              |1 d g|            |a 1 g|            |a d 1|
     *          det |0 e h|        det |b 0 h|        det |b e 0|
     *              |0 f i|            |c 0 i|            |c f 0|
     *      A =    ---------,  B =    ---------,  C =    ---------
     *              |a d g|            |a d g|            |a d g|
     *          det |b e h|        det |b e h|        det |b e h|
     *              |c f i|            |c f i|            |c f i|
     *
     * (0,1,0) = D * (a,b,c) + E * (d,e,f) + F * (g,h,i)
     *      0 = Da + Ed + Fg
     *      1 = Db + Ee + Fh
     *      0 = Dc + Ef + Fi
     *
     *              |0 d g|            |a 0 g|            |a d 0|
     *          det |1 e h|        det |b 1 h|        det |b e 1|
     *              |0 f i|            |c 0 i|            |c f 0|
     *      D =    ---------,  E =    ---------,  F =    ---------
     *              |a d g|            |a d g|            |a d g|
     *          det |b e h|        det |b e h|        det |b e h|
     *              |c f i|            |c f i|            |c f i|
     *
     * (0,0,1) = G * (a,b,c) + H * (d,e,f) + I * (g,h,i)
     *      0 = Ga + Hd + Ig
     *      0 = Gb + He + Ih
     *      1 = Gc + Hf + Ii
     *
     *              |0 d g|            |a 0 g|            |a d 0|
     *          det |0 e h|        det |b 0 h|        det |b e 0|
     *              |1 f i|            |c 1 i|            |c f 1|
     *      G =    ---------,  H =    ---------,  I =    ---------
     *              |a d g|            |a d g|            |a d g|
     *          det |b e h|        det |b e h|        det |b e h|
     *              |c f i|            |c f i|            |c f i|
    */

    double det_0 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});
    if (det_0 == 0)
    {
        printf("create_change_base_matrix_to_ijk(), el determinante de la base from es 0!!!!\n");
        return;
    }


    double det_1 = matrix3_by_col_det((vector3){1,0,0},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_2 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){1,0,1},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_3 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){1,0,0});


    double det_4 = matrix3_by_col_det((vector3){0,1,0},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_5 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){0,1,0},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_6 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){0,1,0});


    double det_7 = matrix3_by_col_det((vector3){0,0,1},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_8 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){0,0,1},
                                      (vector3){from->m[2][0], from->m[2][1], from->m[2][2]});

    double det_9 = matrix3_by_col_det((vector3){from->m[0][0], from->m[0][1], from->m[0][2]},
                                      (vector3){from->m[1][0], from->m[1][1], from->m[1][2]},
                                      (vector3){0,0,1});

    double A = det_1 / det_0;
    double B = det_2 / det_0;
    double C = det_3 / det_0;
    double D = det_4 / det_0;
    double E = det_5 / det_0;
    double F = det_6 / det_0;
    double Gg = det_7 / det_0; // TODO: define G es un crimen
    double H = det_8 / det_0;
    double I = det_9 / det_0;

     /*
     *              |A D G|
     * mcb = matrix |B E H|
     *              |C F I|
     */
    mcb->m[0][0] = A;
    mcb->m[1][0] = B;
    mcb->m[2][0] = C;
    mcb->m[0][1] = D;
    mcb->m[1][1] = E;
    mcb->m[2][1] = F;
    mcb->m[0][2] = Gg;
    mcb->m[1][2] = H;
    mcb->m[2][2] = I;
}


// Por ahora solo hay planetas orbitando focos estaticos
void cuerpos_simular_1toN_naive_euler_cteOrbit_v1(cuerpo3d* planetas, const cuerpo3d* planetas_t0,
                                      orbit_param* o_param, const int planetas_number,
                                      const double G_cte, const int frames, const double dt)
{
    // Aplicar las condiciones ininciales
    for (int p = 0; p < planetas_number; p++)
    {
        planetas[p] = planetas_t0[p];
    }

    for (int f = 1; f < frames; f++)
    {
        for (int p = 0; p < planetas_number; p++)
        {
            int idx = f * planetas_number + p;
            int idx_n1 = (f - 1) * planetas_number + p;

            /*
              r(theta) = A * ( 1 / (1 + e * cos(theta)) )
              w(theta) = B * (1 + e * cos(theta))**2
              A = L**2 / (G * M * m**2)
              B = (G**2 * M**2 * m**3) / L**3
            */

            const double L = vector3_module(&o_param[p].L);
            const double L_squared = L * L;
            const double L_cubed = L_squared * L;
            const double M = o_param[p].M;
            const double M_squared = M * M;
            const double m = planetas[idx_n1].m;
            const double m_squared = m * m;
            const double m_cubed = m_squared * m;
            const double e = o_param[p].e;
            const double theta = o_param[p].theta;
            const double A = L_squared / (G_cte * M * m_squared);
            const double B = (G_cte * G_cte * M_squared * m_cubed) / L_cubed;

            // Calcular la velocidad radial del frame anterior  TODO: Buffer this shit out
            const double w_n1 = B * (1.0 + e * cos(theta)) * (1.0 + e * cos(theta));

            // Euler Intergration
            const double new_theta = fmod(w_n1 * dt, 2.0 * M_PI); // Ojo a la precision

            // Calcular Nuevo Radio
            const double new_r = A * (1 / (1.0 * e * cos(new_theta)));

            // Traducir al sistema de coord absoluto
            const double new_alpha = fmod(new_theta + o_param[p].alpha, 2.0 * M_PI); // Ojo a la precision
            // vector r en el espacio local
            const vector3 new_vector_r_local = {
                .x = new_r * cos(new_theta),
                .y = new_r * sin(new_theta),
                .z = 0
            };
        }
    }
}
