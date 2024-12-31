#ifndef PICTURES_H
#define PICTURES_H

#define MAX_BYTE 255
#include <stdbool.h>

int min_int(int a, int b);
double min_double(double d1, double d2);
double abs_double(double x);



typedef unsigned char byte; /* sizeof(char) == 1 */

double d_from_b(byte b);

enum channel_number{BW_PIXEL_SIZE = 1, RGB_PIXEL_SIZE = 3};

typedef struct{
    int width;
    int height;
    enum channel_number chan_num;
    byte *data;
}picture;

/*Création,nettoyage et copie*/
picture read_picture(const char *filename);
int write_picture(picture p, char *filename);

picture create_picture(unsigned int width, unsigned int height, unsigned int channels);
void clean_picture(picture *p);
picture copy_picture(picture p);

/*Information*/
bool is_empty_picture(picture p);
bool is_gray_picture(picture p);
bool is_color_picture(picture p);
bool same_dimensions(picture p1, picture p2);
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

/*Inversion*/
picture inverse_picture(picture p);

/*Normalisation*/
picture normalize_dynamic_picture(picture p);

/*Réduction du nombre de couleurs utilisées*/
picture set_levels_picture(picture p, byte nb_levels);

/*Différence absolue*/
picture distance_picture(picture p1, picture p2);
/*Produit*/
picture mult_picture(picture p1, picture p2);

/*Mélange*/
picture mix_picture(picture p1, picture p2, picture p3);

/*Helper functions*/

void check_resamplable(picture image, unsigned int width, unsigned int height,double *rx,double *ry);

/*Rééchantillonnage - politique du plus proche voisin*/
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height);

/*Rééchantillonnage - politique d'interpolation bilinéaire*/
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height);

#endif /*PICTURES_H*/