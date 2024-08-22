#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

int main(int argc, char *argv[])
{
  // Set up image
  int rows = 360;
  int cols = 640;
  Image *src = image_create(rows, cols);

  // Set up colors
  Color White, Grey, Silver, Gold, Red;
  color_set(&White, 1.0, 1.0, 1.0);
  color_set(&Grey, 0.5, 0.5, 0.5);
  color_set(&Silver, 0.75, 0.75, 0.75);
  color_set(&Gold, 1.0, 0.84, 0.0);
  color_set(&Red, 1.0, 0.0, 0.0);

  // Set up the view
  View3D view;
  Matrix VTM, GTM;
  point_set3D(&(view.vrp), 5, 5, -10);
  vector_set(&(view.vpn), -5, -5, 10);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);
  matrix_identity(&GTM);

  // Set up drawstate
  DrawState *ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp));
  ds->shade = ShadeGouraud;

  // Set up lighting
  Lighting *light = lighting_create();
  lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0, 0);

  // Create the torus module
  Module *scene = module_create();


  // Animate the ring
  for (int frame = 0; frame < 72; frame++)
  {
    // Clear the image for the next frame
    image_reset(src);

    // Clear the module 
    module_clear(scene);

    matrix_identity(&GTM);

    module_bodyColor(scene, &Gold);
    module_surfaceColor(scene, &Grey);
    matrix_rotateY(&GTM, cos(frame * 2 * M_PI / 36.0), sin(frame * 2 * M_PI / 36.0));
    module_torus(scene, 2.0, 0.5, 18, 9, 1);

    // Draw the scene
    module_draw(scene, &VTM, &GTM, ds, light, src);

    // Write out the image for each frame (optional)
    char filename[20];
    sprintf(filename, "frame-%02d.ppm", frame);
    image_write(src, filename);
  }

  // Free resources
  image_free(src);
  module_delete(scene);
  lighting_delete(light);

  return 0;
}