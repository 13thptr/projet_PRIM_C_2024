#include <stdlib.h>
#include <math.h> /*Pour la gaussienne de gaussian_blur_wrapper*/

#include "wrappers.h"
#include "filename.h"
#include "picture_rw.h"

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
    for(int k=0;k<(int)p.chan_num;++k){
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
void brighten_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, const double BRIGHTENING_FACTOR){
    char brighten_op[9]="brighten";
    char *brighten_concat = concat_parts(res_dir,name_p,brighten_op,res_ext); 
    picture brightened = brighten_picture(p,BRIGHTENING_FACTOR);

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

/*LUT p*/
void inverse_picture_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, bool should_save, picture *save){
    /*1) Inversion*/
    picture inverted = inverse_picture(p);
    char inverted_op[9] = "inverse";
    char *inverted_path = concat_parts(res_dir,name_p,inverted_op,res_ext);

    write_picture(inverted,inverted_path);
    if(should_save){
        *save = inverted;
    }else{
        clean_picture(&inverted);
    }
    free(inverted_path);
}
/*2)Normalisation i.e. optimisation de la dynamique. */
void normalize_picture_wrapper(picture p, const char *res_dir, const char *name_p, char *res_ext){

    picture normalized = normalize_dynamic_picture(p);
    char dynamic_op[8] = "dynamic";
    char *normalized_path = concat_parts(res_dir,name_p,dynamic_op,res_ext);

    write_picture(normalized,normalized_path);
    clean_picture(&normalized);
    free(normalized_path);
}
/*normalize color picture using split & merge.
TODO: either 1) change above function name to normalize_gray_picture_wrapper 
OR           2) merge the two functions into a single one.
*/
//@requires is_color_picture p
void normalize_color_picture_wrapper(picture p, const char *res_dir, const char *name_p, char *res_ext){
    picture *split = split_picture(p);
    for(int k = 0; k<(int)p.chan_num;++k){
        picture tmp = normalize_dynamic_picture(split[k]);
        clean_picture(&split[k]);
        split[k] = tmp;
    }
    picture merged_result = merge_picture(split[0],split[1],split[2]);

    char dynamic_op[8] = "dynamic";
    char *normalized_path = concat_parts(res_dir,name_p,dynamic_op,res_ext);

    write_picture(merged_result,normalized_path);

    clean_picture(&split[0]);
    clean_picture(&split[1]);
    clean_picture(&split[2]);
    clean_picture(&merged_result);

    free(normalized_path);
    free(split);
}

/**/
void set_levels_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){

    picture levels = set_levels_picture(p,8);
    char levels_op[7] = "levels";
    char *levels_path = concat_parts(res_dir,name_p,levels_op,res_ext);

    write_picture(levels,levels_path);
    clean_picture(&levels);
    free(levels_path);
}

void resample_nearest_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, const double RESIZE_FACTOR){
    int new_width = (int)p.width*RESIZE_FACTOR;
    int new_height = (int)p.height*RESIZE_FACTOR;

    picture resampled = resample_picture_nearest(p,new_width,new_height);

    char smaller_op[30] = "smaller_nearest";
    char larger_op[30] = "larger_nearest";
    char *nearest_path = concat_parts(res_dir,name_p,RESIZE_FACTOR>1?larger_op:smaller_op,res_ext);

    write_picture(resampled,nearest_path);

    clean_picture(&resampled);
    free(nearest_path);
}

void resample_bilinear_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, const double RESIZE_FACTOR){
    int new_width = (int)p.width*RESIZE_FACTOR;
    int new_height = (int)p.height*RESIZE_FACTOR;

    picture resampled = resample_picture_bilinear(p,new_width,new_height);

    char smaller_op[30] = "smaller_bilinear";
    char larger_op[30] = "larger_bilinear";
    char *bilinear_path = concat_parts(res_dir,name_p,RESIZE_FACTOR>1?larger_op:smaller_op,res_ext);

    write_picture(resampled,bilinear_path);

    clean_picture(&resampled);
    free(bilinear_path);
}

void normalized_difference_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, const double RESIZE_FACTOR){
  
   
    int new_width = (int)p.width * RESIZE_FACTOR;
    int new_height = (int)p.height * RESIZE_FACTOR;

    picture p1 =  resample_picture_nearest(p,new_width,new_height);
    picture p2 = resample_picture_bilinear(p,new_width,new_height);

    picture difference = distance_picture(p1,p2);

    clean_picture(&p1);
    clean_picture(&p2);

    picture normalized_res = normalize_dynamic_picture(difference);
    clean_picture(&difference);
    
    

    char diff_op[30] = "difference";
    char *difference_path = concat_parts(res_dir,name_p,diff_op,res_ext);

    write_picture(normalized_res,difference_path);

    clean_picture(&normalized_res);
    free(difference_path);
}

/*Produit de deux images:*/
void mult_picture_wrapper(picture p1, picture p2, char *res_dir, char *name_p, char *res_ext){

    picture product = mult_picture(p1,p2);
    
    char product_op[30] = "product";
    char *product_path = concat_parts(res_dir,name_p,product_op,res_ext);

    write_picture(product,product_path);

    clean_picture(&product);
    free(product_path);
}

/*Mixture de deux images (barycentres) selon une troisième*/
void mix_picture_wrapper(picture p1,picture p2, picture p3, char *res_dir, char *name_p, char *res_ext){
    picture mixture = mix_picture(p1,p2,p3);
    char mixture_op[30] = "mixture";
    char *mixture_path = concat_parts(res_dir,name_p,mixture_op,res_ext);
    write_picture(mixture,mixture_path);
    clean_picture(&mixture);
    free(mixture_path);
}

/*----------------------------------------------------BONUS------------------------------------------------------*/
void brighten_lut_wrapper(picture p, char *res_dir, char *name_p, char *res_ext, const double BRIGHTENING_FACTOR){
    picture brightened_lut = brighten_picture_lut(p,BRIGHTENING_FACTOR);
    char brighten_lut_op[30] = "brighten_lut";
    char *brighten_lut_path = concat_parts(res_dir,name_p,brighten_lut_op,res_ext);
    write_picture(brightened_lut,brighten_lut_path);
    clean_picture(&brightened_lut);
    free(brighten_lut_path);
}
/*Fonction générale pour tester les noyaux dans le main si besoin*/
void kernel_wrapper(picture p, kernel k,char *res_dir, char *name_p, char *res_ext){
    picture kernelized = apply_kernel_to_copy(p,k);

    char kernelized_op[30] = "kernelized";
    char *kernelized_path = concat_parts(res_dir,name_p,kernelized_op,res_ext);
    write_picture(kernelized,kernelized_path);
    clean_picture(&kernelized);
    free(kernelized_path);
}



void gaussian_blur_wrapper(picture p,int matrix_size, double std_dev,char *res_dir, char *name_p, char *res_ext){
    
    const double GAUSSIAN_HALVER = 2.0; /*Ne pas toucher*/

    /*----------------On crée d'abord le noyau.--------------------*/
    kernel k;
    k.n = matrix_size;
    k.matrix = create_square_matrix(k.n);
    k.offset = 0.0;
    int u0 = k.n/2;
    int v0 = u0;
    double sum = 0; /*Somme des coefficients*/
    for(int u=0;u<(int)k.n;++u){
        for(int v=0;v<(int)k.n;++v){
            double sq_u = (u-u0)*(u-u0);
            double sq_v = (v-v0)*(v-v0);
            double sq_sigma = std_dev*std_dev;
            /*Plus ou fois ? sq_u+sq_v est choisi conformément à la formule de l'article Wikipédia, je suppose que
            le document contenait une typo.*/
            double value = exp(-(sq_u+sq_v)/GAUSSIAN_HALVER/sq_sigma);
            sum += value;
            set_square_matrix(k.matrix,k.n,u,v,value);
        }
    }
    //printf("Gaussian kernel:initialized:\n");
    //print_square_matrix(k.matrix,k.n);
    k.factor = 1.0/sum;
    /*On appelle maintenant kernel_wrapper */
    //kernel_wrapper(p,k,res_dir,name_p,res_ext);

    picture blurred = apply_kernel_to_copy(p,k);
    char blurred_op[30] = "blurred";
    char *blurred_path = concat_parts(res_dir,name_p,blurred_op,res_ext);
    write_picture(blurred,blurred_path);


    clean_picture(&blurred);
    free(blurred_path);

    delete_square_matrix(k.matrix,k.n);
}
/*Une recherche internet sur la dérivée d'une image m'a permis de découvrir la notion de filtre de sobel.*/
void sobel_filter_wrapper(picture p, char *res_dir, char *name_p, char *res_ext){

    const double THRESHOLD = 195.0; /*Valeur empirique*/
    /*----------------On crée d'abord le noyau pour le filtrage horizontal--------------------*/
    kernel k;
    k.n = 3;
    k.matrix = create_square_matrix(k.n);
    k.offset = 255/2;
    k.factor = 0.25;

    k.matrix[0][0]=-1.0; k.matrix[0][1] = 0.0; k.matrix[0][2] = 1.0;
    k.matrix[1][0]=-2.0; k.matrix[1][1] = 0.0; k.matrix[1][2] = 2.0;
    k.matrix[2][0]= -1.0; k.matrix[2][1] = 0.0; k.matrix[2][2] = 1.0;
    
    //print_square_matrix(k.matrix,k.n);

    picture horizontal = apply_kernel_to_copy(p,k);

    char left_derivative_op[30] = "left_derivative";

    char *horiz_path = concat_parts(res_dir,name_p,left_derivative_op,res_ext);
    write_picture(horizontal,horiz_path);


    /*----------------Ensuite, filtrage vertical--------------------*/
    /*On aurait aussi pu réutiliser la fonction transposée écrite dans un TP précédent*/
    k.matrix[0][0]=-1.0; k.matrix[0][1] = -2.0; k.matrix[0][2] = -1.0;
    k.matrix[1][0]= 0.0; k.matrix[1][1] = 0.0; k.matrix[1][2] = 0.0;
    k.matrix[2][0]= 1.0; k.matrix[2][1] = 2.0; k.matrix[2][2] = 1.0;


    picture vertical = apply_kernel_to_copy(p,k);

    picture sobel = copy_picture(p);

    for(int s=0;s<(int)p.chan_num*p.width*p.height;++s){
        double value = min_double(255.0,sqrt(pow((double)horizontal.data[s],2.0)+pow((double)vertical.data[s],2.0)));
        if(value>THRESHOLD){
            sobel.data[s] = (byte)value;
        }
        else{
            sobel.data[s] = 0;
        }
    }

    /*horizontal & vertical not needed anymore.*/
    clean_picture(&horizontal);
    clean_picture(&vertical);

    char sobel_op[30] = "sobel";
    char *sobel_path = concat_parts(res_dir,name_p,sobel_op,res_ext);
    write_picture(sobel,sobel_path);

    clean_picture(&sobel);
    free(horiz_path);
    free(sobel_path);


    delete_square_matrix(k.matrix,k.n);
}