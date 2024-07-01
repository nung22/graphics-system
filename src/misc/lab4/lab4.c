/*
  Assignment 4, required image: Creative

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
  Image *src, *srcAA;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  Color DarkBrown;
  Color DarkYellow;
  Color LightYellow;
  Color DarkGreen;
  Color Green;
  Color SkyBlue;
  Point pt[100];
  int i;
  
  srand(32);

  color_set(&DarkBrown, 0.4, 0.2, 0.1 );
  color_set(&DarkYellow, 0.95, 0.7, 0.3 );
  color_set(&LightYellow, 1.0, 1.0, 0.9 );
  color_set(&DarkGreen, 0.0, 0.5, 0.0 );
  color_set(&Green, 0.0, 1.0, 0.0 );
  color_set(&SkyBlue, 0.0, 0.75, 1.0 );

  src = image_create(rows, cols);
  srcAA = image_create(225, cols);
  image_fillrgb( srcAA, SkyBlue.c[0], SkyBlue.c[1], SkyBlue.c[2]);


  // make a stem
  point_set2D(&(pt[0]), 195, 150);
  point_set2D(&(pt[1]), 205, 150);
  point_set2D(&(pt[2]), 205, 290);
  point_set2D(&(pt[3]), 195, 290);

  p = polygon_createp(4, pt);

  printf("drawing stem\n");
  polygon_drawFill(p, src, DarkGreen);
  polygon_drawFillAA(p, srcAA, DarkGreen);

  // Draw big polygon for petals
  for(i=0;i<50;i++) {
    float dr = rand() % 37;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(50 + dr),
    100 + sin((float)i * M_PI * 2.0 / 50.0)*(50 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing petals\n");
  polygon_drawFill(p, src, DarkYellow);
  polygon_drawFillAA(p, srcAA, DarkYellow);

  // Draw smaller polygon for seeds
  for(i=0;i<50;i++) {
    float dr = rand() % 15;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(30 + dr),
    100 + sin((float)i * M_PI * 2.0 / 50.0)*(30 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing seeds\n");
  polygon_drawFill(p, src, DarkBrown);
  polygon_drawFillAA(p, srcAA, DarkBrown);

  // Draw polygons for grass
  for(i=0; i<390; i+=7) {
    int h = 10 + rand() % 15;
    point_set2D(&(pt[0]), i+5, 250 + h);
    point_set2D(&(pt[1]), i+10, 299);
    point_set2D(&(pt[2]), i, 299);
    polygon_set(p, 3, &(pt[0]));

    polygon_drawFill(p, src, Green);
    polygon_drawFillAA(p, srcAA, Green);
  }
    printf("drawing %d blades of grass\n", 390/7);

  printf("writing output\n");
  image_write(src, "lab4.ppm");
  image_write(srcAA, "lab4AA.ppm");

  image_free(src);

  return(0);
}
