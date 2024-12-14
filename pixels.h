/*our accéder à la composante k∈[0⋯c−1] d’un pixel situé à la ligne i∈[0⋯h−1] 
et à la colonne j∈[0⋯w−1] d’une image couleur p on pourra donc utiliser ...data[((i * w + j) * c) + k].
Vous pourrez isoler les opérations liées à la lecture ou à l’écriture d’une composante de pixel (i,j,c)

dans une image dans un sous module pixels.[h/c] dans lequel pour pourrez aussi avantageusement
 définir des constantes symboliques (en utilisant un enum par exemple) RED, GREEN et BLUE pour accéder directement à ces composantes dans les pixels.
*/
#include "pictures.h"
#ifndef PIXELS_H
#define PIXELS_H

enum PIXEL_TYPES{RED,GREEN,BLUE};


void write_component_rgb(picture p, int i, int j, int value, enum PIXEL_TYPES c);

void write_pixel_rgb(picture p,int i, int j, byte red, byte green, byte blue);

/*Since write_component_bw would be the same as write_pixel_bw, there's no need to write it.*/
void write_pixel_bw(picture p, int i, int j, byte value);


/*
Lecture: 1) par retour, composante par composante 2)par effet, pixel par pixel.
*/

byte read_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c);
byte read_component_bw(picture p, int i, int j);

/*read_pixel_rgb might not be as useful as the previous function read_component_rgb.*/
void read_pixel_rgb(picture p, int i, int j, byte *red, byte *green, byte *blue);
/*read_pixel_bw might not be as useful as the previous function read_component_bw.*/
void read_pixel_bw(picture p, int i, int j, byte *value);

#endif /*PIXELS_H*/