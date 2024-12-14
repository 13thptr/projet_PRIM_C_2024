#ifndef PICTURES_H
#define PICTURES_H

/*Vous pourrez donc dans ce module déclarer :
    Un type byte par exemple pour contenir les valeurs codées par un octet.
     On pourra aussi définir une constante MAX_BYTE 255
      pour contenir la valeur max des octets.*/
#define MAX_BYTE 255

#define BW_PIXEL_SIZE 1
#define RGB_PIXEL_SIZE 3 
typedef unsigned char byte; /* sizeof(char) == 1 */

enum channel_number{GREY_PICTURE = 1, RGB_PICTURE = 3};
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

picture read_picture(char * filename);

int write_picture(picture p, char * filename);





#endif /*PICTURES_H*/