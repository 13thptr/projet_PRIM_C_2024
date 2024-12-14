#include "pictures.h"
/*#include "pixels.h"*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 32
/*
    Fonction read_picture
    @param filename chemin vers un fichier
    @requires filename chemin valide vers un fichier .pgm ou .ppm valide
    @assigns rien
    @return une structure de type "picture"
*/
picture read_picture(char * filename){
    /*
        TODO:
        1)sanitize input./handle malformed input.
        2)check if malloc went well.
        3)rewrite code with all flags on.
        4)use errno & handle errors better.
    */
    FILE *to_be_read = NULL;
    picture res;
    to_be_read = fopen(filename,"r");
    res.height = 0;
    res.width = 0;
    res.chan_num = GREY_PICTURE;
    res.data = NULL;

    if(!to_be_read){
        fprintf(stderr,"Problème de lecture dans la fonction read_picture.\n");
        //TODO: utiliser errno
        return res;
    }
    //On sait que sizeof(byte)==sizeof(char)==1.
    
    //typedef unsigned char pixel ; //A enlever.

    char buffer[BUFFER_SIZE]; //Pourquoi 128 ? Diminuer si c'est possible.
    //int width , height ;    //A supprimer.
    //byte *btm = NULL ;

    //On récupère les 128 premiers caractères du fichier to_be_read.
    /*
    man page:  fgets()  reads in at most one less than size characters from stream and stores them into the buffer pointed to by s.
      Reading stops after an EOF or a newline.  If a newline is read, it is
       stored into the buffer.  A terminating null byte ('\0') is stored after the last character in the buffer.
       ->fgets lit en gros une ligne.
    */
    fgets (buffer,BUFFER_SIZE, to_be_read);

    //On compare l'en-tête à celui attendu pour un fichier PGM.
    if(strcmp(buffer , "P5\n")){
        perror("not binary pgm file \n");
        exit(1);//semble redondant vu man perror ? mais en fait non.
    }
    //On lit à nouver les 128 caractères ?
    fgets( buffer ,BUFFER_SIZE , to_be_read );
    sscanf(buffer, " %d %d " , &res.width , &res.height);

    printf("Width read:%d\nHeight read:%d\n",res.width,res.height);
    fgets( buffer ,BUFFER_SIZE , to_be_read );

    int alloc_size = res.width*res.height*BW_PIXEL_SIZE;//sizeof(byte)==sizeof(unsigned char)==1
    res.data = malloc (alloc_size);
    fread (res.data , BW_PIXEL_SIZE , alloc_size , to_be_read );
    
    return res;
}

int write_picture(picture p, char * filename){
    filename[0] = 3+p.width;
    
    return 0;
}
