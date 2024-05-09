#ifndef POINTS
#define POINTS

// Podria probar con una version de 32bits
// En teoria si la struct esta compuesta por
// elementos de un tipo, va a ser compacta en memoria
typedef struct {

    // Posicion
    double pos_x;
    double pos_y;

    // Valor del campo
//    double vector_x;
//    double vector_y;

    // Esto no se si lo quiero guardar aqui
    double mod_vector;
    double norm_vec_x;
    double norm_vec_y;
} point2d_64;

typedef struct {
      //El ingles es claramente un lenguaje th, ht XD
    int length;
    int height;
} grid;

int points_size(grid grid, int frames);
void inicilizar_points(point2d_64* points, grid grid, int frames);

#endif
