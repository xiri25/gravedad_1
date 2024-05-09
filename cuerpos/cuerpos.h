#ifndef CUERPOS
#define CUERPOS

// Cualquier cosa con masa suficiente como para tenerla en cuenta
typedef struct {
    double pos_x;
    double pos_y;

    //Asumimos una distribucion de masa cte en una circunferencia
    double r;
    double m;
} cuerpo2d;

#endif
