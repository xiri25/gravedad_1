#ifndef ORBIT_H
#define ORBIT_H

#include "../cuerpos/cuerpos.h"
#include "../math_utils/math_utils.h"


// Change the origin of the frame of reference to a new fixed point for all the frames 
void ORBIT_new_constant_OFR_2d(cuerpo2d* planetas, int planetas_number, int frames, vector2 new_origin);

// Change the origin of the frame of referencce to a new point for all the frames
// This new point is a vector2 array with frames number of vector2, one for each position
// of the new frame of reference
// Expect an array of positions for the new point of reference, one position per frame
void ORBIT_new_variable_OFR_2d(cuerpo2d *planetas, int planetas_number, int frames, vector2* new_origin);

// Change the origin of the frame of reference to a new point, the location of a planet of idx planeta_idx
// each frame. planeta_idx is the index of the body in the initial conditions
void ORBIT_new_OFR_2d_is_body(cuerpo2d *planetas, int planetas_number, int frames, int planeta_idx);
#endif
