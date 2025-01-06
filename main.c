
/*TODO: examiner à la main l'arbre des includes et enlever ceux qui ne sont pas nécessaires.*/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "filename.h"
#include "picture_rw.h"
#include "wrappers.h"
#include "safe_malloc.h"



#define LARGER_FACTOR (1.36) /*resampling functions*/
#define BRIGHTENING_FACTOR (1.5)/*brightening functions*/
#define STANDARD_DEVIATION (10.0)
#define KERNEL_SIZE (8) 


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
   

    struct stat st;

    if (stat(output_dir, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            printf("Le dossier de sortie '%s' existe déjà, on continue...\n", output_dir);
        } else {
            fprintf(stderr, "'%s' existe mais n'est pas un dossier.\n", output_dir);
            fprintf(stderr,"Ce n'est pas censé arriver...");
            return EXIT_FAILURE;
        }
    } else {
        if (mkdir(output_dir, 0755) == 0) {
            printf("Dossier de sortie'%s' créé avec succès.\n", output_dir);
        } else {
            perror("mkdir");
            return EXIT_FAILURE;
        }
    }
    char ppm_ext[4] = "ppm";
    char pgm_ext[4] = "pgm";
    
    /*Vérifications d'usage sur la bonne utilisation du programme. Proposer plus loin une aide pour les différentes options.*/
    if(argc<2){
        fprintf(stderr,"Usage:./prog <f1.[pgm|ppm]> <f2.[pgm|ppm]> (opt) <f3.[pgm|ppm]>\n");
        return EXIT_FAILURE;
    }
    if(argc>4){
        fprintf(stderr,"\nThe fourth argument and the subsequent ones will be ignored.\n");
    }

    const int NB_FILES = argc-1;
    char *dir;
    char *name; 
    char *ext;

    bool THIRD_IMAGE_FLAG = false;
    /*Chargement de la troisième image (optionnel)*/

    if(argc >= 4){
        printf("Reading mask...\n");
        mask = read_picture(argv[3]);
        THIRD_IMAGE_FLAG = true;
    }

    /*----------------------------------------------Boucle principale-------------------------------------------------------------------*/

    for(int i = 1;i <= min_int(NB_FILES,2);++i){
        printf(" Début de boucle, indice:%d\n",i);
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
            /*L'extension est bien ppm car on CONVERTIT le fichier en niveaux 
            de gris vers un fichier en couleur*/
            
            convert_color_wrapper(current_pic,output_dir,name,ppm_ext);
            /*Ici, c'est bien un pgm.*/
            normalize_picture_wrapper(current_pic,output_dir,name,pgm_ext);
            
        }
        if(is_color_picture(current_pic)){
            /*Les deux sont bien des pgm (conversion/canaux différents)*/
            convert_gray_wrapper(current_pic,output_dir,name,pgm_ext);
            split_picture_wrapper(current_pic,output_dir,name,pgm_ext);
            /*Bien un ppm*/
            normalize_color_picture_wrapper(current_pic,output_dir,name,ppm_ext);
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


        /*Bonus: flou gaussien*/
        gaussian_blur_wrapper(current_pic,KERNEL_SIZE,STANDARD_DEVIATION,output_dir,name,pgm_ext);

        /*Dérivée horizontale*/
        sobel_filter_wrapper(current_pic,output_dir,name,pgm_ext);

        printf("checkpoint\n");
        /*Free and reset memory*/
        
        clean_picture(&current_pic);
        printf("checkpoint\n");
        if(THIRD_IMAGE_FLAG){
            clean_picture(&save);
        }
        /*
            NOTE "bug" (pas de mon fait mais dû aux choix de filename.[h|c]
            IL FAUT QUE DIR EXISTE, DONC LES ARGUMENTS DOIVENT ETRE PASSES SOUS LA FORME ./arg1, ./arg2, etc.
        */
        free(dir);
        free(name);
        
        free(ext);
       
        dir = NULL;
        name = NULL;
        ext = NULL;
    }
    //clean_picture(&current_pic);

    if(THIRD_IMAGE_FLAG){
        clean_picture(&mask);
    }

    return EXIT_SUCCESS;
}