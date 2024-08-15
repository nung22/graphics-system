#ifndef POINT_H

#define POINT_H

#include <stdio.h>
#include <math.h>
#include "color.h"

// Structure to represent a point
typedef struct
{
  double val[4]; // four element vector of doubles
} Point;

/* Function prototypes for point operations */ 
void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
float point_distance(Point *p1, Point *p2);
void point_add(Point *p1, Point *p2, Point *result);
void point_scale(Point *p, float scale, Point *result);
void point_normalize(Point *p);
void point_copy(Point *to, Point *from);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);

#endif