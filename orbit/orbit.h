#ifndef ORBIT_H
#define ORBIT_H

#include "../math_utils/math_utils.h"

typedef struct {

    // TODO: Añadir la matriz cambio de base, pensar si dejar la normal

    double e; // ellipticity
    vector3 L; // por ahora creo que lo prefiero en forma de vector3
    vector3 focus;
    vector3 normal; // normal of the plane of the orbit
    double M; // Mas of the object that creates the gravitational field
    double alpha; // angle between x-axis of the absolute coord system and the x-axis of the local coord system
    double theta; // angle between the radius and the x-axis of the local coord system
} orbit_param;

#endif // !ORBIT_H
