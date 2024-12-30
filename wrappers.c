#include <stdlib.h>

#include "wrappers.h"
#include "filename.h"

void convert_color_wrapper(picture p, char *dir_p,char *name_p, char *ext_p){
    picture conv2col = convert_to_color_picture(p);
    char conv2col_op[30] = "convert_color"; //Check wanted filename OK
    char *conv2col_concat = concat_parts(dir_p,name_p,conv2col_op,ext_p);
    write_picture(conv2col,conv2col_concat);
    clean_picture(&conv2col);
    free(conv2col_concat);
}
void convert_gray_wrapper(picture p, char *dir_p, char *name_p, char *ext_p){ 
    picture conv2gray = convert_to_gray_picture(p);
    char conv2gray_op[30]= "convert_gray";
    char *conv2gray_concat = concat_parts(dir_p,name_p,conv2gray_op,ext_p);
    write_picture(conv2gray,conv2gray_concat);
    clean_picture(&conv2gray);
    free(conv2gray_concat);
}