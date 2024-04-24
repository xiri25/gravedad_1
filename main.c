#include <stdio.h>


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
  double x;
  double y;

  //Asumimos una distribucion de masa cte en una circunferencia
  double r;
  double m;
} cuerpo2d;

int main() {
  int size = sizeof(double);
  printf("Un double mide: %d bytes, %d bits\n", size, size * 8);
  return 0;
}
