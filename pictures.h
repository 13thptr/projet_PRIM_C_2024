#ifndef PICTURES_H
#define PICTURES_H

#define MAX_BYTE 255

#define BUFFER_SIZE (32)

#define EPSILON (1e-3)

#include <stdbool.h>
/**
 * Minimum entre deux entiers.
 * @param [in] n1 int
 * @param [in] n2 int 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: min(n1,n2)
 **/

int min_int(int n1, int n2);
/**
 * Maximum entre deux entiers.
 * @param [in] n1 int
 * @param [in] n2 int 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: max(n1,n2)
 **/
int max_int(int n1, int n2);
/**
 * Minimum entre deux doubles.
 * @param [in] d1 double 
 * @param [in] d2 double 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: min(d1,d2)
 **/
double min_double(double d1, double d2);
/**
 * Valeur absolue d'un double
 * @param [in] x double
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: |x|
 **/
double abs_double(double x);



typedef unsigned char byte; /* sizeof(char) == 1 */
/* d_from_b
Fonction utilitaire pour convertir (caster) en évitant un warning "pédant" (pedantic) sans créer trop de variables.
    @param: b
    @requires: rien
    @assigns: rien
    @ensures: rien
    @return: cast de b en double.
*/
double d_from_b(byte b);

enum channel_number{GRAY_PIXEL_SIZE = 1, RGB_PIXEL_SIZE = 3};

typedef struct{
    int width;
    int height;
    enum channel_number chan_num;
    byte *data;
}picture;

/*Création,nettoyage et copie- cf. picture_rw.[h|c] pour la manipulation des fichiers*/

picture create_picture(unsigned int width, unsigned int height, unsigned int channels);
void clean_picture(picture *p);
picture copy_picture(picture p);

/*Information*/
/**
 * is_empty_picture
 * @param [in] p 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return booléen: vrai si aucune des valeurs ne vaut 0, faux sinon.
**/
bool is_empty_picture(picture p);
/**
 * is_gray_picture
 * @param [in] p 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return booléen: vrai si l'image est en niveaux de gris, faux sinon
**/
bool is_gray_picture(picture p);
/**
 * is_color_picture
 * @param [in] p 
 * @assigns: rien
 * @ensures: cf. return
 * @return booléen: vrai si l'image est en couleurs, faux sinon
**/
bool is_color_picture(picture p);
/**
 * info_picture
 * @param [in] p 
 * @assigns: rien
 * @ensures: rien
 * @return rien
**/
void info_picture(picture p);
/**
 * same_dimensions
 * @param [in] p1
 * @param [in] p2 
 * @assigns: rien
 * @ensures: rien
 * @return vrai ssi les deux images ont les mêmes dimensions
**/
bool same_dimensions(picture p1, picture p2);
/*------------------------------------------------------------Conversion(s)-----------------------------------------------------------*/
/**
 * convert_to_color_picture
 * @param [in] p 
 * @requires: p image valide, non vide.
 * @assigns: modifie le tas en créant ou bien une copie de l'image en cours, ou bien une nouvelle image (nouvelle allocation 
 * dans les deux cas)
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal. 
 * @return image en couleur, copie de l'image d'entrée ou résultat de la duplication des canaux.
**/
picture convert_to_color_picture(picture p);
/**
 * convert_to_gray_picture
 * @param [in] p 
 * @requires: p image valide, non vide.
 * @assigns: modifie le tas en créant ou bien une copie de l'image en cours, ou bien une nouvelle image (nouvelle allocation 
 * dans les deux cas)
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal.
 * @return image en niveaux de gris, copie de l'image d'entrée ou résultat de la moyenne pondérée des 3 canaux pixel par pixel.
**/
picture convert_to_gray_picture(picture p);

/*-------------------------------------------------------------Séparation--------------------------------------------------------------*/
/**
 * split_picture
 * @param [in] p 
 * @requires: p image valide;
 * @assigns: modifie le tas en créant:
 *  -1) Un pointeur de type picture* vers la première case d'un tableau de "picture"s de taille 1 ou 3 (selon 
 *  la valeur de p.chan_num).
 *  -2) Des tableaux "data" pour les composantes rouge, verte et bleue sous forme d'images en niveaux de gris.
 * dans les deux cas)
 * 
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal.
 * 
 * @return pointeur de type picture* 
**/
picture * split_picture(picture p);

/*---------------------------------------------------------------Mélange---------------------------------------------------------------*/
/**
 * merge_picture
 * @param [in] red, green, blue
 * @requires: red,green,blue images 
 * @assigns: modifie le tas en créant:
 *  une picture "res" et donc un pointeur vers un tableau alloué dans le tas via create_picture pour res.data;
 * @ensures: Plantage si les trois images n'ont pas la même dimension, via width=0 ou height=0 et create_picture
 *  Renvoie la fusion des 3 images utilisées comme canaux sinon.
 * @return image nommée res(type picture)
**/
picture merge_picture(picture red, picture green, picture blue);

/*------------------------------------------------------------Eclaircissement----------------------------------------------------------*/
/**
 * brighten_picture
 * @param [in] p, factor
 * @requires: p image valide, factor>0.
 * @assigns: modifie le tas en créant:
 *  une copie via copy_picture, donc un nouvau champ data contenant un pointeur vers un tableau de bytes.
 * @ensures: Plantage si les préconditions de copy_picture ne sont pas respectées
 * @return image nommée res(type picture)
**/
picture brighten_picture(picture p, double factor);

/*-----------------------------------------------------------------Fonte---------------------------------------------------------------*/
/**
 * melt_picture
 * @param [in] p, number
 * @requires: p: image valide, number: nombre positifs de pixels à faire fondre.
 * @assigns: modifie le tas en créant une copie que l'on va modifier pour l'effet de fonte.
 * @ensures: Plantage si les préconditions de copy_picture ne sont pas respectées
 * @return image nommée melted (type picture)
**/
picture melt_picture(picture p, int number);

/*--------------------------------------------------------------Inversion--------------------------------------------------------------*/
/**
 * inverse_picture
 * @param [in] p 
 * @requires: p image valide
 * @assigns: nouvelle copie de l'image p dans le tas (heap), modifiée par une lut via apply_lut.
 * @ensures: Plantage si les préconditions de copy_picture, create_lut, apply_lut ne sont pas respectées
 * @return image de type picture contenant un champ data avec les données d'image de p.data "inversées" (involution)
**/
picture inverse_picture(picture p);

/*-------------------------------------------------------------Normalisation-----------------------------------------------------------*/
/**
 * normalize_dynamic_picture
 * @param [in] p 
 * @requires: p image valide
 * @assigns: copie en mémoire de p effectuée avant modificaiton par lut. lut stockée temporairement dans le tas le temps 
 * d'effectuer le traitement.
 * @ensures: plantage si les préconditions de copy_picture ne sont pas respectées, sinon res est renvoyée et contient les données
 * normalisées.
 * @return res image normalisée de type picture.
**/
picture normalize_dynamic_picture(picture p);

/*---------------------------------------------------------------Réduction-------------------------------------------------------------*/
/**
 * set_levels_picture
 * @param [in] p, nb_levels
 * @requires: p picture valide
 * @assigns: modification du tas par copy_picture créant une nouvelle image qui sera renvoyée (res)
 * @ensures: plantage par copy_picture, create_lut ou apply_lut via assert en cas de non-respect d'une précondition, sinon cf. @return
 * @return res image ne contenant que nb_levels niveaux de couleur
**/
picture set_levels_picture(picture p, byte nb_levels);

/*-----------------------------------------------------------Différence absolue--------------------------------------------------------*/
/**
 * distance_picture (non nommée dans l'énoncé:mentionner dans le rapport)
 * @param [in] p1, @param[in] p2
 * @requires:p1,p2 images valides de type picture, de même tailles
 * @assigns: allocation d'une zone mémoire pour le champ data de la picture "res" crée dans le corps de la fonction
 * @ensures: plantage si p1,p2 ne vérifient pas les préconditions, cf. return sinon. p1 et p2 ne sont pas modifiées...
 * @return: image constituée de la distance "byte à byte" 
**/
picture distance_picture(picture p1, picture p2);
/*-----------------------------------------------------------------Produit-------------------------------------------------------------*/
/**
 * mult_picture
 * @param [in] p1
 * @param [in] p2
 * @requires:p1,p2 images valides
 * @assigns: le tas est modifié par la création d'une picture res.
 * @ensures: res contient la multiplication "renormalisée" de p1 et p2 byte à byte
 * "renormalisée" dans le sens où l'on divise le produit des byte convertis en doubles par 255 pour obtenir une résultat
 * qui soit encore entre 0 et 255. On n'utilise pas renormalize_dynamic_picture car le but ici n'est pas d'optimiser la 
 * dynamique.
 * @return picture res contenant le "produit" décrit ci-dessus.
**/

picture mult_picture(picture p1, picture p2);

/*-----------------------------------------------------------------Mélange-------------------------------------------------------------*/
/**
 * Fonction utilitaire mix_reformat
 * Intérêt: Permet de n'avoir à traiter que le cas (RGB,RGB,RGB) pour les arguments (p1,p2,p3) de mix_picture.
 * @param [in] p1 image à "transformer" éventuellement en image couleur
 * @param [in] q1 pointeur vers une image censée accueillir une copie ou une conversion en couleur de p1
 * @requires:p1,p2,p3 images valides de type pictures.
 * @assigns: modification du tas via convert_to_color picture ou copy_picture qui effectuent une allocation
 * @ensures: *q1 contient une copie de p1 si celle-ci était en couleur, une conversion en couleur sinon.
 * @return: rien (void)
**/
void mix_reformat(picture p1,picture *q1);
/** 
 * mix_picture
 * @param [in] p1,p2,p3
 * @requires:p1,p2,p3 images valides de type pictures.
 * @assigns: modifications dans le tas
 *          1) "Temporaires" (propres au corps de fonction):
 *              - q1,q2,q3 sont modifiées par mix_reformat
 *          2) "Permanentes" (on passe la responsabilité de libérer la mémoire à une autre fonction, dans un wrapper de wrappers.c
 *              ou à la fonction main dans main.c):
 *              - l'image res allouée est renvoyée.
 * @ensures: res contient l'image "mélange" selon une politique de barycentre
 *           des deux premières images d'entrée, pondéré par  la troisième image.
 *           ou alors la fonction a planté car l'une des trois images d'entrée n'a pas vérifié une précondition.
 * 
 * @return cf. @ensures:
 **/
picture mix_picture(picture p1, picture p2, picture p3);

/*--------------------------------------------Fonction d'aide : vérication "rééchantillonable" ?---------------------------------------*/
/*
    Fonction utilitaire: utilisée deux fois
    Modifie par référence les ratios de redimensionnement horizontal et vertical
    Affiche un avertissement lorsque l'on déforme l'image.

 * check_resamplable
 * @param: image, width, height, rx, ry
 * @requires: image structure valide de type picture.
 * @requires: width not 0
 * @requires: height not 0
 * @requires: rx,ry valid pointers
**/

void check_resamplable(picture image, unsigned int width, unsigned int height,double *rx,double *ry);

/*-----------------------------------------------------------Rééchantillonnage---------------------------------------------------------*/
/*Politique du plus proche voisin*/
/**
 * resample_picture_nearest
 * @param [in] image
 * @param [in] width
 * @param [in] height
 * @requires: image, width, heigth vérifient les conditions de check_resamplable
 * @assigns: le tas est modifié par la création de l'image renvoyée "res", à libérer dans une autre fonction.
 * @ensures: sous condition de redimensionnabilité, "res" contient "image" redimensionnée avec la politique du plus proche voisin.
 * @return: res l'image d'entrée, mais redimensionnée.
**/
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height);

/*Politique d'interpolation bilinéaire*/
/**
 * resample_picture_bilinear
 * @param [in] image 
 * @param [in] width
 * @param [in] height
 * @requires: image, width, heigth vérifient les conditions de check_resamplable
 * @assigns: le tas est modifié par la création de l'image renvoyée "res", à libérer dans une autre fonction.
 *   
 * @ensures: sous condition de redimensionnabilité, "res" contient "image" redimensionnée
 *  avec la politique d'interpolation bilinéaire.
 * 
 * @return: res l'image d'entrée, mais redimensionnée
**/
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height);
/*--------------------------------------- B  O  N  U  S------------------------------------------------------------------*/

/*  
    Note: je ne me suis pas senti obligé d'écrire les @requires @assigns @ensures pour le bonus, car c'est assez chronophage
    d'autant quand on ne le fait pas au fur et à mesure (mauvaise idée, je sais): il faut se remémorer le contexte d'utilisation
    de la fonction ainsi que son fonctionnement.
*/
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