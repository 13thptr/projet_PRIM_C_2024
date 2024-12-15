#ifndef LUT_H
#define LUT_H

#include "pictures.h"
/*NB: la consigne spécifie la création d'un type ABSTRAIT. On le précisera donc dans lut.c*/

typedef struct lut_s* lut;

lut create_lut(unsigned int n);
void clean_lut(lut *table);
/*
Vu la formulation de l'énoncé, on modifiera les images par effet avec les LUT. On pourra 
cependant créer une version qui renvoie une nouvelle image.
*/
void apply_lut(picture p, lut table);


#endif /*LUT_H*/