
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "safe_malloc.h"
#include "pictures.h"
/*#include "pixels.h"*/
/*
La troisième ligne contient la valeur maximale des pixels à lire dans ce qui suit ...: Ici 255.

    Cette valeur maximale peut varier entre 1 et 255.
    Si cette valeur était 64 cela voudrait dire qu’un pixel composé des valeurs (r=64,v=64,b=64)
     correspondrait à la couleur blanche, alors que si le maximum est de 255 un pixel (r=64,v=64,b=64) correspond à du gris foncé.
    Pour simplifier les choses nous ne prendrons pas en compte cette valeur maximale dans nos structures de données
     (ce qui revient implicitement à considérer une valeur maximale de 255 dans nos images).

      Néanmoins, dans ce cas il faudra la prendre en compte lors de la lecture des fichiers
       et corriger les valeurs des composantes des pixels (par 255max) pour qu’elles soient comprises dans l’intervalle [0⋯255]
        au lieu de la valeur maximale indiquée dans la 3ème ligne du fichier.

Après la première ligne et avant la quatrième ligne il peut y avoir un nombre indéterminé de 
lignes commençant par le caractère # que l’on doit considérer comme des lignes de commentaires et donc ignorer.
La quatrième ligne ... ou tout ce qui suit correspond aux données binaires des pixels.
 Dans le cas présenté ci-dessus il y a donc 512×512×3=786432 octets à lire avant la fin du fichier.

*/

/*
TODO :
- implémenter: ignorer les commentaires
- sanitize input...
- prendre en compte la valeur maximale de la troisième ligne (en "bonus").
*/
#define BUFFER_SIZE 32
/*
    Fonction read_picture
    @param filename chemin vers un fichier
    @requires filename chemin valide vers un fichier .pgm ou .ppm valide
    @assigns rien
    @return une structure de type "picture"
*/
picture read_picture(char *filename){
    FILE *to_be_read = NULL;

    picture res;
    to_be_read = fopen(filename,"r");

    if(to_be_read == NULL){
        fprintf(stderr,"Problème de lecture dans la fonction read_picture.\n");
        //TODO: utiliser errno
        return res;
    }
    //On compare maintenant l'en-tête à celui attendu pour un fichier PGM ou PPM.
    char buffer[BUFFER_SIZE];
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
        perror("Not binary ppm nor pgm file.Aborting...\n");
        exit(1);
    }
    fgets( buffer ,BUFFER_SIZE , to_be_read );
    sscanf(buffer, " %d %d " , &res.width , &res.height);

    printf("Width read:%d\nHeight read:%d\n",res.width,res.height);
    fgets( buffer ,BUFFER_SIZE , to_be_read );

    int alloc_size = res.width*res.height*res.chan_num;
    res.data = myalloc (alloc_size);
    
    fread (res.data , res.chan_num , alloc_size , to_be_read );

    return res;
}
/*
    Fonction write_picture
    @param filename chemin vers un fichier
    @requires "p" structure de type "picture" valide (avec champs width, height, chan_num et data valides).
    @requires "filename" chemin (potentiel) valide.
    @assigns rien
    @return un entier indiquant le statut de succès(0) ou d'échec de l'appel à la fonction.
*/
int write_picture(picture p, char * filename){
    FILE *f = fopen(filename , "wb");
    if(f == NULL){
        fprintf(stderr,"Erreur d'écriture dans la fonction write_picture\n");
        return 1; //cf. man fopen.
    }
    if(p.chan_num == RGB_PIXEL_SIZE){
        fprintf(f, "P6 \n%d %d\n255\n", p.width, p.height);
    }else if(p.chan_num == BW_PIXEL_SIZE){
        fprintf(f, "P5 \n%d %d\n255\n", p.width, p.height);
    }else{
        fprintf(stderr,"Erreur de canal dans la fonction write_picture.\n");
        return 1;
    }
    fwrite(p.data ,p.chan_num, p.width * p.height, f);
    fclose (f);
    return 0;
}
