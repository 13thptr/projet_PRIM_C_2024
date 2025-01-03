#ifndef _PICTURE_RW_
#define _PICTURE_RW_
#include "pictures.h"
#include <stdio.h> //Pour "FILE"
bool fgets_encapsulator(char *buf, FILE *f, int line_counter);

bool read_correctly_block(bool *read_correctly, char *buffer, FILE *to_be_read, picture *res, int *lc_p);

void reset_picture_to_zero(picture *p);

picture read_picture(const char *filename);
int write_picture(picture p, char * filename);

#endif /*_PICTURE_RW */