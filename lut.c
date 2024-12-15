#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lut.h"
#include "pixels.h"

/*NB: la consigne spécifie la création d'un type ABSTRAIT. On le précisera donc dans lut.c*/

/*
NB: J'ai été obligé de copier la définition / précision du type lut_s dans pictures.c pour éviter une erreur de type incomplet,
peut-on éviter cela ?
*/
struct lut_s{
    int n;
    byte *array;
};
typedef struct lut_s* lut;


lut create_lut(unsigned int n){
    assert(n>0);
    lut res = malloc(sizeof(struct lut_s));
    //*res = malloc(sizeof(lut));
    res->n = n;

    //res->array = NULL;

    res->array = calloc(n,sizeof(byte));//sizeof(byte)==1 mais on évite l'utilisation de constantes non nommées.

    //todo: utiliser myalloc (et vérifier partout.)
    return res;
}
void clean_lut(lut *table){
    assert(table!=NULL);
    (*table)->n=0;
    free((*table)->array);
    free(*table); //pas sûr de ça.
    //free(table);
}

void apply_lut(picture p, lut *table){
    if(is_empty_picture(p)){
        return;
    }
    if(p.chan_num == RGB_PIXEL_SIZE){
        for(int i=0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte red_component = read_component_rgb(p,i,j,RED);
                byte green_component = read_component_rgb(p,i,j,GREEN);
                byte blue_component = read_component_rgb(p,i,j,BLUE);
                write_pixel_rgb(p,i,j,(*table)->array[red_component],(*table)->array[green_component],(*table)->array[blue_component]);
            }
        }
    }
    else if(p.chan_num == BW_PIXEL_SIZE){
        for(int i=0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte value = read_component_bw(p,i,j);
                write_pixel_bw(p,i,j,(*table)->array[value]);
            }
        }
    }
    else{
        fprintf(stderr,"chan_num error n apply_lut\n");
    }
}
