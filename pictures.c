#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "safe_malloc.h"
//#include "pictures.h"
#include "pixels.h"
#include "lut.h"
#include "picture_rw.h"

/*
TODO :
- implémenter: ignorer les commentaires
- sanitize input...
- prendre en compte la valeur maximale de la troisième ligne (en "bonus").
*/

double min_double(double d1, double d2){
    return d1<d2?d1:d2;
}
int min_int(int a, int b){
    return a<b?a:b;
}
double abs_double(double x){
    return x>0?x:-x;
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
    p->data = NULL;
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
    for(int k=0;k<(int)res.width*(int)res.height*(int)res.chan_num;k++){
        double value = (double)res.data[k]*factor;
        value = (value <(double)MAX_BYTE)?value:(double)MAX_BYTE; 
        res.data[k] = (byte)value;
    }
    return res;
}

picture melt_picture(picture p, int number){
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
                    write_pixel_rgb(melted,i,j,above_red,above_green,above_blue);
                }
            }
        }
    }
    return melted;
}

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
/*typedef struct lut_s* lut;*/

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
            /*On évite les divisions par 0.*/
            double range = (double)max-(double)min;
            val = 255.0*(((double)c- (double)min)/range);
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

double d_from_b(byte b){
    return (double)b;
}
picture mult_picture(picture p1, picture p2){
    if(is_empty_picture(p1)&&is_empty_picture(p2)){
        return p1;
    }
   
    if(p1.chan_num==BW_PIXEL_SIZE&&p2.chan_num==RGB_PIXEL_SIZE){
        return mult_picture(p2,p1);
    }
    if(p1.width!=p2.width||p1.height!=p2.height){
        /*Cf énoncé: on pourra redimensionner...*/
        picture tmp = resample_picture_bilinear(p2,p1.width,p1.height);
        clean_picture(&p2);
        p2 = tmp; 
    }
    picture res = create_picture(p1.width,p1.height,p1.chan_num);

    /*Justification de l'utilisation des doubles (à mettre dans le rapport):
    on a besoin d'un type plus grand que "char/byte" pour saturer au lieu de cycler. Int ne suffit pas car on divise par 255.0
    pour avoir un nombre entre 0 et 1. 
    */
   /*Rmq: encore un cas de disjonction de cas indépendante de i,j à commenter dans le rapport.*/
    for(int i=0;i<p1.height;++i){
        for(int j=0;j<p1.width;++j){
            if(p1.chan_num == BW_PIXEL_SIZE && p2.chan_num == BW_PIXEL_SIZE){
                double new = (d_from_b(read_component_bw(p1,i,j))/255.0)*d_from_b(read_component_bw(p2,i,j));
                new = min_double(new,255.0);
                byte value = (byte)new;
                write_pixel_bw(res,i,j,value);
            }
            else if(p1.chan_num == RGB_PIXEL_SIZE &&p2.chan_num == BW_PIXEL_SIZE){
                double bw_val = d_from_b(read_component_bw(p2,i,j))/255.0;

                double new_red_d = d_from_b(read_component_rgb(p1,i,j,RED))*bw_val;
                double new_green_d = d_from_b(read_component_rgb(p1,i,j,GREEN))*bw_val;
                double new_blue_d = d_from_b(read_component_rgb(p1,i,j,BLUE))*bw_val;

                new_red_d = min_double(new_red_d,255.0);
                new_green_d = min_double(new_green_d,255.0);
                new_blue_d = min_double(new_blue_d,255.0);

                byte red_b = (byte)new_red_d;
                byte green_b = (byte)new_green_d;
                byte blue_b = (byte)new_blue_d;

                write_pixel_rgb(res,i,j,red_b,green_b,blue_b);
            }
            else{
                /*Cf début de la fonction pour l'élimination du cas symétrique.*/
                assert(p1.chan_num==RGB_PIXEL_SIZE&&p2.chan_num==RGB_PIXEL_SIZE);
                double new_red = d_from_b(read_component_rgb(p1,i,j,RED))*d_from_b(read_component_rgb(p2,i,j,RED));
                double new_green = d_from_b(read_component_rgb(p1,i,j,GREEN))*d_from_b(read_component_rgb(p2,i,j,GREEN));
                double new_blue = d_from_b(read_component_rgb(p1,i,j,BLUE))*d_from_b(read_component_rgb(p2,i,j,BLUE));

                write_pixel_rgb(res,i,j,new_red,new_green,new_blue);
            }
        }
    }
    return res;
}

/*Mélange*/

void mix_reformat(picture p1,picture *q1){
    if(p1.chan_num == BW_PIXEL_SIZE){
        *q1 = convert_to_color_picture(p1);
    }else{
        *q1 = copy_picture(p1);
    }
}
picture mix_picture(picture p1, picture p2, picture p3){

    picture q1;
    picture q2;
    picture q3;

    bool gray = (p1.chan_num == p2.chan_num && p2.chan_num == p3.chan_num && p3.chan_num == BW_PIXEL_SIZE);

    /*
        On fait en sorte que q1,q2,q3 soient des images RGB représentant les mêmes données que p1,p2,p3 qui peuvent
        être en niveaux de gris.
    */
    mix_reformat(p1,&q1);
    mix_reformat(p2,&q2);
    mix_reformat(p3,&q3);
   
    assert(q1.chan_num == q2.chan_num && q2.chan_num == q3.chan_num && q3.chan_num == RGB_PIXEL_SIZE); 
   
    if(q1.width!=q3.width || q1.height!=q3.height){
        /*Cf énoncé: on pourra redimensionner...*/
        picture tmp = resample_picture_bilinear(q2,q1.width,q1.height);
        clean_picture(&q2);
        q2 = tmp; 
    }
    picture res = create_picture(q1.width,q1.height,RGB_PIXEL_SIZE);

    //On ne gère ici que le cas (RGB,RGB,RGB).
    for(int i=0;i<p1.height;++i){
        for(int j=0;j<p1.width;++j){
            for(int c=RED;c<=BLUE;++c){

                double alpha = d_from_b(read_component_rgb(q3,i,j,c))/255.0;
                assert(0<=alpha&&alpha<=255);
                double bary = (1.0-alpha)*d_from_b(read_component_rgb(q1,i,j,c));

                bary += alpha*d_from_b(read_component_rgb(q2,i,j,c));
                

                /*Un peu inutile: un barycentre entre 2 valeurs comprises entre 0 et 255 l'est aussi.*/
                //bary = min_double(bary,255.0);

                byte val = (byte)bary;
                write_component_rgb(res,i,j,c,val);
            }        
        }
    }
    /*Si (et seulement si ?)les trois images d'entrée étaient en niveaux de gris, la conversion en RGB était artificielle 
    et on aurait pu s'en passer (avec plus de code cependant). On reconvertit donc le résultat en image niveaux de gris.
    */
    clean_picture(&q1);
    clean_picture(&q2);
    clean_picture(&q3);
    if(gray){
        picture res_gray = convert_to_gray_picture(res);
        clean_picture(&res);
        return res_gray;
    }
    
    return res;
}
/*Fonction d'aide: utilisée deux fois
Modifie par référence les ratios de redimensionnement horizontal et vertical
Affiche un avertissement lorsque l'on déforme l'image.
*/
void check_resamplable(picture image, unsigned int width, unsigned int height,double *rx,double *ry){
    assert(width>0);
    assert(height>0);
    assert(!is_empty_picture(image));

    *rx = (double)width/(double)image.width;
    *ry = (double)height/(double)image.height;
    assert(*rx>EPSILON);
    assert(*ry>EPSILON);

    double diff = abs_double(*rx-*ry);

    if(diff>EPSILON){
        printf("Warning: the desired aspect ratio differs from that of the original image.\n");
    }
}
/*Rééchantillonnage avec la politique du plus proche voisin.*/
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height){
    
    picture res = create_picture(width,height,image.chan_num);

    double ratio_x;
    double ratio_y;

    check_resamplable(image,width,height,&ratio_x,&ratio_y);

    /*
    Remarque: la condition dans le "if" est indépendante de i et de j. On pourrait donc la sortir de la double boucle,
    ce que j'ai fait dans une version précédente (cf git log) (mentionner dans le rapport)

    */
    for(int i=0;i<(int)height;++i){
        for(int j=0;j<(int)width;++j){

            int old_i = (int)((double)i/ratio_y);
            int old_j = (int)((double)j/ratio_x);

            if(image.chan_num==RGB_PIXEL_SIZE){
                byte red = read_component_rgb(image,old_i,old_j,RED);
                byte green = read_component_rgb(image,old_i,old_j,GREEN);
                byte blue = read_component_rgb(image,old_i,old_j,BLUE);
                write_pixel_rgb(res,i,j,red,green,blue);
            }
            else{
                byte value = read_component_bw(image,old_i,old_j);
                write_pixel_bw(res,i,j,value);
            }
        }
    }
    return res;
}
/*On gère d'abord le cas des images en niveaux de gris. En cas d'image couleur, on la sépare en composantes avec split_picture
et on interpole les composantes séparément, puis on renvoie la fusion des résultats.
*/
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height){
    picture res; 

    double ratio_x;
    double ratio_y;

    check_resamplable(image,width,height,&ratio_x,&ratio_y);

    if(image.chan_num==RGB_PIXEL_SIZE){
        picture *split = split_picture(image);
        for(int k=0;k<3;++k){
            picture tmp = resample_picture_bilinear(split[k],width,height);
            clean_picture(&split[k]);
            split[k] = tmp;
        }
        
        res = merge_picture(split[0],split[1],split[2]);
        clean_picture(&split[0]);
        clean_picture(&split[1]);
        clean_picture(&split[2]);
        free(split);
        return res;
    }
    res = create_picture(width,height,image.chan_num);
    for(int i=0;i<(int)height;++i){
        for(int j=0;j<(int)width;++j){

            double y = (double)i/ratio_y;
            double x = (double)j/ratio_x;
            int old_i = (int)y;
            int old_j = (int)x;

            int x1 = old_j;
            int x2 = min_int(old_j+1,image.width-1);

            int y1 = min_int(old_i+1,image.height-1);
            int y2 = old_i;
            
            
            byte top_left = read_component_bw(image,y2,x1);
            byte top_right = read_component_bw(image,y2,x2);
            byte bottom_right = read_component_bw(image,y1,x2);
            byte bottom_left = read_component_bw(image,y1,x1);

            double alpha = 1.0;
            if(abs(x2-x1)>0){
                alpha = (x2-x)/(x2-x1);
            }
            
            double beta = 1.0;
            if(abs(y2-y1)>0){
                beta = (y2-y)/(y2-y1);
            }

            assert(-EPSILON<beta&&beta<1+EPSILON);
            assert(-EPSILON<alpha&&alpha<1+EPSILON);
            double val_1 = alpha*(double)bottom_left+(1.0-alpha)*(double)bottom_right;
            double val_2 = alpha*(double)top_left+(1.0-alpha)*(double)top_right;
            
            double val = min_double(beta*val_1+(1.0-beta)*val_2,255.0);
            val = val>0?val:-val; //max_double pas implémenté.
            byte value = (byte)val;
            write_pixel_bw(res,i,j,value);
        }
             
    }
    return res;
}

picture brighten_picture_lut(picture p, double factor){
    lut brighten_lut = create_lut(MAX_BYTE+1);
    picture res = copy_picture(p);
    for(int c=0;c<MAX_BYTE;++c){
        double val = min_double((double)c*factor,255.0);
        brighten_lut->array[c] = (byte)val;
    }
    /*Modification par effet*/
    apply_lut(res,brighten_lut);
    clean_lut(&brighten_lut);
    return res;
}
/*
Définir une fonction d'ordre supérieur qui applique une fonction prenant des images en noir et blanc 
aux composantes RGB d'une image couleur passée en argument, puis renvoie la fusion des trois résultats.

Utiliser cette fonction pour améliorer / factoriser resample et melt_picture ?
*/