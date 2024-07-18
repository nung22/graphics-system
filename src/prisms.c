/*
  Nicholas Ung
  Summer 2024

  Example code for the 3D hierarchical modeling system with pyramids.

  C Version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *pyramid;
  Module *pyramids;
  Module *scene;
  float angle;
  int rows = 400;
  int cols = 400;
  int i;

  Color Lime = {{0.0, 1.0, 0.0}};

  DrawState *ds;
  View3D view;

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 0.0, -40.0);
  vector_set(&(view.vpn), 0.0, 0.0, 1.0);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.0;
  view.dv = 1.0;
  view.f = 0.0;
  view.b = 50;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple pyramid module
  pyramid = module_create();
  module_pyramid(pyramid, 1);

  // make a set of a pyramid
  pyramids = module_create();

  module_identity(pyramids);
  module_color(pyramids, &Lime);
  module_scale(pyramids, 1.5, 2, 1);
  module_translate(pyramids, 1, 1, 1);
  module_module(pyramids, pyramid);

  // make a scene with lots of pyramid sets
  scene = module_create();

  for(i=0;i<30;i++) {
    // initialize LTM
    module_identity(scene);

    // rotate by some random angles
    angle = drand48() * 2*M_PI;
    module_rotateX(scene, cos(angle), sin(angle));
    angle = drand48() * 2*M_PI;
    module_rotateY(scene, cos(angle), sin(angle));
    angle = drand48() * 2*M_PI;
    module_rotateZ(scene, cos(angle), sin(angle));

    // translate to a location
    module_translate(scene, 
		     (drand48()-0.5)*15.0, 
		     (drand48()-0.5)*15.0, 
		     (drand48()-0.5)*15.0);
   
    // add a tri-pyramid
    module_module(scene, pyramids);
  }

  ds = drawstate_create();
  ds->shade = ShadeDepth;

  for(i=0;i<36;i++) {
    char buffer[256];

    image_reset( src );

    matrix_identity(&GTM);

    // Apply rotation around the Z-axis for each frame
    matrix_rotateZ(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
    
    matrix_rotateY(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
    module_draw(scene, &VTM, &GTM, ds, NULL, src);

    // write out the image
    sprintf(buffer, "frame-%03d.ppm", i);
    image_write(src, buffer);
  }

  // free stuff here
  module_delete( pyramid );
  module_delete( pyramids );
  module_delete( scene );
  image_free( src );
  

  return(0);
}
