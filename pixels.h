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
/**
 * Ecriture du pixel/triplet de valeurs red,green,blue dans la "case" i,j du champ "data" de p.
 * @param [in] p la structure "picture" à modifier.
 * @param [in] i ligne du pixel à modifier
 * @param [in] j colonne du pixel à modifier
 * 
 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures red,green,blue entre 0 et 255.
 * 
 * @assigns p.data
 *   
 * @return rien
 * 
 */
void write_pixel_rgb(picture p,int i, int j, byte red, byte green, byte blue);

/**
 * Ecriture du pixel/triplet de valeur "value" dans la "case" i,j du champ "data" de p.
 * @param [in] p la structure "picture" à modifier.
 * @param [in] i ligne du pixel à modifier
 * @param [in] j colonne du pixel à modifier
 * 
 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures value entre 0 et 255.
 * 
 * @assigns p.data
 *   
 * @return rien
 * 
 */
void write_pixel_bw(picture p, int i, int j, byte value);

#endif /*PIXELS_H*/