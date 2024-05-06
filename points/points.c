#include "points.h"
#include <stdlib.h>


// En main tengo que alojar la memoria necesaria (porque la voy a liberar en main y asi es mas claro)
// podemos hacer un wrapper para calcular cuanta
void* malloc_points(grid grid, int frames) {
  int points_num = grid.height * grid.length;
  int points_size = sizeof(point2d_64);
  int size = points_num * points_size * frames;
  void* grid_ptr = malloc(size);
  return grid_ptr;
}

void inicilizar_points(point2d_64* points, grid grid, int frames) {
  
  int total_points = grid.length * grid.height;

  for (int f = 0; f < frames; f++) {
    for (int i = 0; i < grid.length; i++) {
      for (int j = 0; j < grid.height; j++) {

        int index = f * total_points + i * grid.height + j;
        
        point2d_64 point = {
          .pos_x = i,
          .pos_y = j,
          .vector_x = 0,
          .vector_y = 0,
          .mod_vector = 0,
          .norm_vec_x = 0,
          .norm_vec_y = 0
        };
        
        points[index] = point;
      }
    }
  }
}
