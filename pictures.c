
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "safe_malloc.h"
#include "pictures.h"
#include "pixels.h"
#include "lut.h"
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

    TODO: replace strcmp with strncmp ? + Increase general safety 
*/
picture read_picture(const char *filename){
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
        printf("%s:",filename);fflush(stdout);
        perror("Not a binary ppm nor a pgm file.Aborting...\n");
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
/**
 * Création d'une image (sans)
 * @param [in] width la largeur de l'image à créer
 * @param [in] height la hauteur de l'image à créer
 * @param [in] channels le nombre de canaux

 * @assigns value  zones de mémoires modifiéres
 * 
 * @ensures 
 * @ensures
 *   
 * @return rien
 */
picture create_picture(unsigned int width, unsigned int height, unsigned int channels){
    //assert(width>0&&height>0);
    assert(channels == RGB_PIXEL_SIZE||channels == BW_PIXEL_SIZE);
    //printf("max:%d\n",max);//éviter "unused parameter". à enlever.
    picture res;
    res.width = width;
    res.height = height;
    res.chan_num = channels;
    res.data = myalloc(width*height*channels);
    return res;
}
void clean_picture(picture *p){
    assert(p!=NULL);
    p->chan_num = 0;
    p->width = 0;
    p->height = 0;
    if(p->data!=NULL){
        free(p->data);
    }
}
picture copy_picture(picture p){
    assert(p.width>0&&p.height>0);
    assert(p.data!=NULL);
    assert(p.chan_num == BW_PIXEL_SIZE || p.chan_num == RGB_PIXEL_SIZE);

    picture res = create_picture(p.width,p.height,p.chan_num);
    for(int k = 0; k<p.width*p.height*(int)p.chan_num;k++){
        res.data[k] = p.data[k];
    }
   
    return res;
}/*
Obtention d’informations sur une image

    Indication d’image vide (si un de ses champs est nul) : int is_empty_picture(picture p);

        [in] p l’image à inspecter
        [out] une valeur non nulle si p est vide, 0 sinon.
*/
bool is_empty_picture(picture p){
    return !(p.width&&p.height&&p.chan_num);
}
bool is_gray_picture(picture p){
    return p.chan_num == BW_PIXEL_SIZE;
}
bool is_color_picture(picture p){
    return p.chan_num == RGB_PIXEL_SIZE;
}
void info_picture(picture p){
    printf("%d x% d x %d\n",p.width,p.height,p.chan_num);
}
bool same_dimensions(picture p1, picture p2){
    return p1.chan_num==p2.chan_num&&p1.width==p2.width&&p1.height==p2.height;
}
/*
Convertir une image en niveau de gris vers une image en couleur : picture convert_to_color_picture(picture p);
        en répétant les valeurs de niveau de gris dans chaque canal R, V, B.
        [in] p l’image à convertir en couleurs
        [out] l’image couleur convertie en couleurs.
            Si p était déjà en couleur on se contentera de faire une copie
            Si pétait une image en niveaux de gris on répétera la composante de niveau de gris dans chacune des composantes (rouge, vert, bleu) de l’image résultat.
*/
picture convert_to_color_picture(picture p){
    assert(!is_empty_picture(p));

    if(p.chan_num == RGB_PIXEL_SIZE){
        return copy_picture(p);
    }
    assert(p.chan_num == BW_PIXEL_SIZE);
    picture res = create_picture(p.width,p.height,RGB_PIXEL_SIZE);

    /*Version alternative, sans utiliser les fonctions déjà écrites.
    for(int k=0;k<p.width*p.height*(int)p.chan_num;k++){
        byte value = p.data[k];
        res.data[3*k] = value;
        res.data[3*k+1] = value;
        res.data[3*k+2] = value; 
    }*/
    
    for(int i=0;i<res.height;i++){
        for(int j=0;j<res.width;j++){
            byte value = read_component_bw(p,i,j);
            write_pixel_rgb(res,i,j,value,value,value);
        }
    }
    return res;
}

/* 
    Convertir une image en couleur vers une image en niveaux de gris : picture convert_to_gray_picture(picture p);
        [in] p l’image à convertir en niveaux de gris
        [out] l’image convertie en niveaux de gris
            Si p était un image en couleur on a convertie les couleurs en niveau de gris en utilisant la règle suivante : G=(0.299×R)+(0.587×V)+(0.114×B).
            Si p était déjà une image en niveau de gris on se contentera d’en faire une copie

*/
picture convert_to_gray_picture(picture p){
    assert(!is_empty_picture(p));

    if(is_gray_picture(p)){
        return copy_picture(p); //Enoncé: on se contentera d'en faire une copie
    }
    assert(is_color_picture(p));
    picture res = create_picture(p.width,p.height,BW_PIXEL_SIZE);

    for(int i=0;i<res.height;i++){
        for(int j=0;j<res.width;j++){
            double red = (double)0.299*read_component_rgb(p,i,j,RED);
            double green = (double)0.587*read_component_rgb(p,i,j,GREEN);
            double blue = (double)0.114*read_component_rgb(p,i,j,BLUE);
            double sum = red+green+blue;
            byte value = (byte)(sum<255?sum:255);
            write_pixel_bw(res,i,j,value);
        }
    }
    return res;
}
/*
Séparation ou mélange des composantes d’une image

    Séparer les composantes d’une image couleur en 3 images en niveau de gris contenant les valeurs pour le rouge, le vert et le bleu respectivement : 
    picture * split_picture(picture p);
        [in] p l’image couleur dont on veut séparer les composantes
        [out] un tableau de 3 images en niveau de gris contenant les valeurs des canaux R, V et B.
            Si p ne peut pas être décomposée on se contentera de renvoyer NULL.
            Si p est une image en niveaux de gris on renverra un tableau ne contenant qu’un seul élément.

*/
picture *split_picture(picture p){
    if(is_empty_picture(p)){
        return NULL;
    }
    picture *arr = malloc(sizeof(picture)*p.chan_num);
    for(int n=0;n<(int)p.chan_num;n++){
        arr[n] = create_picture(p.width,p.height,BW_PIXEL_SIZE);
       
        for(int i= 0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte component = (p.chan_num==BW_PIXEL_SIZE?read_component_bw(p,i,j):read_component_rgb(p,i,j,n));
                write_pixel_bw(arr[n],i,j,component); //ici il n'y a pas de disjonction, on écrit toujours dans une image en niveau de gris.
            }
        }

    }
    return arr;
}

/*
    Mélanger les composantes à partir de 3 images en niveau de gris pour composer une image couleurs : 
    picture merge_picture(picture red, picture green, picture blue);
        [in] red l’image en niveau de gris à utiliser pour fabriquer la composante rouge de l’image résultat.
        [in] green l’image en niveau de gris à utiliser pour fabriquer la composante verte de l’image résultat.
        [in] blue l’image en niveau de gris à utiliser pour fabriquer la composante bleue de l’image résultat.
        [out] l’image composée
            Si l’image résultat ne peut pas être créée (si par exemple les trois images red, green et blue ne sont pas de même taille ou type) on se contentera de renvoyer une image vide.

*/
picture merge_picture(picture red, picture green, picture blue){
    int width = red.width * (red.width==green.width&&green.width == blue.width);
    //printf("width:%d\n",width);
    int height = red.height *(red.height==green.height&&green.height==blue.height);
    //printf("height:%d\n",height);
    enum channel_number chan_num = 3;

    picture res = create_picture(width,height,chan_num);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            byte r = read_component_bw(red,i,j);
            byte g = read_component_bw(green,i,j);
            byte b = read_component_bw(blue,i,j);
            write_pixel_rgb(res,i,j,r,g,b);
        }
    }
    return res;
}

picture brighten_picture(picture p, double factor){
    picture res = copy_picture(p);
    for(unsigned int k=0;k<res.width*res.height*res.chan_num;k++){
        double value = (double)res.data[k]*factor;
        value = (value <(double)MAX_BYTE)?value:(double)MAX_BYTE; 
        res.data[k] = (byte)value;
    }
    return res;
}

picture melt_picture(picture p, int number){
    /*On choisit N pixels au hasard dans l’image. Pour chacun de ces pixels,
    si le pixel situé juste au dessus est plus sombre que le pixel choisi alors le pixel courant prends la valeur du pixel du dessus.
    [in] p l’image à faire fondre
    [in] number le nombre de pixels à choisir aléatoirement
    [out] l’image contenant les pixels qui ont fondu vers le bas*/

    /*Il faut distinguer deux cas: image en niveaux de gris ou couleur.*/

    picture melted = copy_picture(p);

    if(p.chan_num == BW_PIXEL_SIZE){
        for(int pix = 0;pix<number;pix++){
            //On sélectionne abcisse et ordonnée au hasard.
            int i = rand()%(p.height);
            int j = rand()%(p.width);
            //Le pixel du dessus existe-t-il ?i>0
            byte valeur_courante = read_component_bw(melted,i,j);//(on n'écrira pas valeur_actuelle)
            if(i>0&&read_component_bw(melted,i-1,j)<valeur_courante){
                write_pixel_bw(melted,i,j,read_component_bw(melted,i-1,j));
            }
        }
    }else if(p.chan_num == RGB_PIXEL_SIZE){
        for(int pix = 0;pix<number;pix++){
            //On sélectionne abcisse et ordonnée au hasard.
            int i = rand()%(p.height);
            int j = rand()%(p.width);
            //Le pixel du dessus existe-t-il ?i>0
            byte curr_red = 0.299*read_component_rgb(melted,i,j,RED);
            byte curr_green = 0.587*read_component_rgb(melted,i,j,GREEN);
            byte curr_blue = 0.114*read_component_rgb(melted,i,j,BLUE);
            byte curr_value = curr_red+curr_green+curr_blue;
            //Pour comparer les luminosités respectives, on utilise la formule de conversion RGB->BW donnée par l'énoncé:
            if(i>0){
                byte above_red = read_component_rgb(melted,i-1,j,RED);
                byte above_green = read_component_rgb(melted,i-1,j,GREEN);
                byte above_blue = read_component_rgb(melted,i-1,j,BLUE);
                byte above_value = 0.299*(double)above_red+0.587*(double)above_green+0.144*(double)above_blue;

                if(above_value<curr_value){
                    //write_component_rgb(melted,i,j,RED,above_red);
                    //write_component_rgb(melted,i,j,GREEN,above_green);
                    //write_component_rgb(melted,i,j,BLUE,above_blue);
                    write_pixel_rgb(melted,i,j,above_red,above_green,above_blue);
                }
            }
        }
    }
    return melted;
}
//2ème version, plus maligne: on convertit en image niveau de gris puis on effectue un appel récursif.
//Il faut cependant une fonction auxiliaire pour savoir quel pointeur libérer, sinon on crée une fuite mémoire.
/*A faire...*/


/*Look-up tables:*/

/*
    QUESTION/TODO:
    Pourrait-on mettre cette définition/précision de structure
    dans lut.c sans avoir une erreur de type "incomplete type" ? Et si oui, comment ? Poser la question à M. Burel ?
    + faire une comparaison avec la situation du TP9 que j'ai fini par faire marcher.
*/
struct lut_s{
    int n;
    byte *array;
};
typedef struct lut_s* lut;

picture inverse_picture(picture p){
    picture res = copy_picture(p);
    if(is_empty_picture(p)){
        return res;
    }
    /*On crée la LUT d'inversion*/
    lut invert_lut = create_lut(MAX_BYTE+1);

    for(int c=0;c<MAX_BYTE;c++){
        invert_lut->array[c] = 255-c;
    }
    apply_lut(res,invert_lut);
    clean_lut(&invert_lut);
    return res;
}
picture normalize_dynamic_picture(picture p){
    picture res = copy_picture(p);
    if(is_empty_picture(p)){
        return res;
    }
    /*On crée la LUT de normalisation*/
    /*Première étape: trouver le min et le max.*/
    int max = p.data[0];
    int min = p.data[0];
    for(int k=0;k<(int)p.chan_num*p.width*p.height;k++){
        if(p.data[k]>max){
            max = p.data[k];
        }
        if(p.data[k]<min){
            min = p.data[k];
        }
    }

    lut normalize_lut = create_lut(MAX_BYTE+1);

    for(int c=min;c<=max;c++){

        double val = 0;
        if(max!=min){
            double range = (double)max-(double)min;
            val=255.0*(((double)c- (double)min)/range);
        }
        else
            val = 0.0;
        val = val<255.0?val:255.0;
        normalize_lut->array[c] = (byte)val;
    }
    apply_lut(res,normalize_lut);
    clean_lut(&normalize_lut);
    return res;
}

picture set_levels_picture(picture p, byte nb_levels){
    picture res = copy_picture(p);
    if(is_empty_picture(p)){
        return res;
    }
    /*On crée la LUT de réduction*/
    lut set_lut = create_lut(nb_levels);

    for(int c=0;c<nb_levels;c++){
        set_lut->array[c] = c; //Identité.
    }
    apply_lut(res,set_lut);
    clean_lut(&set_lut);
    return res;
}
/*Différence absolue*/
picture distance_picture(picture p1, picture p2){
    if(is_empty_picture(p1)&&is_empty_picture(p2)){
        return p1;
    }
    /*On vérifie que les tailles et les types (couleur ou niveaux de gris) sont les mêmes*/
    assert(p1.width==p2.width);
    assert(p1.height==p2.height);
    assert(p1.chan_num==p2.chan_num);
    picture res = create_picture(p1.width,p2.height,p1.chan_num);
    for(int k=0;k<(int)res.chan_num*res.width*res.height;k++){
        signed int diff = p1.data[k]-p2.data[k]; //même problème que d'habitude avec le type (byte)
        diff = diff>0 ?diff:-diff; //même effet que diff = abs(diff) sans la fonction abs.
        res.data[k] = (byte)diff;
    }
    return res;
}
/*Produit*/
picture mult_picture(picture p1, picture p2){
    if(is_empty_picture(p1)&&is_empty_picture(p2)){
        return p1;
    }
    /*On vérifie que les tailles et les types (couleur ou niveaux de gris) sont les mêmes*/
    assert(p1.width==p2.width);
    assert(p1.height==p2.height);
    assert(p1.chan_num==p2.chan_num);
    picture res = create_picture(p1.width,p2.height,p1.chan_num);
    for(int k=0;k<(int)res.chan_num*res.width*res.height;k++){
        
        double value =  p1.data[k]*p2.data[k];
        //value = value<MAX_BYTE?value:MAX_BYTE;
        res.data[k] = (byte)value;
    }
    return res;
}

/*Mélange*/
picture mix_picture(picture p1, picture p2, picture p3){
    if(is_empty_picture(p1)&&is_empty_picture(p2)&&is_empty_picture(p3)){
        return p1;
    }
    assert(same_dimensions(p1,p2)&&same_dimensions(p2,p3));
    picture res = create_picture(p1.width,p1.height,p1.chan_num);

    for(int k=0;k<(int)res.chan_num*res.width*res.height;k++){
        double alpha = p3.data[k]/255.0;
        res.data[k] = (1.0-alpha)*(double)p1.data[k]+alpha*(double)p2.data[k];
    }
    return res;
}