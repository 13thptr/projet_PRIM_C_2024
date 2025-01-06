#ifndef _PICTURE_RW_
#define _PICTURE_RW_
#include "pictures.h"
#include <stdio.h> //Pour "FILE"
/*
    Fonction fgets_encapsulator
    @param: buf est un pointeur vers un tableau de caractères 
    @param f fichier à lire
    @param line_counter compteur de lignes (ne sert qu'à affichier, n'est pas modifié)

    @requires f fichier valide.
    
    @assigns: cf. comportement de fgets.
    @ensures: le booléen renvoyé correspond au statut de lecture.
*/
bool fgets_encapsulator(char *buf, FILE *f, int line_counter);

/*
    Fonction read_correctly
    @param: pointeur vers un booléen indiquant si le "bloc" (commentaires) a été lu correctement ou non
    @param buffer tableau où stocker les données lues
    @param to_be_read fichier à lire
    @param res pointeur vers une structure de type picture

    @requires: "*p" structure de type "picture" valide (avec champs width, height, chan_num et data valides).
    @requires: "
    @assigns: read_correctly contient true ssi la lecture s'est bien effectuée.
    @assigns: buffer contient un début de ligne de longueur BUFFER_SIZE via l'appel à fgets_encapsulator
    @ensures: renvoie un booléen (en fait 0 ou 1) indiquant si le bloc éventuellement commenté a pu être lu ou pas 
    @ensures: en cas d'erreur, on réinitialise l'image vers laquelle res pointe en une image vide, que l'on renverra
    dans read_picture.
    @return: cf.@ ensures
*/
bool read_correctly_block(bool *read_correctly, char *buffer, FILE *to_be_read, picture *res, int *lc_p);
/*
    Fonction reset_picture_to_zero
    @requires p pointeur bien initialisé
    @assigns *p
    @ensures *p est une image vide à l'issue de l'appel reset_picture_to_zero(p);
*/
void reset_picture_to_zero(picture *p);
/*
    Fonction read_picture
    @param: filename chemin vers un fichier
    @requires: filename chemin valide vers un fichier .pgm ou .ppm valide
    @assigns: rien
    @return: une structure de type "picture"

    TODO: replace strcmp with strncmp ? + Increase general safety 
*/

picture read_picture(const char *filename);

/*
    Fonction write_picture
    @param: filename chemin vers un fichier
    @requires: "p" structure de type "picture" valide (avec champs width, height, chan_num et data valides).
    @requires: "filename" chemin (potentiel) valide.
    @assigns: rien
    @ensures: ce qu'il faut
    @return un entier indiquant le statut de succès(0) ou d'échec de l'appel à la fonction.
*/

int write_picture(picture p, char * filename);

#endif /*_PICTURE_RW */