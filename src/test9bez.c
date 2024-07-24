/*
  Bruce A Maxwell
  Summer 2024
  CS 5310

  Creates a simple Bezier curve for testing surface normal directions

  Accepts one command line parameter, which is the number of
  subdivisions to execute.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  char buffer[256];
  Color white, direct, bkg, red;
  DrawState ds;
  Module *surface;
  View3D view;
  Matrix VTM, GTM;
  Point p;
  BezierSurface bs;
  int divisions = 4;
  int rows = 900, cols = 1600;
  Image *src = image_create(rows, cols);

  Lighting *light;

  // grab the command line argument, if one exists
  if(argc > 1) {
    int tmp = atoi(argv[1]);
    if( tmp >= 0 && tmp < 10 )
      divisions = tmp;
  }
  printf("Creating a single Bezier surface with %d subdivisions\n", divisions);

  color_set(&white, 0.8, 0.8, 0.8 );
  color_set(&direct, 0.8, 0.8, 0.8 );
  color_set(&bkg, 0.1, 0.07, 0.06 );
  color_set(&red, 0.8, 0.40, 0.50 );

  // set up the drawstate
  drawstate_init( &ds );

  // set up the view
  point_set3D(&(view.vrp), 0.0, 5.0, 5.0 );
  vector_set( &(view.vpn), 0.0, -5.0, -5.0 );
  vector_set( &(view.vup), 0.0, 1.0, 0.0 );
  view.d = 1.0;
  view.du = 1.0;
  view.dv = 1.0*rows/cols;
  view.screeny = rows;
  view.screenx = cols;
  view.f = 0.0;
  view.b = 10.0;

  // set up the lighting and the drawstate parameters
  light = lighting_create();
  lighting_add( light, LightAmbient, &bkg, NULL, NULL, 0, 0 );
  lighting_add( light, LightPoint, &white, NULL, &(view.vrp), 0, 0 );
  drawstate_setViewer( &ds, &(view.vrp) );
  drawstate_setShading( &ds, ShadeGouraud );

  matrix_setView3D( &VTM, &view );
  matrix_identity( &GTM );

  // create a simple Bezier surface by adjusting some of the control points up/down
  bezierSurface_init( &bs );

  bezierSurface_getPoint( &bs, &p, 0, 0 );
  p.val[1] = 1.0;
  bezierSurface_setPoint( &bs, &p, 0, 0);

  bezierSurface_getPoint( &bs, &p, 1, 0 );
  p.val[1] = 1.0;
  bezierSurface_setPoint( &bs, &p, 1, 0);

  bezierSurface_getPoint( &bs, &p, 2, 3 );
  p.val[1] = -1.0;
  bezierSurface_setPoint( &bs, &p, 2, 3);

  bezierSurface_getPoint( &bs, &p, 3, 3 );
  p.val[1] = -1.0;
  bezierSurface_setPoint( &bs, &p, 3, 3);

  bezierSurface_getPoint( &bs, &p, 0, 3 );
  p.val[1] = 0.5;
  bezierSurface_setPoint( &bs, &p, 0, 3);
  
  surface = module_create();

  module_color(surface, &white);
  module_bodyColor( surface, &red );
  module_translate( surface, -0.5, 0.0, -0.5 );
  module_scale( surface, 3, 1, 2 );
  module_bezierSurface( surface, &bs, divisions, 1 );

  module_draw( surface, &VTM, &GTM, &ds, light, src );

  sprintf(buffer, "test9bez.ppm");
  image_write(src, buffer);

  // clean up
  printf("cleaning up\n");
  lighting_delete( light);
  module_delete( surface );
  image_free( src );
  
  return(0);
}
