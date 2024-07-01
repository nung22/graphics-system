/*
  Nicholas Ung
  Summer 2024

  Test functions for creating a cookie and ellipse pattern.
 */

// edit the include files as necessary
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

// Print out images for extensions 3 and 4
int main(int argc, char *argv[]) {
  Image *src;
  Color Red;
  Color Green;
  Color Orange;
  Color LightBrown;
  Color DarkBrown;
  Point p;
  Circle circ;
  Ellipse ell;

  color_set( &Red, 0.9, 0.05, 0.05 );
  color_set( &Green, 0.05, 0.9, 0.05 );
  color_set( &Orange, 1.0, 0.5, 0.0 );
  color_set( &LightBrown, 0.6, 0.4, 0.2 );
  color_set( &DarkBrown, 0.4, 0.2, 0.05 );

  /* Extension 3 */

  srand48(0);

  const int nPoints = 1000;

  Polyline *chocolateChip;
  Point plist[nPoints];
  int i;
  int rows = 500;
  int cols = 500;
  
  printf("Startup\n");
  for(i=0;i<nPoints;i++) {
    point_set2D( &(plist[i]), drand48()*cols/2, drand48()*rows/2 );
  }

  // Create a polyline
	chocolateChip = polyline_createp( 10, &(plist[105] ) );
  
  // Create an image
  src = image_create( rows, cols );

  // Draw a cookie
  point_set2D( &p, 250, 250 );
  circle_set( &circ, p, 200 );
  circle_drawFill( &circ, src, LightBrown );

  // Draw several chocolate chips
  point_set2D( &(plist[500]), 100, 300 );
  point_set2D( &(plist[501]), 100, 350 );
  point_set2D( &(plist[502]), 150, 350 );
  point_set2D( &(plist[503]), 150, 300 );
  point_set2D( &(plist[504]), 100, 300 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 120, 325, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 250, 370 );
  point_set2D( &(plist[501]), 250, 420 );
  point_set2D( &(plist[502]), 300, 420 );
  point_set2D( &(plist[503]), 300, 370 );
  point_set2D( &(plist[504]), 250, 370 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 275, 380, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 250, 100 );
  point_set2D( &(plist[501]), 250, 150 );
  point_set2D( &(plist[502]), 300, 150 );
  point_set2D( &(plist[503]), 300, 100 );
  point_set2D( &(plist[504]), 250, 100 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 275, 120, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 230, 250 );
  point_set2D( &(plist[501]), 230, 300 );
  point_set2D( &(plist[502]), 280, 300 );
  point_set2D( &(plist[503]), 280, 250 );
  point_set2D( &(plist[504]), 230, 250 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 245, 280, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 350, 310 );
  point_set2D( &(plist[501]), 350, 360 );
  point_set2D( &(plist[502]), 400, 360 );
  point_set2D( &(plist[503]), 400, 310 );
  point_set2D( &(plist[504]), 350, 310 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 375, 325, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 370, 190 );
  point_set2D( &(plist[501]), 370, 240 );
  point_set2D( &(plist[502]), 420, 240 );
  point_set2D( &(plist[503]), 420, 190 );
  point_set2D( &(plist[504]), 370, 190 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 375, 225, DarkBrown, DarkBrown);

  point_set2D( &(plist[500]), 125, 150 );
  point_set2D( &(plist[501]), 125, 200 );
  point_set2D( &(plist[502]), 175, 200 );
  point_set2D( &(plist[503]), 175, 150 );
  point_set2D( &(plist[504]), 125, 150 );
  polyline_set( chocolateChip, 5, &(plist[500]) );
  polyline_draw( chocolateChip, src, DarkBrown );
  floodFill(src, 150, 175, DarkBrown, DarkBrown);

  image_write( src, "lab3_extension3.ppm");

  printf("Cleanup\n");
  polyline_free( chocolateChip );
  image_free( src );

    /* Extension 4 */

  src = image_create( rows, cols );

  // Draw some ellipses with various orientations
  int x = 50;
  int y = 50;
  int angle = 0;
  int willFill = 0;
  for (i = 0; i < 54; i++) {
    Color pillColor;
    point_set2D( &p, x, y );
    ellipse_set( &ell, p, 20, 10, angle);

    // Rotate colors
    if (i % 3 == 0) {
      pillColor = Red;
    }
    else if (i % 3 == 1) {
      pillColor = Green;
    }
    else {
      pillColor = Orange;
    }

    ellipse_draw( &ell, src, pillColor );
    if (willFill) {
      floodFill(src, x, y, pillColor, pillColor);
    }

    if(x < 450) {
      x += 50;
    }
    else {
      x = 50;
      y += 75;
      willFill = !willFill;
    }
    angle += 8;
  }

  
  image_write( src, "lab3_extension4.ppm" );

  image_free( src );


  return(0);
}
