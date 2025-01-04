
#include <stdio.h> //Pour fopen, file...
#include <stdlib.h>//Pour exit
#include <string.h>//Pour strcmp, entre autres.
#include <assert.h>//Self-explanatory 
#include <limits.h>//Pour INT_MAX, INT_MIN

#include "safe_malloc.h"
#include "picture_rw.h"
/*
    Fonction read_picture
    @param: filename chemin vers un fichier
    @requires: filename chemin valide vers un fichier .pgm ou .ppm valide
    @assigns: rien
    @return: une structure de type "picture"

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
        fprintf(stderr,"Could not read line %d properly.\n",line_counter);
        fprintf(stderr,"Returning empty picture...\n");
        return false;
    }
    return true;
}   

bool read_correctly_block(bool *read_correctly, char *buffer, FILE *to_be_read, picture *res, int *lc_p){
    /*On pourrait probablement utiliser une boucle "do while" ici.*/
    bool first_pass = true;
    /*
        Argument de terminaison: first_pass est faux à partir du second tour de boucle, et le fichier est de longueur
        finie, ce qui cause un retour "early return" via le pointeur nul renvoyé par fgets dans le cas où toutes les lignes sont des
        commentaires. Si une ligne ne commence pas par un croisillon, on s'arrête encore avant.
    */
    while(first_pass || buffer[0] == '#'){
        first_pass = false;
        *read_correctly = fgets_encapsulator(buffer,to_be_read,*lc_p);
        if(!*read_correctly){
            reset_picture_to_zero(res);
            fclose(to_be_read);
            return false;
        }
        *lc_p = 1 + *lc_p;
        if(buffer[0]=='#'){
            printf("Comment on line %d.\n",*lc_p);
        }
    }
    return true;
}

/*
    Convention/explication du nom des variables: on considère qu'une ligne
    est "intéressante" si elle n'est pas un commentaire.
    interesting_line_counter compte le nombre de lignes intéressantes
    true_line_counter compte le nombre de lignes total (avec les commentaires, donc.)
*/

picture read_picture(const char *filename){
    FILE *to_be_read = NULL;
    int max_val;/*Pourrait être un type byte si l'entrée était GARANTIE entre 0 et 255 mais on ne sait jamais...*/
    picture res;
    bool read_correctly = true; /*Sera utile pour déterminer si une ligne a été lue correctement ou pas.*/
    reset_picture_to_zero(&res);

    to_be_read = fopen(filename,"r");

    /*Avant l'ouverture du fichier, on n'a lu aucune ligne: on est à la ligne "0" (on comptera à partir de 1)*/
    int interesting_line_counter = 0;
    int true_line_counter = 0;

    if(to_be_read == NULL){
        fprintf(stderr,"Problem opening file. Returning empty picture...\n");
        /*Convention : en cas d'échec, on renvoie une structure "picture" vide.*/
        fclose(to_be_read);
        return res;
    }
    printf("File opened successfullly.\n");
    //On compare maintenant l'en-tête à celui attendu pour un fichier PGM ou PPM.
    char buffer[BUFFER_SIZE];

    read_correctly = fgets_encapsulator(buffer,to_be_read,true_line_counter);
    if(!read_correctly){
        /*Il y a déjà un message d'erreur affiché par fgets_encapsulator dans ce cas.*/
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }
    /*On a lu la première ligne avec succès:*/
    interesting_line_counter++;
    true_line_counter++;
    /*----------------------------------------------Identification du "magic number"--------------------------------------------------*/
    if(!strcmp(buffer,"P6\n")){
        printf("Reading ppm file...\n");
        res.chan_num = RGB_PIXEL_SIZE;
    }
    else if(!strcmp(buffer,"P5\n")){
        printf("Reading pgm file...\n");
        res.chan_num = GRAY_PIXEL_SIZE;
    }
    else{
        printf("%s:",filename);fflush(stdout);
        fprintf(stderr,"\nNot a valid binary ppm nor pgm file. Returning empty picture...\n");
        fclose(to_be_read);
        return res;
    }
    /*-------------------------------Commentaires éventuels : entre lignes "intéressantes" 1 et 2 ---------------------------------------*/
    if(!read_correctly_block(&read_correctly,buffer,to_be_read,&res,&true_line_counter)){
        return res;
    }
    /*-------------------------------Lecture des dimensions de l'image.------------------------------------------------------------------*/
    int status = sscanf(buffer,"%d %d", &res.width, &res.height);
    
    if(status<NB_ITEMS_2_DIMENSIONS){
        fprintf(stderr,"\nError while reading image dimensions. Returning empty picture...\n");
        reset_picture_to_zero(&res);
        fclose(to_be_read);
        return res;
    }
    printf("\nWidth read:%d\nHeight read:%d\n",res.width,res.height);


    /*On est arrivé à la deuxième ligne, sans compter les commentaires.*/
    interesting_line_counter++;


    /*-------------------------------Commentaires éventuels : entre lignes "intéressantes" 2 et 3 ---------------------------------------*/
    if(!read_correctly_block(&read_correctly,buffer,to_be_read,&res,&true_line_counter)){
        return res;
    }

    /*Lecture de la valeur maximale:*/
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
    /*-------------------------------On a lu la "vraie"troisième ligne: plus de commentaires. ---------------------------------------*/

    size_t alloc_size = res.width * res.height * res.chan_num * ITEM_SIZE;
    res.data = myalloc(alloc_size);
    
    size_t nb_read_items = fread(res.data,ITEM_SIZE,alloc_size, to_be_read);
    printf("\n%ld items read.\n",nb_read_items);
    /*Peut-on détecter à ce stade le problème invalid_read_count ?*/
    
    if(alloc_size != nb_read_items){
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
    @param: filename chemin vers un fichier
    @requires: "p" structure de type "picture" valide (avec champs width, height, chan_num et data valides).
    @requires: "filename" chemin (potentiel) valide.
    @assigns: rien
    @ensures: ce qu'il faut
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
    }else if(p.chan_num == GRAY_PIXEL_SIZE){
        fprintf(f, "P5\n%d %d\n255\n", p.width, p.height);
    }else{
        fprintf(stderr,"Erreur de canal dans la fonction write_picture.\n");
        return 1;
    }
    fwrite(p.data ,p.chan_num, p.width * p.height, f);
    fclose (f);
    return 0;
}