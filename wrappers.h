
#include "pictures.h"
//conv2col_wrapper (better code readability)
//@ requires p is a grayscale (pgm) picture ? No -> Just copy if it's already a color picture
void convert_color_wrapper(picture p, char *dir_p,char *name_p, char *ext_p);

/*RGB->gray*/
/*@requires picture is valid ppm file*/
void convert_gray_wrapper(picture p, char *dir_p, char *name_p, char *ext_p);

/*RGB->red component,green component, blue component*/
void split_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext);

/*Brighten*/
void brighten_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext);

/*Melt*/
void melt_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext);

/*Invert*/
void inverse_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext);
/*Normalize*/
void normalize_picture_wrapper(picture p, const char *res_dir, const char *name_p, char *res_ext);
/*Set_levels*/
void set_levels_wrapper(picture p, char *res_dir, char *name_p, char *res_ext);