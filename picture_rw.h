#ifndef _PICTURE_RW_
#define _PICTURE_RW_
#include "pictures.h"

void reset_picture_to_zero(picture *p);

picture read_picture(const char *filename);
int write_picture(picture p, char * filename);

#endif /*_PICTURE_RW */