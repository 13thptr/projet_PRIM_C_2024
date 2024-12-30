
/*TODO: examiner à la main l'arbre des includes et enlever ceux qui ne sont pas nécessaires.*/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include "pictures.h"
#include "filename.h"

#include "wrappers.h"

int main(int argc, char* argv[]){
 
    /*-------------------------Déclaration des variables en amont (conforme à la norme ISO C)-------------------------------*/

    picture current_pic; /*Variable qui sera mise à jour au fur et à mesure dans une boucle sur les arguments.*/

    char output_dir[13] = "Lenna_output"; /*Chemin pour produire les images afin de ne pas mélanger les entrées et les sorties*/

  

    char ppm_ext[4] = "ppm";
    char pgm_ext[4] = "pgm";
    
    /*Vérifications d'usage sur la bonne utilisation du programme. Proposer plus loin une aide pour les différentes options.*/
    if(argc<2){
        fprintf(stderr,"Usage:./prog <f1.[pgm|ppm]>...<fn.[pgm|ppm]>\n");
        return EXIT_FAILURE;
    }

    const int NB_FILES = argc-1;
    char *dir;
    char *name; 
    char *ext;

    /*----------------------------------------------Boucle principale-------------------------------------------------------------------*/

    for(int i = 1;i <= NB_FILES;++i){
        dir = dir_from_path(argv[i]);
        name = name_from_path(argv[i]);
        ext = ext_from_path(argv[i]);

        /*Read the i-th picture.*/
        current_pic = read_picture(argv[i]);

        /* Get and print pic info */
        info_picture(current_pic);

        /*Certains traitements ne doivent être faits que pour des images en niveaux de gris, d'autres pour des images en couleur.*/
        if(is_gray_picture(current_pic)){
            convert_color_wrapper(current_pic,output_dir,name,ppm_ext);
        }
        if(is_color_picture(current_pic)){
            convert_gray_wrapper(current_pic,output_dir,name,pgm_ext);
            split_picture_wrapper(current_pic,output_dir,name,pgm_ext);
            normalize_color_picture_wrapper(current_pic,output_dir,name,ext);
        }

        /*Opérations ne dépendant pas du type d'image: brighten, melt,inverse, set_levels...*/
        brighten_picture_wrapper(current_pic,output_dir,name,ext);
        melt_picture_wrapper(current_pic,output_dir,name,ext);
        inverse_picture_wrapper(current_pic,output_dir,name,ext);
        set_levels_wrapper(current_pic,output_dir,name,ext);
        
        /*Free and reset memory*/
        clean_picture(&current_pic);//Check the prototype clean_picture should have.
        free(dir);
        free(name);
        free(ext);
        dir = NULL;
        name = NULL;
        ext = NULL;
    }
    /*On traite à part la normalisation du fichier Lenna_gray pour éviter que Lenna_BW soit traité.*/
    current_pic = read_picture("Lenna_input/Lenna_gray.pgm");
    normalize_picture_wrapper(current_pic,"Lenna_output","Lenna_gray",pgm_ext);
    clean_picture(&current_pic);
    
    return EXIT_SUCCESS;
}