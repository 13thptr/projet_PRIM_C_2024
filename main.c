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

    char **dir = malloc((argc-1)*sizeof(char*)); 
    char **name = malloc((argc-1)*sizeof(char*));
    char **ext = malloc((argc-1)*sizeof(char*)); 

    //printf("dir:%s,name:%s,ext:%s\n",dir,name,ext);

    /*
        TODO: loop through a picture array.
        
        Be smart: do not load every picture into memory before processing them.  SO NOT A PICTURE ARRAY, JUST A FILENAME ARRAY.
        Process the current picture (picture current_pic), free the memory and then go to the next one.argc

    */

    for(int i = 1;i<=argc;i++){
        dir[]
    }

    /*----------------------------------------------Test des fonctions d'interrogation-------------------------------------------------*/
    if(is_empty_picture(pic)){
        printf("Empty picture. Aborting...\n");
        return EXIT_FAILURE;
    }else{
        printf("Non-empty picture. Proceeding...\n");
    }
    /*--------------------------------------------------------Tests principaux---------------------------------------------------------*/

    /*Conversion : GRAY->RGB*/
    picture conv2col = convert_to_color_picture(pic1);/*TODO: instead of hardcoded first-argument conversion, convert every grayscale image.*/
    char conv2col_op[9]="conv2col";
    
    char *conv2col_concat = concat_parts(dir,name,conv2col_op,ppm_ext);
    write_picture(conv2col,conv2col_concat);
    clean_picture(&conv2col);
    free(conv2col_concat);



    /*-------------------------------------------------Libération de la mémoire---------------------------------------------------------*/
    free(dir);
    free(name);
    free(ext);
    
    
    /*clean_picture(&copy);*/
    clean_picture(&pic);
    return EXIT_SUCCESS;
}