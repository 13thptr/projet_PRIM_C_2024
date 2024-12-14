#include <stdio.h>
#include <stdlib.h>
#include "pictures.h"
/*
    Un code abondamment commenté ; 
    la première partie des commentaires comportera systématiquement les lignes :

    @requires décrivant les préconditions :
     c’est-à-dire conditions sur les paramètres
      pour une bonne utilisation (pas de typage ici),
    
    @assigns listant les zones de mémoire modifiées,
    @ensures décrivant la propriété vraie à la sortie
    de la fonction lorsque les préconditions sont respectées,
    le cas échéant avec mention des comportements en cas de succès
    et en cas d’échec,
    En outre chaque boucle while doit contenir un commentaire
    précisant la raison de sa terminaison (le cas échéant).
    De même en cas d’appels récursifs.
    On pourra préciser des informations additionnelles
    si des techniques particulières méritent d’être mentionnées.

*/
int main(int argc, char* argv[]){
/* 
On vous fournit un module filename.[h/c] qui vous permettra de
Séparer un chemin vers un fichier image <dirname>/<name>.<ext> en ses composantes <dirname>, <name> et <ext>
Composer un chemin vers un fichier image à partir de ces composantes.
Vous pourrez ainsi extraire le nom du fichier image traité par votre programme
 et le customiser avec les noms des opérations que vous appliquerez dessus avant*/
    picture test;
    char* test_path;
    if(argc!=3){
        fprintf(stderr,"Usage:./prog <filename.[pgm|ppm]> <path_to_new_file.[pgm|ppm]\n");
        return EXIT_FAILURE;
    }
    test_path = argv[2];
    test = read_picture(argv[1]);
    write_picture(test,test_path);

    return EXIT_SUCCESS;
}