#include "frame_reference.h"

#include <stdio.h>

// TODO: Los cambios en el frame of reference se tienen que implementar tabien para los vectores del campo

// OFR => Origin of Frame of Reference
// Esta es para los cuerpos
void change_OFR_2d_frame(cuerpo2d* frame, int frame_len, vector2 new_origin)
{
    for (int p = 0; p < frame_len; p++) {
        frame[p].pos_x -= new_origin.x;
        frame[p].pos_y -= new_origin.y;
    }
}

void FR_new_constant_OFR_2d(cuerpo2d* planetas, int planetas_number, int frames, vector2 new_origin)
{
    for (int f = 0; f < frames; f++) {
        change_OFR_2d_frame(&planetas[f * planetas_number], planetas_number, new_origin);
    }
}

void FR_new_variable_OFR_2d(cuerpo2d *planetas, int planetas_number, int frames, vector2* new_origin)
{
    /*
    * Se supone que:
    * planetas.len() = new_origin.len() = frames
    */ 
    for (int f = 0; f < frames; f++) {
        change_OFR_2d_frame(&planetas[f * planetas_number], planetas_number, new_origin[f]);
    }
}

void FR_new_OFR_2d_is_body(cuerpo2d *planetas, int planetas_number, int frames, int planeta_idx)
{
    if (planeta_idx > planetas_number || planeta_idx < 0) {
        printf("FR_OFR_2d_is_body(): El indice del planeta proporcionado (%d) no es correcto\n", planeta_idx);
        return;
    }

    for (int f = 0; f < frames; f++) {
        vector2 planeta_pos = {
            planetas[f * planetas_number + planeta_idx].pos_x,
            planetas[f * planetas_number + planeta_idx].pos_y,
        };
        change_OFR_2d_frame(&planetas[f * planetas_number], planetas_number, planeta_pos);
    }
}
