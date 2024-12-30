
#include "pictures.h"
//conv2col_wrapper (better code readability)
//@ requires p is a grayscale (pgm) picture ? No -> Just copy if it's already a color picture
void convert_color_wrapper(picture p, char *dir_p,char *name_p, char *ext_p);

/*RGB->gray*/
/*@requires picture is valid ppm file*/
void convert_gray_wrapper(picture p, char *dir_p, char *name_p, char *ext_p);

