
/*TODO: examiner à la main l'arbre des includes et enlever ceux qui ne sont pas nécessaires.*/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include "filename.h"

#include "picture_rw.h"

#include "wrappers.h"
#include "safe_malloc.h"


#define LARGER_FACTOR (1.36)
#define BRIGHTENING_FACTOR (1.5)

/*
* @param [in]: argc
* @param [in]: argv
* @requires: argc > 1
* @requires: argv contient des noms de fichiers ou d'options valide
* @assigns modifications dans le tas (cf. wrappers.c pour les allocations et libérations)
* @return: statut (EXIT_SUCCESS, ou EXIT_FAILURE)
*/
int main(int argc, char* argv[]){
    /*-------------------------Déclaration des variables en amont (conforme à la norme ISO C)-------------------------------*/

    picture current_pic; /*Variable qui sera mise à jour au fur et à mesure dans une boucle sur les arguments.*/
    picture mask;  /*Le 3ème argument éventuel*/
    picture save; /*Sauvegarder l'image inversée pour la réutiliser comme l'énoncé semble le demander.*/
 
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

    bool THIRD_IMAGE_FLAG = false;
    /*Chargement de la troisième image (optionnel)*/

    if(argc == 4){
        printf("Reading mask...\n");
        mask = read_picture(argv[3]);
        THIRD_IMAGE_FLAG = true;
    }

    /*----------------------------------------------Boucle principale-------------------------------------------------------------------*/

    for(int i = 1;i <= min_int(NB_FILES,2);++i){

        dir = dir_from_path(argv[i]);
        name = name_from_path(argv[i]);
        ext = ext_from_path(argv[i]);

        /*Print file info for debugging purposes*/
        printf("\nCurrent file:%s/%s.%s\n",dir,name,ext);

        /*Read the i-th picture.*/
        current_pic = read_picture(argv[i]);

        /* Get and print pic info */
        info_picture(current_pic);

        /*Certains traitements ne doivent être faits que pour des images en niveaux de gris, d'autres pour des images en couleur.*/
        if(is_gray_picture(current_pic)){
            convert_color_wrapper(current_pic,output_dir,name,ppm_ext);
            gaussian_blur_wrapper(current_pic,5,10.0,output_dir,name,pgm_ext);
        }
        if(is_color_picture(current_pic)){
            convert_gray_wrapper(current_pic,output_dir,name,pgm_ext);
            split_picture_wrapper(current_pic,output_dir,name,pgm_ext);
            normalize_color_picture_wrapper(current_pic,output_dir,name,ext);
        }

        /*Opérations ne dépendant pas du type d'image: brighten, melt,inverse, set_levels...*/
        brighten_picture_wrapper(current_pic,output_dir,name,ext,BRIGHTENING_FACTOR);
        melt_picture_wrapper(current_pic,output_dir,name,ext);


        /*On sauvegarde l'image inversée si la troisième est présente, car alors on mixe et on en aura besoin plus tard.*/
        inverse_picture_wrapper(current_pic,output_dir,name,ext,THIRD_IMAGE_FLAG,&save);
        
        /*Réduction du nombre de couleurs*/
        set_levels_wrapper(current_pic,output_dir,name,ext);

        /*Rééchantillonnages:*/
        resample_nearest_wrapper(current_pic,output_dir,name,ext,1.0/LARGER_FACTOR);/*Plus petit d'un facteur 1.36*/
        resample_nearest_wrapper(current_pic,output_dir,name,ext,LARGER_FACTOR);/*Plus grand d'un facteur 1.36*/
        
        resample_bilinear_wrapper(current_pic,output_dir,name,ext,1.0/LARGER_FACTOR);
        resample_bilinear_wrapper(current_pic,output_dir,name,ext,LARGER_FACTOR);

        /*Différence normalisée*/
        /*NB: on refait dans cette fonction ce qui a été fait plus haut / tôt, mais bon...*/
        normalized_difference_wrapper(current_pic,output_dir,name,ext,LARGER_FACTOR);

        brighten_lut_wrapper(current_pic,output_dir,name,ext,BRIGHTENING_FACTOR);

        if(THIRD_IMAGE_FLAG){
            /*On s'occupe du mixage selon le masque ici.*/
            mult_picture_wrapper(current_pic,mask,output_dir,name,ext);
            mix_picture_wrapper(save,current_pic,mask,output_dir,name,ext);
        }

        /*Free and reset memory*/
        
        clean_picture(&current_pic);
        if(THIRD_IMAGE_FLAG){
            clean_picture(&save);
        }
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

    if(THIRD_IMAGE_FLAG){
        clean_picture(&mask);
    }

    printf("Reste à tester le code avec:\n\t1)Des images d'entrées rectangulaires\n");
    printf("\t2)Des filtres en cascade\n");
    printf("pour s'assurer qu'il n'y a de problème avec aucune combinaison possible (couleur, largeur, hauteur)\n");

    return EXIT_SUCCESS;
}