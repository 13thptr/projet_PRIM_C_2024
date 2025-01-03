#include "pictures.h"
#ifndef PIXELS_H
#define PIXELS_H

enum PIXEL_TYPES{RED,GREEN,BLUE};


void write_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c, int value);

void write_pixel_rgb(picture p,int i, int j, byte red, byte green, byte blue);

/*Since write_component_bw would be the same as write_pixel_bw, there's no need to write it.*/
void write_pixel_bw(picture p, int i, int j, byte value);

/*
    Lecture: 
        1)par retour, composante par composante
        2)par effet, pixel par pixel.
*/
byte read_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c);
byte read_component_bw(picture p, int i, int j);

/*read_pixel_rgb might not be as useful as the previous function read_component_rgb.*/
void read_pixel_rgb(picture p, int i, int j, byte *red, byte *green, byte *blue);
/*read_pixel_bw might not be as useful as the previous function read_component_bw.*/
void read_pixel_bw(picture p, int i, int j, byte *value);

#endif /*PIXELS_H*/