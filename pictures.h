#ifndef PICTURES_H
#define PICTURES_H

#define MAX_BYTE 255

#define BUFFER_SIZE (32)

#define EPSILON (1e-3)

#include <stdbool.h>

int min_int(int n1, int n2);
int max_int(int n1, int n2);
double min_double(double d1, double d2);
double abs_double(double x);



typedef unsigned char byte; /* sizeof(char) == 1 */

double d_from_b(byte b);

enum channel_number{GRAY_PIXEL_SIZE = 1, RGB_PIXEL_SIZE = 3};

typedef struct{
    int width;
    int height;
    enum channel_number chan_num;
    byte *data;
}picture;

/*Création,nettoyage et copie*/
//picture read_picture(const char *filename);
//int write_picture(picture p, char *filename);

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
void mix_reformat(picture p1,picture *q1);
picture mix_picture(picture p1, picture p2, picture p3);

/*Helper functions*/

void check_resamplable(picture image, unsigned int width, unsigned int height,double *rx,double *ry);

/*Rééchantillonnage - politique du plus proche voisin*/
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height);

/*Rééchantillonnage - politique d'interpolation bilinéaire*/
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height);

/*Brighten - LUT version*/
picture brighten_picture_lut(picture p, double factor);

/*Manipulation de noyaux*/

typedef struct kernel_s{
    unsigned int n;/*taille du noyau*/
    double **matrix;/*de dimensions n*n, pour stocker les coefficients*/
    double factor;
    double offset;
}kernel;

double **create_square_matrix(int n);
void delete_square_matrix(double **matrix, int n);
void set_square_matrix(double **matrix, int n, int i,int j, double coefficient);
double **copy_square_matrix(double **matrix, int n);
void print_square_matrix(double **matrix, int n);

void apply_matrix_affine_transformation(double **matrix, int n, double factor, double offset);

/*@requires p is greyscale picture*/
double get_convolved_value(double **matrix, int n, picture p, int i, int j);

/*On fait d'abord marcher la fonction pour des images en niveaux de gris.*/
picture apply_kernel_to_copy_bw(const picture p, const kernel k);

/*Puis on code une version générale avec split et merge.*/
picture apply_kernel_to_copy(const picture p, const kernel k);

double sum_coeff_square_matrix(double **matrix,int n);
void normalize_square_matrix(double **matrix, int n);
bool is_normalized(double **matrix, int n);



#endif /*PICTURES_H*/