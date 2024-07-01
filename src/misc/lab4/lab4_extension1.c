/*
  Assignment 4, Extension 1

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
  Image *src1, *src2;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  Color Purple;
  Color Pink;
  Color Magenta;
  Color White;
  Point pt[100];
  int i;
  
  srand(36);

  color_set(&Purple, 0.5, 0.0, 0.5 );
  color_set(&Pink, 1.0, 0.0, 1.0 );
  color_set(&Magenta, 1.0, 0.0, 0.5 );
  color_set(&White, 1.0, 1.0, 1.0 );

  src1 = image_create(rows, cols);
  src2 = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 50, 30);
  point_set2D(&(pt[2]), 50, 50);
  point_set2D(&(pt[3]), 30, 50);

  p = polygon_createp(4, pt);

  printf("drawing a square\n");
  polygon_drawFill(p, src1, White);
  polygon_drawFillAA(p, src2, White);

  // Draw outer polygon
  for(i=0;i<50;i++) {
    float dr = rand() % 20;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(70 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(70 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing polygon 1\n");
  polygon_drawFill(p, src1, Pink);
  printf("drawing polygon 1 (anti-aliasing)\n");
  polygon_drawFillAA(p, src2, Pink);


  // Draw middle polygon
  for(i=0;i<50;i++) {
    float dr = rand() % 15;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(50 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(50 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing polygon 2\n");
  polygon_drawFill(p, src1, Magenta);
  printf("drawing polygon 2 (anti-aliasing)\n");
  polygon_drawFillAA(p, src2, Magenta);


  // Draw inner polygon
  for(i=0;i<50;i++) {
    float dr = rand() % 10;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(30 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(30 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing polygon 3\n");
  polygon_drawFill(p, src1, Purple);
  printf("drawing polygon 3 (anti-aliasing)\n");
  polygon_drawFillAA(p, src2, Purple);


  printf("writing output\n");
  image_write(src1, "without-anti-aliasing.ppm");
  image_write(src2, "with-anti-aliasing.ppm");

  image_free(src1);
  image_free(src2);

  return(0);
}
