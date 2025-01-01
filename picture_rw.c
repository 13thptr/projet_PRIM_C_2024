
#include <stdio.h> //Pour fopen, file...
#include <stdlib.h>//Pour exit
#include <string.h>//Pour strcmp, entre autres.
#include <assert.h>//Self-explanatory 

#include "safe_malloc.h"
#include "picture_rw.h"
/*
    Fonction read_picture
    @param filename chemin vers un fichier
    @requires filename chemin valide vers un fichier .pgm ou .ppm valide
    @assigns rien
    @return une structure de type "picture"

    TODO: replace strcmp with strncmp ? + Increase general safety 
*/
picture read_picture(const char *filename){
    FILE *to_be_read = NULL;

    picture res;
    res.width = 0;
    res.height = 0;
    res.chan_num = 0;


    to_be_read = fopen(filename,"r");

    if(to_be_read == NULL){
        fprintf(stderr,"Problem reading ppm or pgm file.Aborting...\n");
        /*Convention : en cas d'échec, on renvoie une structure "picture" vide.*/
        exit(1);
        return res;
    }
    //On compare maintenant l'en-tête à celui attendu pour un fichier PGM ou PPM.
    char buffer[BUFFER_SIZE];

    /*On va passer de fgets à getline pour plus de sécurité. */
    fgets (buffer,BUFFER_SIZE, to_be_read);
   

    if(!strcmp(buffer,"P6\n")){
        printf("Reading ppm file...\n");
        res.chan_num = RGB_PIXEL_SIZE;
    }
    else if(!strcmp(buffer,"P5\n")){
        printf("Reading pgm file...\n");
        res.chan_num = BW_PIXEL_SIZE;
    }
    else{
        printf("%s:",filename);fflush(stdout);
        perror("Not a binary ppm nor a pgm file.Aborting...\n");
        exit(1);
    }
    fgets(buffer ,BUFFER_SIZE , to_be_read );
    sscanf(buffer, " %d %d " , &res.width , &res.height);

    assert(res.width>0&&res.height>0&&(res.chan_num==RGB_PIXEL_SIZE||res.chan_num==BW_PIXEL_SIZE));

    printf("Width read:%d\nHeight read:%d\n",res.width,res.height);
    fgets( buffer ,BUFFER_SIZE , to_be_read );

    int alloc_size = res.width*res.height*res.chan_num;
    res.data = myalloc (alloc_size);
    
    fread(res.data, res.chan_num, alloc_size, to_be_read);

    /*Important: Ne pas oublier de fermer le fichier. */
    fclose(to_be_read);

    return res;
}
/*
    Fonction write_picture
    @param filename chemin vers un fichier
    @requires "p" structure de type "picture" valide (avec champs width, height, chan_num et data valides).
    @requires "filename" chemin (potentiel) valide.
    @assigns rien
    @ensures ce qu'il faut
    @return un entier indiquant le statut de succès(0) ou d'échec de l'appel à la fonction.
*/
int write_picture(picture p, char * filename){
    FILE *f = fopen(filename , "wb");
    if(f == NULL){
        fprintf(stderr,"Erreur d'écriture dans la fonction write_picture\n");
        return 1; //cf. man fopen.
    }
    if(p.chan_num == RGB_PIXEL_SIZE){
        fprintf(f, "P6\n%d %d\n255\n", p.width, p.height);
    }else if(p.chan_num == BW_PIXEL_SIZE){
        fprintf(f, "P5\n%d %d\n255\n", p.width, p.height);
    }else{
        fprintf(stderr,"Erreur de canal dans la fonction write_picture.\n");
        return 1;
    }
    fwrite(p.data ,p.chan_num, p.width * p.height, f);
    fclose (f);
    return 0;
}