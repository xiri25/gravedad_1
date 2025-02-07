#ifndef CUERPOS
#define CUERPOS

#include "../math_utils/math_utils.h"

// Cualquier cosa con masa suficiente como para tenerla en cuenta
typedef struct {
    double pos_x;
    double pos_y;
    double v_x;
    double v_y;

    //Asumimos una distribucion de masa cte en una circunferencia
    double r;
    double m;
} cuerpo2d;

// voy a probar con vectores, deberia ser igual
typedef struct {
    vector3 pos;
    vector3 v;
    double r;
    double m;
} cuerpo3d;

#endif
