#include <assert.h>
#include "pixels.h"

void write_pixel_rgb(picture p, int i, int j, byte red, byte green, byte blue){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    int index =  ((i*p.width)+j)*3;
    p.data[index] = red;
    p.data[index+1] = green;
    p.data[index+2] = blue;
}
void write_pixel_bw(picture p, int i, int j,byte value){
    assert(0<=i&&i<p.height);
    assert(0<=j&&j<p.width);
    p.data[(i*p.width)+j] = value;
}