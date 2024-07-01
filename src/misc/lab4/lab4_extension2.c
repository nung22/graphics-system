/*
  Assignment 4, Extension 2

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

/*
  Program to test polygon functionality
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  Color Teal;
  Point pt[100];
  int i;
  
  srand(42);

  color_set(&Teal, 0.0, 0.5, 0.5 );

  src = image_create(rows, cols);

  // something more interesting
  for(i=0;i<50;i++) {
    float dr = rand() % 20;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(70 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(70 + dr));
  }
  p = polygon_createp(50, pt);

  printf("drawing a polygon\n");
  polygon_drawFill(p, src, Teal);

  printf("writing output\n");
  image_write(src, "scanline-fill.ppm");

  image_free(src);

  return(0);
}
