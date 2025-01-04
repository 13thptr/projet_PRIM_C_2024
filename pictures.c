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

/*-------------------------------------------------------Fonctions utilitaires------------------------------------------*/

/**
 * Minimum entre deux entiers.
 * @param [in] n1 int
 * @param [in] n2 int 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: min(n1,n2)
 */
int min_int(int n1, int n2){
    return n1<n2?n1:n2;
}
/**
 * Maximum entre deux entiers.
 * @param [in] n1 int
 * @param [in] n2 int 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: max(n1,n2)
 */
int max_int(int n1, int n2){
    return n1>n2?n1:n2;
}
/**
 * Minimum entre deux doubles.
 * @param [in] d1 double 
 * @param [in] d2 double 
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: min(d1,d2)
 */
double min_double(double d1, double d2){
    return d1<d2?d1:d2;
}
/**
 * Valeur absolue d'un double
 * @param [in] x double
 * @requires: rien
 * @assigns: rien
 * @ensures: cf. return
 * @return: |x|
 */
double abs_double(double x){
    return x>0?x:-x;
}


/*-----------------------------Fonctions principales----------------------------------------------------------------------------------*/


/*-----------------------------------------------Création, copie et nettoyage---------------------------------------------------------*/
/**
 * Création d'une image
 * @param [in] width la largeur de l'image à créer
 * @param [in] height la hauteur de l'image à créer
 * @param [in] channels le nombre de canaux
 * 
 * @requires: channels vaut 1 ou 3. 
 * @assigns: le tas est modifié avec un appel à malloc() (cf. safe_malloc.c)
 * 
 * @ensures: L'image renvoyée est bien initialisée. Plantage si l'une des préconditions n'est pas respectée.
 * 
 * @return nouvelle image créée
 */
picture create_picture(unsigned int width, unsigned int height, unsigned int channels){
    assert(channels == RGB_PIXEL_SIZE||channels == GRAY_PIXEL_SIZE);
    picture res;
    res.width = width;
    res.height = height;
    res.chan_num = channels;
    res.data = myalloc(width*height*channels);
    return res;
}
/**
 * Nettoyage d'une image
 * @param [in] p pointeur vers une image dont le champ data est bien initialisé.
 * 
 * @requires: p n'est pas le pointeur nul.
 * @assigns: le tas est modifié avec un appel à free()
 * 
 * @ensures: La mémoire occupée par le champ data est bien libérée. Plantage si l'une des préconditions n'est pas respectée.
 * 
 * @return rien
 */
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
/**
 * Copie d'une image
 * @param [in] p 
 * 
 * @requires: p non vide. (on n'a pas encore is_empty_picture)
 * @assigns: le tas est modifié avec un appel à malloc() via create_picture.
 * 
 * @ensures: Plantage si l'une des préconditions n'est pas respectée. Sinon, une nouvelle image est initialisée en mémoire
 * contenant les mêmes informations d'image que le paramètre.
 * 
 * @return La copie nouvellement créée
*/

picture copy_picture(picture p){
    assert(p.width>0&&p.height>0);
    assert(p.data!=NULL);
    assert(p.chan_num == GRAY_PIXEL_SIZE || p.chan_num == RGB_PIXEL_SIZE);

    picture res = create_picture(p.width,p.height,p.chan_num);
    for(int k = 0; k<p.width*p.height*(int)p.chan_num;k++){
        res.data[k] = p.data[k];
    }
   
    return res;
}
/*------------------------------------------------------------Obtention d’informations sur une image-----------------------------------*/

/**
 * is_empty_picture
 * @param [in] p 
 * 
 * @requires: rien
 * @assigns: rien
 * 
 * @ensures: cf. return
 * 
 * @return booléen: vrai si aucune des valeurs ne vaut 0, faux sinon.
*/
bool is_empty_picture(picture p){
    return !(p.width&&p.height&&p.chan_num);
}
/**
 * is_gray_picture
 * @param [in] p 
 * 
 * @requires: rien
 * @assigns: rien
 * 
 * @ensures: cf. return
 * 
 * @return booléen: vrai si l'image est en niveaux de gris, faux sinon
*/
bool is_gray_picture(picture p){
    return p.chan_num == GRAY_PIXEL_SIZE;
}
/**
 * is_color_picture
 * @param [in] p 
 * @assigns: rien
 * 
 * @ensures: cf. return
 * 
 * @return booléen: vrai si l'image est en couleurs, faux sinon
*/
bool is_color_picture(picture p){
    return p.chan_num == RGB_PIXEL_SIZE;
}
/**
 * info_picture
 * @param [in] p 
 * @assigns: rien
 * 
 * @ensures: rien
 * 
 * @return rien
*/
void info_picture(picture p){
    printf("%d x% d x %d\n",p.width,p.height,p.chan_num);
}
/**
 * same_dimensions
 * @param [in] p1
 * @param [in] p2 
 * @assigns: rien
 * 
 * @ensures: rien
 * 
 * @return vrai ssi les deux images ont les mêmes dimensions
*/
bool same_dimensions(picture p1, picture p2){
    return p1.chan_num==p2.chan_num&&p1.width==p2.width&&p1.height==p2.height;
}

/*-----------------------------------------------------------FONCTIONS PRINCIPALES (MANIPULATION)------------------------------------*/

/**
 * convert_to_color_picture
 * @param [in] p 
 * @requires: p image valide, non vide.
 * @assigns: modifie le tas en créant ou bien une copie de l'image en cours, ou bien une nouvelle image (nouvelle allocation 
 * dans les deux cas)
 * 
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal.
 * 
 * @return image en couleur, copie de l'image d'entrée ou résultat de la duplication des canaux.
*/
picture convert_to_color_picture(picture p){
    assert(!is_empty_picture(p));

    if(p.chan_num == RGB_PIXEL_SIZE){
        return copy_picture(p);
    }
    assert(p.chan_num == GRAY_PIXEL_SIZE);
    picture res = create_picture(p.width,p.height,RGB_PIXEL_SIZE);
    
    for(int i=0;i<res.height;i++){
        for(int j=0;j<res.width;j++){
            byte value = read_component_bw(p,i,j);
            write_pixel_rgb(res,i,j,value,value,value);
        }
    }
    return res;
}
/**
 * convert_to_gray_picture
 * @param [in] p 
 * @requires: p image valide, non vide.
 * @assigns: modifie le tas en créant ou bien une copie de l'image en cours, ou bien une nouvelle image (nouvelle allocation 
 * dans les deux cas)
 * 
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal.
 * 
 * @return image en niveaux de gris, copie de l'image d'entrée ou résultat de la moyenne pondérée des 3 canaux pixel par pixel.
*/
picture convert_to_gray_picture(picture p){
    assert(!is_empty_picture(p));

    if(is_gray_picture(p)){
        return copy_picture(p); //Enoncé: on se contentera d'en faire une copie
    }
    assert(is_color_picture(p));
    picture res = create_picture(p.width,p.height,GRAY_PIXEL_SIZE);

    for(int i=0;i<res.height;i++){
        for(int j=0;j<res.width;j++){
            double red = (double)0.299*read_component_rgb(p,i,j,RED);
            double green = (double)0.587*read_component_rgb(p,i,j,GREEN);
            double blue = (double)0.114*read_component_rgb(p,i,j,BLUE);
            double sum = min_double(red+green+blue,(double)MAX_BYTE);
            byte value = (byte)(sum);
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

/**
 * split_picture
 * @param [in] p 
 * @requires: p image valide;
 * @assigns: modifie le tas en créant:
 *  -1) Un pointeur de type picture* vers la première case d'un tableau de "picture"s de taille 1 ou 3 (selon 
 *  la valeur de p.chan_num).
 *  -2) Des tableaux "data" pour les composantes rouge, verte et bleue sous forme d'images en niveaux de gris.
 * dans les deux cas)
 * 
 * @ensures: Plantage si l'un e des préconditions n'est pas respectée; cf. @return pour le comportement normal.
 * 
 * @return pointeur de type picture* 
*/
picture *split_picture(picture p){
    if(is_empty_picture(p)){
        return NULL;
    }
    picture *arr = myalloc(sizeof(picture)*p.chan_num);
    for(int n=0;n<(int)p.chan_num;n++){
        arr[n] = create_picture(p.width,p.height,GRAY_PIXEL_SIZE);
       
        for(int i= 0;i<p.height;i++){
            for(int j=0;j<p.width;j++){
                byte component = (p.chan_num==GRAY_PIXEL_SIZE?read_component_bw(p,i,j):read_component_rgb(p,i,j,n));
                write_pixel_bw(arr[n],i,j,component); //ici il n'y a pas de disjonction, on écrit toujours dans une image en niveaux de gris.
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

/**
 * merge_picture
 * @param [in] red, green, blue
 * @requires: red,green,blue images 
 * @assigns: modifie le tas en créant:
 *  une picture "res" et donc un pointeur vers un tableau alloué dans le tas via create_picture pour res.data;
 * 
 * @ensures: Plantage si les trois images n'ont pas la même dimension, via width=0 ou height=0 et create_picture
 *  Renvoie la fusion des 3 images utilisées comme canaux sinon.
 * 
 * @return image nommée res(type picture)
*/
picture merge_picture(picture red, picture green, picture blue){
    assert(is_gray_picture(red));
    assert(is_gray_picture(green));
    assert(is_gray_picture(blue));
    int width = red.width * (red.width==green.width&&green.width == blue.width);

    int height = red.height *(red.height==green.height&&green.height==blue.height);
    
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
/**
 * brighten_picture
 * @param [in] p, factor
 * @requires: p image valide, factor>0.
 * @assigns: modifie le tas en créant:
 *  une copie via copy_picture, donc un nouvau champ data contenant un pointeur vers un tableau de bytes.
 *  
 *
 * @ensures: Plantage si les préconditions de copy_picture ne sont pas respectées
 * 
 * @return image nommée res(type picture)
*/
picture brighten_picture(picture p, double factor){
    picture res = copy_picture(p);
    assert(factor>0);
    for(int k=0;k<(int)res.width*(int)res.height*(int)res.chan_num;k++){
        double value = (double)res.data[k]*factor;
        value = min_double(value,MAX_BYTE); 
        res.data[k] = (byte)value;
    }
    return res;
}
/**
 * melt_picture
 * 
 * @param [in] p, number
 * 
 * @requires: p: image valide, number: nombre positifs de pixels à faire fondre.
 * @assigns: modifie le tas en créant une copie que l'on va modifier pour l'effet de fonte.
 * @ensures: Plantage si les préconditions de copy_picture ne sont pas respectées
 * 
 * @return image nommée melted (type picture)
*/
picture melt_picture(picture p, int number){
    /*Il faut distinguer deux cas: image en niveaux de gris ou couleur.*/
    assert(number>=0);
    picture melted = copy_picture(p);

    if(p.chan_num == GRAY_PIXEL_SIZE){
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

/**
 * inverse_picture
 * @param [in] p 
 * 
 * @requires: p image valide
 * @assigns: nouvelle copie de l'image p dans le tas (heap), modifiée par une lut via apply_lut.
 * @ensures: Plantage si les préconditions de copy_picture, create_lut, apply_lut ne sont pas respectées
 * 
 * @return image de type picture contenant un champ data avec les données d'image de p.data "inversées" (involution)
*/

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
/**
 * normalize_dynamic_picture
 * @param [in] p 
 * 
 * @requires: p image valide
 * @assigns: copie en mémoire de p effectuée avant modificaiton par lut. lut stockée temporairement dans le tas le temps 
 * d'effectuer le traitement.
 * @ensures: plantage si les préconditions de copy_picture ne sont pas respectées, sinon res est renvoyée et contient les données
 * normalisées.
 * 
 * @return res image normalisée de type picture.
*/
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
/**
 * set_levels_picture
 * @param [in] p, nb_levels
 * 
 * @requires: p picture valide
 * @assigns: modification du tas par copy_picture créant une nouvelle image qui sera renvoyée (res)
 * @ensures: plantage par copy_picture, create_lut ou apply_lut via assert en cas de non-respect d'une précondition, sinon cf. @return
 * 
 * @return res image ne contenant que nb_levels niveaux de couleur
*/
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
/**
 * 
 * @param [in] p1, @param[in] p2
 * 
 * @requires:p1,p2 images valides de type picture, de même tailles
 * @assigns: allocation d'une zone mémoire pour le champ data de la picture "res" crée dans le corps de la fonction
 * 
 * 
 * @ensures: plantage si p1,p2 ne vérifient pas les préconditions, cf. return sinon. p1 et p2 ne sont pas modifiées...
 * 
 * @return: image constituée de la distance "byte à byte" 
*/

/*Différence absolue*/
picture distance_picture(picture p1, picture p2){
    if(is_empty_picture(p1)&&is_empty_picture(p2)){
        return p1;
    }
    /*On vérifie que les tailles et les types (couleur ou niveaux de gris) sont les mêmes*/
    assert(same_dimensions(p1,p2));
    picture res = create_picture(p1.width,p1.height,p1.chan_num);
    for(int k=0;k<(int)res.chan_num*res.width*res.height;k++){
        signed int diff = (signed int)p1.data[k]-(signed int)p2.data[k]; //même problème que d'habitude avec le type (byte)
        diff = diff>0 ?diff:-diff; //même effet que diff = abs(diff) sans la fonction abs.
        res.data[k] = (byte)diff;
    }
    return res;
}
/*Produit*/
/*d_from_b
Fonction utilitaire pour convertir (caster) en évitant un warning "pédant" (pedantic) sans créer trop de variables.
    @param: b
    @requires: rien
    @assigns: rien
    @ensures: rien
    @return: cast de b en double.
*/
double d_from_b(byte b){
    return (double)b;
}
/**
 * mult_picture
 * @param [in] p1
 * @param [in] p2
 * 
 * @requires:p1,p2 images valides
 * @assigns: le tas est modifié par la création d'une picture res.
 *   
 * @ensures: res contient la multiplication "renormalisée" de p1 et p2 byte à byte
 * "renormalisée" dans le sens où l'on divise le produit des byte convertis en doubles par 255 pour obtenir une résultat
 * qui soit encore entre 0 et 255. On n'utilise pas renormalize_dynamic_picture car le but ici n'est pas d'optimiser la 
 * dynamique.
 * 
 * @return picture res contenant le "produit" décrit ci-dessus.
*/

picture mult_picture(picture p1, picture p2){
    if(is_empty_picture(p1)&&is_empty_picture(p2)){
        return p1;
    }
   
    if(p1.chan_num==GRAY_PIXEL_SIZE&&p2.chan_num==RGB_PIXEL_SIZE){
        /*
            Appel récursif: à l'appel suivant, la condition du if ne pourra pas s'évaluer à true grâce à l'interversion.
            On garantit un ordre en quelque sorte.
        */
        return mult_picture(p2,p1);
    }
    if(p1.width!=p2.width||p1.height!=p2.height){
        /*Cf énoncé: on pourra redimensionner...*/
        picture tmp = resample_picture_bilinear(p2,p1.width,p1.height);
        clean_picture(&p2);
        p2 = tmp; 
    }
    picture res = create_picture(p1.width,p1.height,p1.chan_num);

    /*
        Justification de l'utilisation des doubles (à mettre dans le rapport):
        on a besoin d'un type plus grand que "char/byte" pour saturer au lieu de cycler. Le type "int" ne suffit pas car on divise par 255.0
        pour avoir un nombre entre 0 et 1: un int prendrait la valuer 0. On pourrait sans doute éviter cela en ne divisant qu'après,
        mais alors le code serait plus éloigné de l'énoncé car pour être cohérent il faudrait faire de même dans mix_picture, 
        et donc changer la définition de alpha.
    */
   /*Remarque: on a ici un cas de disjonction de cas indépendante de i,j à commenter dans le rapport.*/
    for(int i=0;i<p1.height;++i){
        for(int j=0;j<p1.width;++j){
            if(p1.chan_num == GRAY_PIXEL_SIZE && p2.chan_num == GRAY_PIXEL_SIZE){
                double new = (d_from_b(read_component_bw(p1,i,j))/255.0)*d_from_b(read_component_bw(p2,i,j));
                new = min_double(new,255.0);
                byte value = (byte)new;
                write_pixel_bw(res,i,j,value);
            }
            else if(p1.chan_num == RGB_PIXEL_SIZE &&p2.chan_num == GRAY_PIXEL_SIZE){
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

/**
 * Fonction utilitaire mix_reformat
 * Intérêt: Permet de n'avoir à traiter que le cas (RGB,RGB,RGB) pour les arguments (p1,p2,p3) de mix_picture.
 * @param [in] p1 image à "transformer" éventuellement en image couleur
 * @param [in] q1 pointeur vers une image censée accueillir une copie ou une conversion en couleur de p1
 * @requires:p1,p2,p3 images valides de type pictures.
 * @assigns: modification du tas via convert_to_color picture ou copy_picture qui effectuent une allocation

 * @ensures: *q1 contient une copie de p1 si celle-ci était en couleur, une conversion en couleur sinon.
 * 
 * @return: rien (void)
*/
void mix_reformat(picture p1,picture *q1){
    if(p1.chan_num == GRAY_PIXEL_SIZE){
        *q1 = convert_to_color_picture(p1);
    }else{
        *q1 = copy_picture(p1);
    }
}
/**
 * 
 * @param [in] p1,p2,p3
 * 
 * @requires:p1,p2,p3 images valides de type pictures.
 * @assigns: modifications dans le tas
 *          1) "Temporaires" (propres au corps de fonction):
 *              - q1,q2,q3 sont modifiées par mix_reformat
 *                
 *          2) "Permanentes" (on passe la responsabilité de libérer la mémoire à une autre fonction, dans un wrapper de wrappers.c
 * 
 *          ou à la fonction main dans main.c):
 *              - l'image res allouée est renvoyée.
 * 
 * @ensures: res contient l'image "mélange" selon une politique de barycentre
 *           des deux premières images d'entrée, pondéré par  la troisième image.
 *           ou alors la fonction a planté car l'une des trois images d'entrée n'a pas vérifié une précondition.
 * 
 * @return cf. @ensures:
 * */

picture mix_picture(picture p1, picture p2, picture p3){

    picture q1;
    picture q2;
    picture q3;
    /*
        On doit gérer le cas où les trois images d'entrées sont en niveaux de gris: mix_reformat doit se charger
        de les transformer en image couleurs pour éviter d'écrire un traitement à part, mais alors pour des raisons de cohérence il 
        faudra retransformer le résultat en image niveaux de gris, car on aurait pu effectuer tout le traitement en niveaux de gris.
        Le résultat devrait être le même, j'ai fait ce choix pour éviter la duplication inutile de code, puisque
        dans la majorité des cas les trois images doivent être converties (éventuellement de façon idempotente) en images couleurs.
    */
    bool gray = (p1.chan_num == p2.chan_num && p2.chan_num == p3.chan_num && p3.chan_num == GRAY_PIXEL_SIZE);

    /*
        On fait en sorte que q1,q2,q3 soient des images RGB représentant les mêmes données que p1,p2,p3 qui peuvent
        être en niveaux de gris.
    */
    mix_reformat(p1,&q1);
    mix_reformat(p2,&q2);
    mix_reformat(p3,&q3);
   
    assert(q1.chan_num == q2.chan_num && q2.chan_num == q3.chan_num && q3.chan_num == RGB_PIXEL_SIZE); 
    printf("Check around line 720 of pictures.c for bizarre resizing condition.\n");
    if(q1.width!=q3.width || q1.height!=q3.height){
        /*Cf énoncé: on pourra redimensionner...*/
        printf("Pretty sure this should cause a double free / segfault if the flow was to actually reach this point...\n");
        printf("Input images for mix_picture are different sizes, resizing (bilinear interpolation)...\n");
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
                double bary = (1.0-alpha)*d_from_b(read_component_rgb(q1,i,j,c));
                bary += alpha*d_from_b(read_component_rgb(q2,i,j,c));
                byte val = (byte)bary;
                write_component_rgb(res,i,j,c,val);
            }        
        }
    }
    
    clean_picture(&q1);
    clean_picture(&q2);
    clean_picture(&q3);
    /*
        Si, et seulement si les trois images d'entrée étaient en niveaux de gris, la conversion en RGB était artificielle,
        et l'on aurait pu s'en passer (avec plus de code cependant). On reconvertit donc le résultat en image niveaux de gris.
    */
    if(gray){
        picture res_gray = convert_to_gray_picture(res);
        clean_picture(&res);
        return res_gray;
    }
    
    return res;
}
/*
    Fonction utilitaire: utilisée deux fois
    Modifie par référence les ratios de redimensionnement horizontal et vertical
    Affiche un avertissement lorsque l'on déforme l'image.

 * check_resamplable
 * @param: image, width, height, rx, ry
 * @requires: image structure valide de type picture.
 * @requires: width not 0
 * @requires: height not 0
 * @requires: rx,ry valid pointers
*/
void check_resamplable(picture image, unsigned int width, unsigned int height,double *rx,double *ry){
    assert(width>0);
    assert(height>0);
    assert(!is_empty_picture(image));
    assert(rx!=NULL&&ry!=NULL);
    *rx = (double)width/(double)image.width;
    *ry = (double)height/(double)image.height;
    assert(*rx>EPSILON);
    assert(*ry>EPSILON);

    double diff = abs_double(*rx-*ry);

    if(diff>EPSILON){
        printf("Warning: the desired aspect ratio differs from that of the original image.\n");
    }
}
/**
 * resample_picture_nearest
 * @param [in] image
 * @param [in] width
 * @param [in] height
 * @requires: image, width, heigth vérifient les conditions de check_resamplable
 * @assigns: le tas est modifié par la création de l'image renvoyée "res", à libérer dans une autre fonction.
 *   
 * @ensures: sous condition de redimensionnabilité, "res" contient "image" redimensionnée avec la politique du plus proche voisin.
 * 
 * @return: res l'image d'entrée, mais redimensionnée.
*/

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

/**
 * resample_picture_bilinear
 * @param [in] image 
 * @param [in] width
 * @param [in] height
 * @requires: image, width, heigth vérifient les conditions de check_resamplable
 * @assigns: le tas est modifié par la création de l'image renvoyée "res", à libérer dans une autre fonction.
 *   
 * @ensures: sous condition de redimensionnabilité, "res" contient "image" redimensionnée
 *  avec la politique d'interpolation bilinéaire.
 * 
 * @return: res l'image d'entrée, mais redimensionnée.

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
/**
 * brighten_picture_lut
 * @param [in] p 
 * @param [in] factor
 * 
 * @requires:p image valide, factor >=0 (peut être nul, mais c'est un double...).
 * @assigns: tas modifié par la création d'une copie qui sera renvoyée (res)
 *  
 * @ensures: res contient une image similaire à p, mais dont la "luminosité" a été changée (augmentée ou diminuée selon le facteur)
 * par application d'une LUT.
 * 
 * @return 
*/

picture brighten_picture_lut(picture p, double factor){
    assert(factor>=0);
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
/*Manipulation de noyaux*/


double **create_square_matrix(int n){
    assert(n>0);
    double **res = myalloc(n*sizeof(double*));
    for(int i=0;i<n;++i){
        res[i] = myalloc(n*sizeof(double));
        for(int j=0;j<n;++j){
            res[i][j]=0;
        }
    }
    return res;
}
void delete_square_matrix(double **matrix, int n){
    assert(matrix!=NULL);
    for(int i=0;i<n;++i){
        assert(matrix[i]!=NULL);
        free(matrix[i]);
    }
    free(matrix);
}
void set_square_matrix(double **matrix, int n, int i,int j, double coefficient){
    assert(0<=i&&i<n);
    assert(0<=j&&j<n);
    matrix[i][j] = coefficient;
}
double **copy_square_matrix(double **matrix, int n){
    /*printf("Matrix to be copied:\n");
    print_square_matrix(matrix,n);
    */
    double **res = create_square_matrix(n);
    printf("res initialisé\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            //printf("matrix[%d][%d]:%lf ;",i,j,matrix[i][j]);
            res[i][j] = matrix[i][j];
        }
    }
    return res;
}
void print_square_matrix(double **matrix, int n){
    printf("\nSquare matrix (convolution kernel):\n");
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            printf("%lf ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void apply_matrix_affine_transformation(double **matrix, int n, double factor, double offset){
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            matrix[i][j] = factor*matrix[i][j]+offset;
        }
    }
}

/*@requires p is greyscale picture*/
double get_convolved_value(double **matrix, int n, picture p, int i, int j){

    double sum = 0.0;/*Element neutre pour la somme*/
    for(int k=0;k<n;++k){
        int new_i = min_int(max_int(i-n/2+k,0),p.height-1);
        for(int l=0;l<n;++l){
            int new_j = min_int(max_int(j-n/2+l,0),p.height-1);
            double value = d_from_b(read_component_bw(p,new_i,new_j));
            sum += value*matrix[k][l];
        }
    }
    return sum;
}

/*On fait d'abord marcher la fonction pour des images en niveaux de gris.*/
picture apply_kernel_to_copy(const picture p, const kernel k){
    picture res = copy_picture(p);
    /*printf("Checking k.matrix...\n");
    print_square_matrix(k.matrix,k.n); OK
    */
    double **copy = copy_square_matrix(k.matrix,k.n);

    apply_matrix_affine_transformation(copy,k.n,k.factor,k.offset);

    assert(!is_empty_picture(p));//pas sûr de ça.
    assert(is_gray_picture(p));//à enlever

    for(int i=0;i<p.height;++i){
        for(int j=0;j<p.width;++j){
            double convolved = get_convolved_value(copy,k.n,p,i,j);
            //printf("convolved:%lf\n",convolved);
            assert(0<convolved&&convolved<(double)MAX_BYTE);
            byte value = (byte)convolved;
            write_pixel_bw(res,i,j,value);
        }
    }
    delete_square_matrix(copy,k.n);
    return res;
}

/*
    Définir une fonction d'ordre supérieur qui applique une fonction prenant des images en noir et blanc 
    aux composantes RGB d'une image couleur passée en argument, puis renvoie la fusion des trois résultats.

    Utiliser cette fonction pour améliorer / factoriser resample et melt_picture ?
*/

