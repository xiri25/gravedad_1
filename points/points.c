#include "points.h"
#include <stdlib.h>


// En main tengo que alojar la memoria necesaria (porque la voy a liberar en main y asi es mas claro)
// podemos hacer un wrapper para calcular cuanta
void* malloc_points(grid grid) {
  int points_num = grid.height * grid.length;
  int points_size = sizeof(point2d_64);
  int size = points_num * points_size;
  void* grid_ptr = malloc(size);
  return grid_ptr;
}


