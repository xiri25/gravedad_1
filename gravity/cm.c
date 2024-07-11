#include "cm.h"

vector2 CM_pos_frame(cuerpo2d* frame, int frame_len)
{ 
    // Como si la masa pudiera cambiar en cada frame
    double M = 0;
    for (int p = 0; p < frame_len; p++) {
        M += frame[p].m;
    }

    vector2 mx = {0.0, 0.0};
    for (int p = 0; p < frame_len; p++) {
        mx.x += frame[p].pos_x * frame[p].m / M;
        mx.y += frame[p].pos_y * frame[p].m / M;
    }

    return mx;
}

void Center_mass_calc(vector2* CM, cuerpo2d* planetas, int planetas_number, int frames)
{
    for (int f = 0; f < frames; f++) {
        CM[f] = CM_pos_frame(&planetas[f * planetas_number], planetas_number);
    }
}
