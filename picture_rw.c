
#include <stdio.h> //Pour fopen, file...
#include <stdlib.h>//Pour exit
#include <string.h>//Pour strcmp, entre autres.
#include <assert.h>//Self-explanatory 
#include <limits.h>

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

/*Le code pour cette fonction était initialement inspiré de celui du cours. 
Je l'ai modifié au fur et à mesure pour inclure plus de 
vérifications et prendre en charge les commentaires.*/
#define NB_ITEMS_2_DIMENSIONS (2) 
#define NB_ITEM_1_MAX_VAL (1)
#define ITEM_SIZE (1) /*Nombre d'octets par item lu */

void reset_picture_to_zero(picture *p){
    p->width = 0;
    p->height = 0;
    p->chan_num = 0;
}
/*Renvoie faux en cas d'échec de lecture, vrai sinon.*/
bool fgets_encapsulator(char *buf, FILE *f, int line_counter){

    char *fgets_return_value = fgets(buf,BUFFER_SIZE, f);

    if(fgets_return_value == NULL){
        fprintf(stderr,"Could not read line %d (omitting comments) properly.\n",line_counter);
        fprintf(stderr,"Returning empty picture...\n");
        return false;
    }
    return true;
}   


picture read_picture(const char *filename){
    FILE *to_be_read = NULL;
    int max_val;/*Pourrait être un type byte si l'entrée était GARANTIE entre 0 et 255 mais on ne sait jamais...*/
    

    picture res;
 

    reset_picture_to_zero(&res);

    

    to_be_read = fopen(filename,"r");
    /*On a besoin d'un compteur pour savoir à quelle "vraie ligne" on est dans le traitement des commentaires 
    
    (on ne compte pas les lignes commençant par #)*/
    int line_counter = 1;

    if(to_be_read == NULL){
        fprintf(stderr,"Problem opening file. Returning empty picture...\n");
        /*Convention : en cas d'échec, on renvoie une structure "picture" vide.*/
       
        fclose(to_be_read);
        return res;
    }
    //On compare maintenant l'en-tête à celui attendu pour un fichier PGM ou PPM.
    char buffer[BUFFER_SIZE];

    /*On va passer de fgets à getline pour plus de sécurité. */
    if(!fgets_encapsulator(buffer,to_be_read,line_counter)){
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }
   

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
        fprintf(stderr,"\nNot a valid binary ppm nor pgm file. Returning empty picture...\n");
        fclose(to_be_read);
        return res;
       
    }
    line_counter++;/*On a lu la première ligne: il peut  désormais y a voir des commentaires.*/
    /*Gestion des commentaires: Après la première ligne et jusqu'à la quatrième, il peut y avoir des commentaires.*/

    if(!fgets_encapsulator(buffer,to_be_read,line_counter)){
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }
    /*Lire attentivement le man de sscanf pour être sûr de son comportement.*/
    int status = sscanf(buffer,"%d %d", &res.width, &res.height);

    if(status<NB_ITEMS_2_DIMENSIONS){
        fprintf(stderr,"\nError while reading image dimensions. Returning empty picture...\n");
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }


    printf("\nWidth read:%d\nHeight read:%d\n",res.width,res.height);

    /*On lit la troisième ligne, censée contenir la valeur maximale des pixels à lire.*/
    if(!fgets_encapsulator(buffer,to_be_read,line_counter)){
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }

    status = sscanf(buffer,"%d",&max_val);
    if(status<NB_ITEM_1_MAX_VAL){
        fprintf(stderr,"\nError while reading max byte value. Returning empty picture...\n");
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
       
    }
    if(!(1 <= max_val && max_val <= 255)){
        fprintf(stderr,"\nError: invalid max byte value. Returning empty picture...\n");
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }
    //assert(1 <= max_val && max_val <= 255);
    /*Après la première ligne et avant la quatrième ligne il peut y avoir un nombre indéterminé de lignes
     commençant par le caractère # que l’on doit considérer comme des lignes de commentaires et donc ignorer.
    La quatrième ligne ... ou tout ce qui suit correspond aux données binaires des pixels. 
    Dans le cas présenté ci-dessus il y a donc 512×512×3=786432 octets à lire avant la fin du fichier.*/

    size_t alloc_size = res.width*res.height*res.chan_num*ITEM_SIZE;
    res.data = myalloc(alloc_size);
    
    size_t nb_read_items = fread(res.data,ITEM_SIZE,alloc_size, to_be_read);
    printf("\n%ld items read.\n",nb_read_items);
    /*Peut-on détecter à ce stade le problème invalid_read_count ?*/
    if(alloc_size!=nb_read_items){
        free(res.data);
        reset_picture_to_zero(&res);
        fprintf(stderr,"Expected number of items doesn't match number of items read.\n");
        fprintf(stderr,"Expected number of items:%ld\n",alloc_size);
        fprintf(stderr,"Number of items read:%ld\n",nb_read_items);
        fclose(to_be_read);
        return res;
    }
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