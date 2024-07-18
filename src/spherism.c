/*
  Nicholas Ung
  Summer 2024

  Example code for the 3D hierarchical modeling system with spheres.

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
  Module *sphere;
  Module *spheres;
  Module *scene;
  float angle;
  int rows = 400;
  int cols = 400;
  int i;
  int slices = 10, stacks = 10;

  Color Magenta = {{1.0, 0.0, 1.0}};
  Color Orange = {{1.0, 0.5, 0.0}};
  Color Turqoise = {{0.0, 1.0, 1.0}};

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

  // make a simple sphere module
  sphere = module_create();
  module_sphere(sphere, slices, stacks, 1);

  // make a set of 3 spheres
  spheres = module_create();

  module_identity(spheres);
  module_color(spheres, &Magenta);
  module_scale(spheres, 1.2, 1.2, 1.2);
  module_translate(spheres, 1, 1, 1);
  module_module(spheres, sphere);

  module_identity(spheres);
  module_color(spheres, &Orange);
  module_translate(spheres, -1, -1, -1);
  module_module(spheres, sphere);

  module_identity(spheres);
  module_color(spheres, &Turqoise);
  module_module(spheres, sphere);

  // make a scene with lots of sphere sets
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
   
    // add a tri-sphere
    module_module(scene, spheres);
  }

  ds = drawstate_create();
  ds->shade = ShadeDepth;

  for(i=0;i<36;i++) {
    char buffer[256];

    image_reset( src );

    matrix_identity(&GTM);
    matrix_rotateY(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
    module_draw(scene, &VTM, &GTM, ds, NULL, src);

    // write out the image
    sprintf(buffer, "frame-%03d.ppm", i);
    image_write(src, buffer);
  }

  // free stuff here
  module_delete( sphere );
  module_delete( spheres );
  module_delete( scene );
  image_free( src );
  

  return(0);
}
