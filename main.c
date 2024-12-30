#include <stdio.h>
#include <stdlib.h>
#include "pictures.h"
#include "filename.h"
#include <string.h>
#include <assert.h>
/*
    Un code abondamment commenté ; 
    la première partie des commentaires comportera systématiquement les lignes :

    @requires décrivant les préconditions :
     c’est-à-dire conditions sur les paramètres
      pour une bonne utilisation (pas de typage ici),
    
    @assigns listant les zones de mémoire modifiées,
    @ensures décrivant la propriété vraie à la sortie
    de la fonction lorsque les préconditions sont respectées,
    le cas échéant avec mention des comportements en cas de succès
    et en cas d’échec,
    En outre chaque boucle while doit contenir un commentaire
    précisant la raison de sa terminaison (le cas échéant).
    De même en cas d’appels récursifs.
    On pourra préciser des informations additionnelles
    si des techniques particulières méritent d’être mentionnées.

*/

//conv2col_wrapper (better code readability)
//@ requires p is a grayscale (pgm) picture ?

void conv2col_wrapper(picture p, char *dir_p,char *name_p, char *ext_p);

void conv2col_wrapper(picture p, char *dir_p,char *name_p, char *ext_p){
    picture conv2col = convert_to_color_picture(p);
    char conv2col_op[9] = "conv2col"; //Check wanted filename
    char *conv2col_concat = concat_parts(dir_p,name_p,conv2col_op,ext_p);
    write_picture(conv2col,conv2col_concat);
    clean_picture(&conv2col);
    free(conv2col_concat);
}

int main(int argc, char* argv[]){
    /*Votre programme principal devra consister en la lecture d’une ou plusieurs images source depuis des fichiers (PGM ou PPM)
    fournis en arguments du programme. Exemple :

    ./projet Lenna_gray.pgm Lenna_color.ppm

    REMARQUE: j'avais donc mal interprété la consigne au début. Le deuxième argument n'est pas la destination mais un autre fichier à traiter.

    Il va falloir effectuer une boucle sur les arguments, mais dans un premier temps on peut supposer qu'il y en a exactement 2.

    -> En fait non, pour éviter d'avoir à revenir sur cette partie du code, je vais directement gérer n entrées à l'aide d'une boucle.

    */

    /*TODO: factoriser les tests qui se ressemblent à l'aide d'une fonction supplémentaire, déclarée avant le main.*/



    /*-------------------------Déclaration des variables en amont (conforme à la norme ISO C)-------------------------------*/

    picture current_pic; /*Variable qui sera mise à jour au fur et à mesure dans une boucle sur les arguments.*/

    /*On évite de créer un tableau de "pictures" pour des raisons d'économie de mémoire. On effectue plutôt 
    les traitements au fur et à mesure, argument par argument, fonction par fonction en libérant la mémoire utilisée dès que possible/nécessaire.
    */

    char output_path[13] = "Lenna_output"; /*Chemin pour produire les images afin de ne pas mélanger les entrées et les sorties*/

  

    char ppm_ext[4] = "ppm";
    char pgm_ext[4] = "pgm";
    
    /*Vérifications d'usage sur la bonne utilisation du programme. Proposer plus loin une aide pour les différentes options.*/
    if(argc<2){
        fprintf(stderr,"Usage:./prog <f1.[pgm|ppm]>...<fn.[pgm|ppm]>\n");
        return EXIT_FAILURE;
    }


    /*------------------------------------Récupération des différents éléments constituant le chemin------------------------------------*/

    const int NB_FILES = argc-1;


    //Now why would we store everything into an array ? No reason. Just keep the current file info...
    char *dir;
    char *name; 
    char *ext; 

    //printf("dir:%s,name:%s,ext:%s\n",dir,name,ext);

    /*
        TODO: loop through a picture array.
        
        Be smart: do not load every picture into memory before processing them.  SO NOT A PICTURE ARRAY, JUST A FILENAME ARRAY.
        Process the current picture (picture current_pic), free the memory and then go to the next one.argc

    */

    /*----------------------------------------------Boucle principale-------------------------------------------------------------------*/

    for(int i = 1;i <= NB_FILES;++i){
        dir = dir_from_path(argv[i]);
        name = name_from_path(argv[i]);
        ext = ext_from_path(argv[i]);

        /*Read the i-th picture.*/
        current_pic = read_picture(argv[i]);
        /* Get and print pic info */
        
        info_picture(current_pic);
    

        /*Free and reset memory*/
        clean_picture(&current_pic);//Check the prototype clean_picture should have.



        free(dir);
        free(name);
        free(ext);
        dir = NULL;
        name = NULL;
        ext = NULL;
    }



    /*-------------------------------------------------Libération de la mémoire---------------------------------------------------------*/
    //free(dir);
    //free(name);
    //free(ext);
    
    
    /*clean_picture(&copy);*/
    
    return EXIT_SUCCESS;
}