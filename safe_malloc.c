#include <stdlib.h>
#include <stdio.h>
#include "safe_malloc.h"


void *myalloc(size_t mem_size)
{
    void *mem_ptr = NULL;
    int i = 0;
    /*
        La boucle while ci-dessous termine nécessairement grâce au compteur i.
    */
    while(!mem_ptr && mem_size &&i<MAX_MALLOC_TRIES){
        mem_ptr = malloc(mem_size);
        i++;
    }
    if(i==MAX_MALLOC_TRIES){
        printf("Could not allocate memory!");
        abort();
    }
    return mem_ptr;
}