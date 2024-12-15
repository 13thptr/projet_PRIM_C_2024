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
/* 
On vous fournit un module filename.[h/c] qui vous permettra de
Séparer un chemin vers un fichier image <dirname>/<name>.<ext> en ses composantes <dirname>, <name> et <ext>
Composer un chemin vers un fichier image à partir de ces composantes.
Vous pourrez ainsi extraire le nom du fichier image traité par votre programme
 et le customiser avec les noms des opérations que vous appliquerez dessus avant*/
    picture test;
    char* test_path;

    char ppm_ext[4] = "ppm";
    char pgm_ext[4] = "pgm";
    
    if(argc!=3){
        fprintf(stderr,"Usage:./prog <filename.[pgm|ppm]> <path_to_new_file.[pgm|ppm]\n");
        return EXIT_FAILURE;
    }
    test_path = argv[2];

    test = read_picture(argv[1]);

    /*Test des fonctions d'interrogation*/
    if(is_empty_picture(test)){
        printf("Empty picture.\n");
    }else{
        printf("Non-empty picture.\n");
    }
    printf("%d\n",is_gray_picture(test));
    printf("%d\n",is_color_picture(test));
    info_picture(test);

    write_picture(test,test_path);

    picture copy = copy_picture(test);
    info_picture(copy);
    char *dir = dir_from_path(argv[1]);
    char *name = name_from_path(argv[1]);
    char copy_op[5] = "copy"; //Cette syntaxe est nécessaire, on ne peut utiliser char* pour faire les choses proprement.
    char *ext = ext_from_path(argv[1]);
    char *copy_concat = concat_parts(dir,name,copy_op,ext);
    write_picture(copy,copy_concat);
    free(copy_concat);

    /*Test: conversion.*/
    /*GRAY>RGB*/
    picture conv2col = convert_to_color_picture(test);
    char conv2col_op[9]="conv2col";
    
    char *conv2col_concat = concat_parts(dir,name,conv2col_op,ppm_ext);
    write_picture(conv2col,conv2col_concat);
    clean_picture(&conv2col);
    free(conv2col_concat);
    
    /*RGB->GRAY*/
    picture conv2bw = convert_to_gray_picture(test);
    char conv2bw_op[9]= "conv2bw";
    
    char *conv2bw_concat = concat_parts(dir,name,conv2bw_op,pgm_ext);
    write_picture(conv2bw,conv2bw_concat);

    /*Eclaircissement: on utilise l'image convertie en noir et blanc. RGB->BW, que l'on libérera plus bas*/
    char brighten_op[9]="brighten";
    char *brighten_concat = concat_parts(dir,name,brighten_op,ext); 
    picture brightened = brighten_picture(conv2bw,1.5);

    write_picture(brightened,brighten_concat);
    clean_picture(&brightened);
    free(brighten_concat);

    clean_picture(&conv2bw);
    free(conv2bw_concat);

    /*Split & merge*/
    picture *split_test = split_picture(test);

    char split_shared_str[7] = "split_";
    for(char k=0;k<(char)test.chan_num;k++){
        printf("k:%d\n",(int)k);
        char *split_individualized_channel = malloc(sizeof(char)*8);

        size_t max_length = 8;
        strncpy(split_individualized_channel,split_shared_str,max_length);
        split_individualized_channel[6] = 48+k;
        split_individualized_channel[7] = '\0';
        char *split_channel_concat = concat_parts(dir,name,split_individualized_channel,pgm_ext);

        write_picture(split_test[(int)k],split_channel_concat);

        free(split_individualized_channel);
        free(split_channel_concat);
        
    }
    
    char merge_op[6] = "merge";
    char *merge_concat = concat_parts(dir,name,merge_op,ppm_ext);
    picture merged;
    if(test.chan_num == RGB_PIXEL_SIZE){
        merged = merge_picture(split_test[0],split_test[1],split_test[2]);
    }else{
        assert(test.chan_num == BW_PIXEL_SIZE);
        merged = merge_picture(split_test[0],split_test[0],split_test[0]);
    }
    

    write_picture(merged,merge_concat);
    clean_picture(&merged);
    free(merge_concat);
    for(char k=0;k<(char)test.chan_num;k++){
        clean_picture(&split_test[(int)k]);
    }
    free(split_test);

    
    picture melted = melt_picture(test,test.width*test.height*test.chan_num*5);
    char melted_op[7]="melted";
    char *melted_path = concat_parts(dir,name,melted_op,ext);

    write_picture(melted,melted_path);
    clean_picture(&melted);
    free(melted_path);

    /*On libère les chemins*/
    free(dir);
    free(name);
    free(ext);
    
    
    clean_picture(&copy);
    clean_picture(&test);
    return EXIT_SUCCESS;
}