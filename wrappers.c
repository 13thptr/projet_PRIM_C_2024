#include <stdlib.h>
//#include <string.h>

#include "wrappers.h"
#include "filename.h"

/*TODO: merge this file with safe_malloc.c ?*/

void convert_color_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){
    picture conv2col = convert_to_color_picture(p);
    char conv2col_op[30] = "convert_color"; //Check wanted filename OK
    char *conv2col_concat = concat_parts(res_dir,name_p,conv2col_op,res_ext);
    write_picture(conv2col,conv2col_concat);
    clean_picture(&conv2col);
    free(conv2col_concat);
}
void convert_gray_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){ 
    picture conv2gray = convert_to_gray_picture(p);
    char conv2gray_op[30]= "convert_gray";
    char *conv2gray_concat = concat_parts(res_dir,name_p,conv2gray_op,res_ext);
    write_picture(conv2gray,conv2gray_concat);
    clean_picture(&conv2gray);
    free(conv2gray_concat);
}
/*Split*/
void split_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){
    picture *split_array = split_picture(p);
    /*Ce qui suit a requis la modification de filename.[h|c], plus précisément l'ajout du mot-clef const.*/
    const char *rgb_filename[3];
    rgb_filename[0] = "red";
    rgb_filename[1] = "green";
    rgb_filename[2] = "blue";
    for(int k=0;k<(int)p.chan_num;k++){
        /*Faire dépendre la boucle de chan_num permet de gérer les cas d'image ppm ou pgm sans disjonction explicite.*/
        char *split_channel_concat = concat_parts(res_dir,name_p,rgb_filename[k],res_ext);
        write_picture(split_array[k],split_channel_concat);
        /*On libère le chemin créé pour la composante*/
        free(split_channel_concat);
        /*On nettoie l'image (libère la mémoire occupée par la k-ème composante)*/
        clean_picture(&split_array[k]);
    }
    /*On libère le tableau d'images.*/
    free(split_array);
}
/*Brighten (doit marcher sur RGB & BW alike)*/
void brighten_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){
    char brighten_op[9]="brighten";
    char *brighten_concat = concat_parts(res_dir,name_p,brighten_op,res_ext); 
    picture brightened = brighten_picture(p,1.5);

    write_picture(brightened,brighten_concat);
    clean_picture(&brightened);
    free(brighten_concat);
}

void melt_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){
    
    picture melted = melt_picture(p,p.width*p.height*p.chan_num*5);
    char melted_op[7]="melted";
    char *melted_path = concat_parts(res_dir,name_p,melted_op,res_ext);

    write_picture(melted,melted_path);
    clean_picture(&melted);
    free(melted_path);
}