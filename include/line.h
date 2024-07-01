#ifndef LINE_H

#define LINE_H

#include "point.h"

// Structure to represent a line
typedef struct
{
  int zBuffer; // whether to use z-buffer; defaults to true
  Point a, b;  // starting point, ending point
} Line;

/* Function prototypes for Line operations */ 
void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);

#endif


