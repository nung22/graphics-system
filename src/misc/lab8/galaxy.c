/*
  Nicholas Ung
  Summer 2024

  Code to make a 3D solar system model.
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
  Module *sun;
  Module *planet;
  Module *solarSystem;
  float angle;
  int rows = 400;
  int cols = 400;
  int i;
  int slices = 10, stacks = 10;

  Color Yellow = {{1.0, 1.0, 0.0}};
  Color Red = {{1.0, 0.0, 0.0}};
  Color Blue = {{0.0, 0.0, 1.0}};
  Color Green = {{0.0, 1.0, 0.0}};
  Color Orange = {{1.0, 0.5, 0.0}};
  Color Turquoise = {{0.0, 1.0, 1.0}};

  DrawState *ds;
  View3D view;

  // Initialize the image
  src = image_create(rows, cols);

  // Initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // Set the View parameters
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

  // Print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // Make a simple sphere module
  sphere = module_create();
  module_sphere(sphere, slices, stacks, 1);

  // Create the sun module
  sun = module_create();
  module_color(sun, &Yellow);
  module_scale(sun, 2.0, 2.0, 2.0); // Larger size for the sun
  module_module(sun, sphere);

  // Create a planet module
  planet = module_create();

  // Make a solar system module with the sun and planets
  solarSystem = module_create();

  // Add the sun to the solar system
  module_module(solarSystem, sun);

  // Add planets with different colors, sizes, and positions
  Color planetColors[] = {Red, Blue, Green, Orange, Turquoise};
  float planetDistances[] = {4.0, 6.0, 8.0, 10.0, 12.0};
  float planetSizes[] = {0.5, 0.6, 0.7, 0.8, 0.9};
  float planetZOffsets[] = {0.0, 0.2, -0.2, 0.4, -0.4};
  int numPlanets = 5;

  for (i = 0; i < numPlanets; i++) {
    module_identity(planet);
    module_color(planet, &planetColors[i]);
    module_scale(planet, planetSizes[i], planetSizes[i], planetSizes[i]);
    module_translate(planet, planetDistances[i], 0, planetZOffsets[i]); // Position on x and z axes
    module_module(planet, sphere);
    module_module(solarSystem, planet);
  }

  ds = drawstate_create();
  ds->shade = ShadeDepth;

  for (i = 0; i < 36; i++) {
    char buffer[256];

    image_reset(src);

    matrix_identity(&GTM);
    matrix_rotateY(&GTM, cos(i * 2 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));
    module_draw(solarSystem, &VTM, &GTM, ds, NULL, src);

    // Write out the image
    sprintf(buffer, "frame-%03d.ppm", i);
    image_write(src, buffer);
  }

  // Free resources
  module_delete(sphere);
  module_delete(sun);
  module_delete(planet);
  module_delete(solarSystem);
  image_free(src);

  return 0;
}