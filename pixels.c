#include <assert.h>
#include <stdlib.h>
#include "pixels.h"

/**
 * Ecriture de la valeur "value" dans le canal "c" du pixel d'indice i,j du champ "data" de p.
 * @param [in] p la structure "picture" à modifier.
 * @param [in] i ligne du pixel à modifier
 * @param [in] j colonne du pixel à modifier
 * @param [in] value valeur du pixel à modifier
 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures red,green,blue entre 0 et 255.
 * 
 * @assigns p.data
 *   
 * @return rien
 * 
 */
void write_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c,int value){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    assert(c==RED||c==GREEN||c==BLUE);
    int index =  ((i*p.width)+j)*3;
    p.data[index+c] = value;
}
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
void write_pixel_rgb(picture p, int i, int j, byte red, byte green, byte blue){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    int index =  ((i*p.width)+j)*3;
    p.data[index] = red;
    p.data[index+1] = green;
    p.data[index+2] = blue;
}
/**
 * Ecriture du pixel/singleton de valeur "value" dans la "case" i,j du champ "data" de p.
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
 */
void write_pixel_bw(picture p, int i, int j,byte value){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    p.data[(i*p.width)+j] = value;
}
/**
 * Lecture de la c-ème composante de la "case" i,j du champ "data" de p.
 * @param [in] p la structure "picture" à lire.
 * @param [in] i ligne du pixel à lire (entier)
 * @param [in] j colonne du pixel à lire (entier)
 * @param [in] c la composante à extraire.

 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures c entre 0 et 2.
 * 
 * @assigns rien
 *   
 * @return p.data[(i*p.width+j)*3+c];
 */
byte read_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    assert(c==RED||c==GREEN||c==BLUE);
    return p.data[(i*p.width+j)*3+c];
}
/**
 * Lecture de l'unique composante de la "case" i,j du champ "data" de p.
 * @param [in] p la structure "picture" à lire.
 * @param [in] i ligne du pixel à lire (entier)
 * @param [in] j colonne du pixel à lire (entier)

 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * 
 * @assigns rien
 *   
 * @return p.data[(i*p.width+j)*3+c];
 */
byte read_component_bw(picture p, int i, int j){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    return p.data[i*p.width+j];
}
/**
 * Lecture du pixel/triplet de la "case" i,j du champ "data" de p. Assignation aux pointeurs red,green,blue de type byte*.
 * @param [in] p la structure "picture" à lire.
 * @param [in] i ligne du pixel à lire (entier)
 * @param [in] j colonne du pixel à lire (entier)
 * @param [in] red pointeur stockant l'addresse d'une case mémoire stockant la valeur du canal "rouge" 
 * @param [in] green pointeur stockant l'addresse d'une case mémoire stockant la valeur du canal "vert" 
 * @param [in] blue pointeur stockant l'addresse d'une case mémoire stockant la valeur du canal "bleu" 
 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures *red,*green,*blue entre 0 et 255. ? (à vérifier).
 * 
 * @assigns red,green,blue (modification par effet de bord)
 *   
 * @return rien
 */
void read_pixel_rgb(picture p, int i, int j, byte *red, byte *green, byte *blue){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    assert(red!=NULL&&green!=NULL&&blue!=NULL);
    int index =  ((i*p.width)+j)*3;
    *red = p.data[index];
    *green = p.data[index+1];
    *blue = p.data[index+2];   
}
/**
 * Lecture du pixel/triplet de la "case" i,j du champ "data" de p. Assignation au pointeur value de type byte*.
 * @param [in] p la structure "picture" à lire.
 * @param [in] i ligne du pixel à lire (entier)
 * @param [in] j colonne du pixel à lire (entier)
 * @param [in] value pointeur stockant l'addresse d'une case mémoire stockant la valeur du pixel.

 * @ensures p structure valide
 * @ensures i,j indices valides (assert)
 * @ensures value entre 0 et 255.
 * 
 * @assigns value (modification par effet de bord)
 *   
 * @return rien
 */

void read_pixel_bw(picture p, int i, int j, byte *value){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    assert(value!=NULL);
    *value = p.data[(i*p.width)+j];
}