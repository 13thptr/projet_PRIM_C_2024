#include <assert.h>
#include <stdlib.h>
#include "pixels.h"

/**
 * Ecriture de la valeur "value" dans le canal "c" du pixel d'indice i,j du champ "data" de p.
 * @param [in] p la structure "picture" à modifier.
 * @param [in] i ligne du pixel à modifier
 * @param [in] j colonne du pixel à modifier
 * @param [in] value valeur du pixel à modifier
 * 
 * @requires: p n'est pas vide.
 * @requires: c canal valide (RED=0 ou GREEN=1 ou BLUE = 2)
 * @requires: i,j indices valides (entre 0 et hauteur, resp. largeur moins 1)
 * @assigns: p.data
 * 
 * @ensures: p.data est modifié et contient le pixel de la bonne couleur. 
 * @return rien
 * 
 */
void write_component_rgb(picture p, int i, int j, enum PIXEL_TYPES c,int value){
    assert(!is_empty_picture(p));
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
 * @requires: p structure valide (image non vide)
 * @requires: i,j indices valides (assert)
 * @requires:
 * 
 * @assigns: p.data
 * @ensures: p.data est modifié et contient un pixel des bonnes couleurs au bon endroit. 
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
 * @requires: p structure valide
 * @requires: i,j indices valides (assert)
 * @requires: value entre 0 et 255.
 * 
 * @assigns: p.data
 * 
 * @ensures: plantage en cas de non respect des préconditions, sinon le pixel écrit est le bon.
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

 * @requires: p structure valide
 * @requires: i,j indices valides (assert)
 * @requires: c entre 0 et 2.
 * 
 * @assigns: rien
 *   
 * @ensure: plantage en cas de non-respect des préconditions,sinon la valeur de la composante renvoyée est la bonne.
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

 * @requires: p structure valide
 * @requires: i,j indices valides (assert)
 * 
 * @assigns: rien
 *   
 * @ensures: plantage en cas de non-respect des préconditions, le bon résultat sinon.
 * 
 * @return p.data[i*p.width+j];
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
 * 
 * @requires: p structure valide
 * @requires: i,j indices valides (assert)
 * @requires: *red,*green,*blue entre 0 et 255. ? (à vérifier).
 * 
 * @assigns: red,green,blue (modification par effet de bord)
 * @ensures: plantage si les préconditions ne sont pas respectées, le bon résultat sinon.
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

 * @requires: p structure valide
 * @requires: i,j indices valides (assert)
 * @requires: value entre 0 et 255.
 * 
 * @assigns: value (modification par effet de bord)
 *  
 * @ensure: plantage si non-respect des préconditions, sinon *value contient la valeur du pixel (i,j).
 * @return rien
 */

void read_pixel_bw(picture p, int i, int j, byte *value){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    assert(value!=NULL);
    *value = p.data[(i*p.width)+j];
}