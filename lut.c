#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lut.h"
#include "pixels.h"
#include "safe_malloc.h"
#include "pictures.h"

struct lut_s{
    int n;
    byte *array;
};
typedef struct lut_s* lut;

/*

*/
lut create_lut(unsigned int n){
    assert(n>0);
    lut res = malloc(sizeof(struct lut_s));

    res->n = n;
    res->array = myalloc(n*sizeof(byte));//sizeof(byte)==1 mais on évite l'utilisation de constantes non nommées.


    return res;
}
void clean_lut(lut *table){
    assert(table!=NULL);
    (*table)->n=0;
    free((*table)->array);
    free(*table); 
}

void apply_lut(picture p, lut table){
    if(is_empty_picture(p)){
        return;
    }
    double factor = (double)(table->n-1)/255.0;/*Ajouté pour les petites LUTs*/
    assert(abs_double(factor)>EPSILON);
    if(p.chan_num == RGB_PIXEL_SIZE){
        for(int i=0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte red_component = read_component_rgb(p,i,j,RED);
                byte green_component = read_component_rgb(p,i,j,GREEN);
                byte blue_component = read_component_rgb(p,i,j,BLUE);
                /*Lignes ajoutées pour gérer les LUTs de taille <256*/
                
                int new_red_index = (double)red_component*factor;
                int new_green_index = (double)green_component*factor;
                int new_blue_index = (double)blue_component*factor;

                byte lut_red = table->array[new_red_index]/factor;/*lut(new_red_index), image par la "fonction" lut implémentée par un tableau.*/
                byte lut_green = table->array[new_green_index]/factor;/*lut(new_green_index)*/
                byte lut_blue = table->array[new_blue_index]/factor;/*lut(new_blue_index)*/

                write_pixel_rgb(p,i,j,lut_red,lut_green,lut_blue);
            }
        }
    }
    else if(p.chan_num == BW_PIXEL_SIZE){
        for(int i=0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte value = read_component_bw(p,i,j);
                int new_index = (double)value*factor;
                write_pixel_bw(p,i,j,table->array[new_index]/factor);
            }
        }
    }
    else{
        fprintf(stderr,"chan_num error n apply_lut\n");
    }
}
