/*
  Nicholas Ung
  Summer 2024

  Test functions for creating a Tesla Cybertruck.
 */

// edit the include files as necessary
#include "graphics.h"

// draw a box
static int box( Image *src, Color color, int x, int y, int dx, int dy );
static int box( Image *src, Color color, int x, int y, int dx, int dy ) {
  Line l;
  
  line_set2D( &l, x, y, x, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x, y+dy, x + dx, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y+dy, x+dx, y );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y, x, y );
  line_draw( &l, src, color );

  return(0);
}

// draw a few boxes, a circle, and some lines.
int main(int argc, char *argv[]) {
  Image *src;
  Color Red;
  Color Green;
  Color DarkGrey;
  Color OffBlack;
  Color Black;
  Color Orange;
  Color Silver;
  Point p;
  Circle circ;
  Ellipse ell;
  Line line;

  color_set( &Red, 0.9, 0.05, 0.05 );
  color_set( &Green, 0.05, 0.9, 0.05 );
  color_set( &Black, 0.0, 0.0, 0.0 );
  color_set( &OffBlack, 0.1, 0.1, 0.1 );
  color_set( &DarkGrey, 0.2, 0.2, 0.2 );
  color_set( &Orange, 1.0, 0.5, 0.0 );
  color_set( &Silver, 0.75, 0.75, 0.75 );

  src = image_create( 400, 600 );
  image_fillrgb( src, Green.c[0], Green.c[1], Green.c[2]);

  // Shadow
  point_set2D( &p, 270, 275 );
  ellipse_set( &ell, p, 250, 15, 0);
  ellipse_drawFill( &ell, src, OffBlack );

  // Right Rear Tire
  point_set2D( &p, 450, 225 );
  circle_set( &circ, p, 46 );
  circle_drawFill( &circ, src, Black );
  circle_set( &circ, p, 45 );
  circle_drawFill( &circ, src, DarkGrey );

  // Right Front Tire
  point_set2D( &p, 150, 225 );
  circle_set( &circ, p, 46 );
  circle_drawFill( &circ, src, Black );
  circle_set( &circ, p, 45 );
  circle_drawFill( &circ, src, DarkGrey );
  
  // Car body
  box( src, OffBlack, 20, 140, 490, 90 );
  floodFill(src, 30, 160, Silver, OffBlack);
  line_set2D( &line, 20, 140, 220, 80);
  line_draw( &line, src, OffBlack );
  line_set2D( &line, 220, 80, 510, 140);
  line_draw( &line, src, OffBlack );
  floodFill(src, 220, 90, Silver, OffBlack);

  // Left Rear Tire
  point_set2D( &p, 385, 235 );
  circle_set( &circ, p, 46 );
  circle_drawFill( &circ, src, Black );
  circle_set( &circ, p, 45 );
  circle_drawFill( &circ, src, DarkGrey );
  // Hubcap
  point_set2D( &p, 385, 235 );
  circle_set( &circ, p, 35 );
  circle_drawFill( &circ, src, Black );

  // Left Front Tire
  point_set2D( &p, 85, 235 );
  circle_set( &circ, p, 46 );
  circle_drawFill( &circ, src, Black );
  circle_set( &circ, p, 45 );
  circle_drawFill( &circ, src, DarkGrey );
  // Hubcap
  point_set2D( &p, 85, 235 );
  circle_set( &circ, p, 35 );
  circle_drawFill( &circ, src, Black );

  // Car doors
  box( src, OffBlack, 135, 140, 100, 90 );
  box( src, OffBlack, 235, 140, 90, 90 );
  // Handles
  box( src, OffBlack, 210, 145, 20, 5 );
  box( src, OffBlack, 300, 145, 20, 5 );

  // Car windows
  line_set2D( &line, 85, 130, 220, 90);
  line_draw( &line, src, OffBlack );
  line_set2D( &line, 220, 90, 325, 110);
  line_draw( &line, src, OffBlack );
  line_set2D( &line, 325, 110, 325, 130);
  line_draw( &line, src, OffBlack );
  line_set2D( &line, 85, 130, 325, 130);
  line_draw( &line, src, OffBlack );
  floodFill(src, 200, 110, OffBlack, OffBlack);

  // Brake lights
  box( src, Black, 505, 140, 5, 20 );
  floodFill(src, 507, 145, Red, Black);

  // Headlights
  box( src, Black, 20, 140, 20, 5 );
  floodFill(src, 21, 142, Orange, Black);

  // Front bumper
  box( src, Black, 13, 212, 25, 20 );
  floodFill(src, 30, 215, Black, Black);

  // Step
  box( src, Black, 131, 227, 207, 7 );
  floodFill(src, 140, 230, Black, Black);

  // Rear bumper
  box( src, Black, 431, 212, 85, 20 );
  floodFill(src, 455, 215, Black, Black);

  
  image_write( src, "lab3_4.ppm" );

  image_free( src );

  return(0);
}
