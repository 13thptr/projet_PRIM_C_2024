#ifndef LUT_H
#define LUT_H

#include "pictures.h"

/*NB: la consigne spécifie la création d'un type ABSTRAIT. On le précisera donc dans lut.c*/
typedef struct lut_s* lut;
/*
 * create_lut
 * @param [in]: n
 * 
 * @requires: n non nul
 * @assigns: création dans le tas d'une lut.
 * @ensures: res->array pointeur vers un tableau de bytes de taille n
 * 
 * @returns: res
 * 
*/
lut create_lut(unsigned int n);


/*
 * clean_lut
 * @param [in]: table
 * 
 * @requires: table not null
 * @assigns: modifie le tas
 * @ensures: table-> array et table sont libérés.
 * 
 * @returns: rien
 * 
*/
void clean_lut(lut *table);


/*
 * apply_lut
 * @param [in]: p
 * @param [in]: table
 * @requires: p valid picture
 * @assigns: modifie le tas en modifiant "en place" l'image passée en paramètre par application de la LUT table
 * @ensures: p est modifiée selon lut, ou alors la fonction a planté suite à un assert.
 * 
 * @returns: rien (modification par effet)
 * 
*/
void apply_lut(picture p, lut table);


#endif /*LUT_H*/