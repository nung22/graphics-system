/*
  Nicholas Ung
  Summer 2024
  CS 5310

  Creates a series of mochis on skewers as a custom 3D model.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  char buffer[256];
  Color white, direct, bkg, red, green, brown, gold;
  DrawState ds;
  Module *mochi, *skewer, *scene;
  View3D view;
  Matrix VTM, GTM;
  int rows = 900, cols = 1600;
  Image *src = image_create(rows, cols);

  Lighting *light;

  color_set(&white, 0.8, 0.8, 0.8 );
  color_set(&direct, 0.8, 0.8, 0.8 );
  color_set(&bkg, 0.1, 0.07, 0.06 );
  color_set(&red, 0.8, 0.40, 0.50 );
  color_set(&green, 0.0, 1.0, 0.0 );
  color_set(&brown, 0.5, 0.3, 0.1 );
  color_set(&gold, 1.0, 0.8, 0.0 );

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
  
  // create 3 mochis
  mochi = module_create();
  module_color(mochi, &white);
  module_scale( mochi, .5, .4, .4 );
  module_bodyColor( mochi, &white );
  module_sphere( mochi, 10, 20, 1 );

  module_translate( mochi, 0, 0.7, 0 );
  module_bodyColor( mochi, &red );
  module_sphere( mochi, 10, 20, 1 );

  module_translate( mochi, 0, 0.7, 0 );
  module_bodyColor( mochi, &green );
  module_sphere( mochi, 10, 20, 1 );

  // create a skewer
  skewer = module_create();
  module_module( skewer, mochi );
  module_scale( skewer, .1, 4.5, .1 );
  module_bodyColor( skewer, &brown );
  module_cube( skewer, 1 );

  // scene with three desserts (skewer module)
  scene = module_create();
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.5 ), sin( 0.5 ) );
  module_rotateY( scene, cos(-0.1), sin(-0.1) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, -2, 0, 0 );
  module_module( scene, skewer );

  module_identity(scene);
  module_scale( scene, 0.9, 0.9, 0.9 );
  module_rotateZ( scene, cos( 0.3 ), sin( 0.1 ) );
  module_rotateY( scene, cos(-0.05), sin(-0.05) );
  module_rotateX( scene, cos( 0.2), sin(0.1) );
  module_translate( scene, 2.5, 0, 0 );
  module_module( scene, skewer );

  module_identity(scene);
  module_scale( scene, 1.1, 1.1, 1.1 );
  module_rotateZ( scene, cos( 0.1 ), sin( 0.1 ) );
  module_rotateX( scene, cos( 0.2), sin(-0.2) );
  module_translate( scene, 0, -0.5, 0 );
  module_module( scene, skewer );

  module_draw( scene, &VTM, &GTM, &ds, light, src );

  sprintf(buffer, "test9e.ppm");
  image_write(src, buffer);

  printf("Converting to low resolution image\n");
  sprintf(buffer, "convert -resize 25%% test9e.ppm test9e.png");
  system(buffer);

  printf("Deleting high-res image\n");
  system("rm test9e.ppm");

  // clean up
  printf("cleaning up\n");
  lighting_delete( light);
  module_delete( scene );
  module_delete( mochi );
  module_delete( skewer );
  image_free( src );
  
  return(0);
}
