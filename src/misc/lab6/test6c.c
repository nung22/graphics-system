/*
Nicholas Ung
Summer 2024

Demonstrates model cube function

Draw a cube.
 */
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

int main(int argc, char *argv[])
{
  // Create the necessary objects
  Image *src;
  Module *cube, *cubeOutline;
  Matrix vtm, gtm;
  DrawState *ds;
  View3D view;
  Color Blue = {{0.0, 0.1, 0.9}}; // Blue color for the cube
  Color Red = {{0.9, 0.1, 0.0}};  // Red color for the cube outline

  // Set up the view parameters
  point_set3D(&(view.vrp), 5, 5, 10);   // Adjust camera position
  vector_set(&(view.vpn), -5, -5, -10); // Point the camera towards the origin
  vector_set(&(view.vup), 0, 1, 0);     // Keep the up direction along the y-axis

  view.d = 1.0;       // Distance to the view plane
  view.du = 1.0;      // Width of the view plane
  view.dv = 1.0;      // Height of the view plane
  view.f = 0.0;       // Front clip plane
  view.b = 50.0;      // Back clip plane
  view.screenx = 640; // Screen width
  view.screeny = 360; // Screen height

  // Set up the matrices
  matrix_setView3D(&vtm, &view); // Set the view transformation matrix
  matrix_identity(&gtm);         // Set the global transformation matrix

  // Create the cube module
  cube = module_create();
  module_scale(cube, 2.0, 2.0, 2.0);
  module_color(cube, &Blue);
  module_cube(cube, 1); // Solid cube

  // Create the outline of the cube
  cubeOutline = module_create();
  module_color(cubeOutline, &Red);
  module_cube(cubeOutline, 0); // Cube outline

  // Add the outline to the cube
  module_module(cube, cubeOutline);

  // Create the image and drawstate
  src = image_create(view.screeny, view.screenx);
  ds = drawstate_create();
  ds->shade = ShadeConstant;

  // Draw the cube
  module_draw(cube, &vtm, &gtm, ds, NULL, src);

  // Write out the image
  image_write(src, "solid_cube.ppm");

  // Free the allocated resources
  module_delete(cube);
  free(ds);
  image_free(src);

  return 0;
}
