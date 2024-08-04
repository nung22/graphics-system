#ifndef COLOR_H

#define COLOR_H

#include "image.h"
#include <stdio.h>

// Structure to represent a color
typedef struct
{
  float c[3];
} Color;

/* Function prototypes for Color operations */
void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
int color_compare(Color c1, Color c2);
void image_setColor(Image *src, int r, int c, Color val);
Color image_getColor(Image *src, int r, int c);
void color_print(Color *c, FILE *fp);

#endif // COLOR_H