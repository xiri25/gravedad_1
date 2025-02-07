#ifndef ORBIT_H
#define ORBIT_H

#include "../math_utils/math_utils.h"

typedef struct {
    double e; // ellipticity
    vector3 L; // por ahora creo que lo prefiero en forma de vector3
    vector3 focus;
    vector3 normal; // normal of the plane of the orbit
    double m; // Mas of the object that creates the gravitational field
    double a; // angle between x-axis of the absolute coord system and the x-axis of the local coord system
    double theta; // angle between the radius and the x-axis of the local coord system
} orbit_param;

#endif // !ORBIT_H
