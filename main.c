#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ITERATIONS 100

// Podria probar con una version de 32bits
// En teoria si la struct esta compuesta por
// elementos de un tipo, va a ser compacta en memoria
typedef struct {

  // Posicion
  double pos_x;
  double pos_y;

  // Valor del campo
  double vector_x;
  double vector_y;

  // Esto no se si lo quiero guardar aqui
  double mod_vector;
  double norm_vec_x;
  double norm_vec_y;
} point2d_64;

// Cualquier cosa con masa suficiente como para tenerla en cuenta
typedef struct {
  double pos_x;
  double pos_y;

  //Asumimos una distribucion de masa cte en una circunferencia
  double r;
  double m;
} cuerpo2d;

cuerpo2d mover_tierra(double radio_orbita, double w, int t) {
  // Por ahora un  movimiento circular
  double x, y;
  x = radio_orbita * cos(w*t);
  y = radio_orbita * sin(w*t);

  cuerpo2d cuerpo;

  cuerpo.pos_x = x;
  cuerpo.pos_y = y;
  cuerpo.r = 2;
  cuerpo.m = 2;
  return cuerpo;
}

cuerpo2d mover_luna(cuerpo2d tierra, double radio_orbita, double w, int t) {
  double dx, dy;
  dx = radio_orbita * cos(w*t);
  dy = radio_orbita * sin(w*t);

  double x, y;
  x = tierra.pos_x + dx;
  y = tierra.pos_y + dy;

  cuerpo2d cuerpo;
  cuerpo.pos_x = x;
  cuerpo.pos_y = y;
  cuerpo.r = 1;
  cuerpo.m = 1;

  return cuerpo;
}

void escribir_archivo(cuerpo2d *resultados) {
  FILE *fptr;
  fptr = fopen("resultados.txt", "w");
  
  fprintf(fptr, "Hola\n");
  fprintf(fptr, "%p\n", resultados);

  fclose(fptr);
}

int main() {
  int size = sizeof(double);
  printf("Un double mide: %d bytes, %d bits\n", size, size * 8);

  // Primero vamos a hacer que los "planetas" se muevan
  cuerpo2d sol = {0, 0, 100, 100};
  cuerpo2d tierra = {10, 10, 2, 2};
  cuerpo2d luna = {11, 11, 1, 1};

  int planetas_size = sizeof(cuerpo2d) * ITERATIONS * 3;
  cuerpo2d *planetas = (cuerpo2d *)malloc(planetas_size);
  if (planetas == NULL) {
    printf("Malloc failed alojando el array planetas\n");
    return 1;
  }
  planetas[0] = sol;
  planetas[1] = tierra;
  planetas[2] = luna;

  printf("D. Memoria de planetas: %p\n", &planetas);

  //En este toy problem el sol es estático y punto de referencia
  printf("planetas_size: %d\n", planetas_size);
  int planetas_number = ITERATIONS * 3;
  printf("planetas_number: %d\n", planetas_number);
  
  double r_orbita_tierra_sol = sqrt(10*10+10*10);
  
  // (11 - 10)**2 + (11 - 10)**2
  double r_orbita_luna_tierra = sqrt(2);
  for (int i = 0; i < planetas_number; i += 3) {
    //printf("%d\n", i);
    planetas[i] = sol;
    planetas[i+1] = mover_tierra(r_orbita_tierra_sol, 0.01, i);
    planetas[i+2] = mover_luna(planetas[i+1], r_orbita_luna_tierra, 0.005, i);
  }

  printf("Resultados, Sol.pos_x, Sol.pos_y; Tierra.pos_x, Tierra.pos_y; Luna.pos_x, Luna.pos_y\n");
  for (int i = 0; i < planetas_number; i += 3) {
    printf("%f, %f; %f, %f\n",
           /* planetas[i].pos_x, planetas[i].pos_y, */
           planetas[i + 1].pos_x, planetas[i + 1].pos_y,
           planetas[i + 2].pos_x, planetas[i + 2].pos_y);
  }

  escribir_archivo(planetas);

  //DO NOT FORGET
  free(planetas);
  return 0;
}
