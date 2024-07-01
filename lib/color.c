// These functions provide methods for manipulating colors.
// Written by Nicholas Ung 2024-05-26

#include "color.h"
#include <stdio.h>

// Copies the Color data.
void color_copy(Color *to, Color *from) {
  to->c[0] = from->c[0];
  to->c[1] = from->c[1];
  to->c[2] = from->c[2];
}

// Sets the Color data;
void color_set(Color *to, float r, float g, float b) {
  to->c[0] = r;
  to->c[1] = g;
  to->c[2] = b;
}

// Compares the Color data.
int color_compare(Color c1, Color c2) {
  if (c1.c[0] == c2.c[0] && c1.c[1] == c2.c[1] && c1.c[2] == c2.c[2]) {
    return 1;
  } else {
    return 0;
  }
}

// Copies the Color data to the proper pixel.
void image_setColor(Image *src, int r, int c, Color val) {
  FPixel fp;
  
  fp.rgb[0] = val.c[0];
  fp.rgb[1] = val.c[1];
  fp.rgb[2] = val.c[2];
  image_setf(src, r, c, fp);
}

// Returns a Color structure built from the pixel values.
Color image_getColor(Image *src, int r, int c) {
  Color val;

  val.c[0] = image_getf(src, r, c).rgb[0];
  val.c[1] = image_getf(src, r, c).rgb[1];
  val.c[2] = image_getf(src, r, c).rgb[2];
  return val;
}

// Prints the Color data.
void color_print(Color *c, FILE *fp) {
  fprintf(fp, "(%f, %f, %f)\n", c->c[0], c->c[1], c->c[2]);
}