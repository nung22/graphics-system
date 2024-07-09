// test_sphere.c

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

int main(int argc, char *argv[])
{
  Image *src;
  Module *scene;
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  Color Red = {{1.0, 0.0, 0.0}};
  Color Green = {{0.0, 1.0, 0.0}};
  Color Blue = {{0.0, 0.1, 0.9}};
  int slices = 16, stacks = 16;

  // Set up the view
  point_set3D(&(view.vrp), 3, 3, 5);
  vector_set(&(view.vpn), -3, -3, -5);
  vector_set(&(view.vup), 0, 1, 0);
  view.d = 2.2;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 50;
  view.screenx = 640;
  view.screeny = 480;

  matrix_setView3D(&vtm, &view);
  matrix_identity(&gtm);

  // Create the scene module
  scene = module_create();

  // Add a filled sphere
  module_color(scene, &Red);
  module_translate(scene, -1.5, 0, 0);
  module_sphere(scene, slices, stacks, 1);
  // Add an outline
  module_color(scene, &Blue);
  module_sphere(scene, slices, stacks, 0);

  // Add a wireframe sphere
  module_color(scene, &Green);
  module_translate(scene, 3.0, 0, 0);
  module_sphere(scene, slices, stacks, 0);

  // Create the image and drawstate
  src = image_create(view.screeny, view.screenx);
  ds = drawstate_create();
  ds->shade = ShadeConstant;

  // Draw into the scene
  module_draw(scene, &vtm, &gtm, ds, NULL, src);

  // Write out the scene
  image_write(src, "sphere.ppm");

  // Free the modules
  module_delete(scene);

  // Free the drawstate
  free(ds);

  // Free the image
  image_free(src);

  return (0);
}
