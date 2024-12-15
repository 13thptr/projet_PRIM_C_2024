#ifndef PICTURES_H
#define PICTURES_H

/*Vous pourrez donc dans ce module déclarer :
    Un type byte par exemple pour contenir les valeurs codées par un octet.
     On pourra aussi définir une constante MAX_BYTE 255
      pour contenir la valeur max des octets.*/
#define MAX_BYTE 255
#include <stdbool.h>

typedef unsigned char byte; /* sizeof(char) == 1 */

enum channel_number{BW_PIXEL_SIZE = 1, RGB_PIXEL_SIZE = 3};
/*
    Une structure picture similaire à celle utilisée dans le TP 6 qui contiendra
    - Une hauteur, une largeur
    - Le nombre de canaux de l’image
        1 canal pour les images en niveau de gris.
        3 canaux pour les images en couleur correspondant aux valeurs pour le rouge, le vert et le bleu.
    - Un pointeur vers les données pixels de l’image utilisant un tableau unidimensionnel d’octets.
*/
typedef struct{
    int width;
    int height;
    enum channel_number chan_num;
    byte *data;
}picture;

/*Création,nettoyage et copie*/
picture read_picture(char * filename);
int write_picture(picture p, char * filename);
picture create_picture(unsigned int width, unsigned int height, unsigned int channels, byte max);
void clean_picture(picture *p);
picture copy_picture(picture p);

/*Information*/
bool is_empty_picture(picture p);
bool is_gray_picture(picture p);
bool is_color_picture(picture p);
void info_picture(picture p);

/*Conversion*/
picture convert_to_color_picture(picture p);
picture convert_to_gray_picture(picture p);

/*Séparation*/
picture * split_picture(picture p);

/*Mélange*/
picture merge_picture(picture red, picture green, picture blue);

/*Eclaircissement*/
picture brighten_picture(picture p, double factor);

/*Fonte*/
picture melt_picture(picture p, int number);

#endif /*PICTURES_H*/