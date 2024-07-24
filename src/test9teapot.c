/*
  Bruce A Maxwell
  Summer 2024
  CS 5310

  Creates a teapot and renders using ShadeGouraud or ShadeFlat

  Accepts one command line parameter, which is the number of
  subdivisions to execute.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  int frame;
  Color blue, green, purple, red, white, direct, bkg;
  DrawState ds;
  Module *teapot;
  View3D view;
  Matrix VTM, GTM;
  int divisions = 4;
  int rows = 400, cols = 600;
  Image *src = image_create(rows, cols);

  Lighting *light;

  // grab the command line argument, if one exists
  if(argc > 1) {
    int tmp = atoi(argv[1]);
    if( tmp >= 0 && tmp < 10 )
      divisions = tmp;
  }
  printf("Creating teapot with %d subdivisions\n", divisions);

  color_set(&white, 1.0, 1.0, 1.0 );
  color_set(&direct, 0.8, 0.8, 0.8 );
  color_set(&blue, .1, .2, .8);
  color_set(&green, .2, 0.7, 0.3 );
  color_set(&purple, 0.6, 0.1, 0.7 );
  color_set(&red, 0.75, 0.3, 0.3 );
  color_set(&bkg, 0.05, 0.04, 0.03 );

  teapot = module_create();

  module_color(teapot, &purple);
  module_bodyColor( teapot, &purple );
  module_teapot(teapot, divisions, 1);

  // set up the drawstate
  drawstate_init( &ds );

  // set up the view
  point_set3D(&(view.vrp), 0.0, 2.4, -7.0 );
  vector_set( &(view.vpn), 0.0, -0.6, 4.0 );
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
  drawstate_setShading( &ds, ShadeFlat );

  matrix_setView3D( &VTM, &view );
  matrix_identity( &GTM );

  matrix_rotateX(&GTM, cos(M_PI/2.0), -sin(M_PI/2.0) );

  for(frame=0;frame<60;frame++) {
    char buffer[256];

    module_draw( teapot, &VTM, &GTM, &ds, light, src );
    matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );

    sprintf(buffer, "frame-%03d.ppm", frame);
    image_write(src, buffer);
    image_reset(src);
  }

  // clean up
  printf("cleaning up\n");
  lighting_delete( light);
  module_delete( teapot );
  image_free( src );
  
  return(0);
}
