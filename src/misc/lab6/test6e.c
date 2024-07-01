/*
Nicholas Ung
Summer 2024

Draws three 3D pencils
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

  Takes in the number of subdivisions to use when creating the cylinder
 */
void cylinder(Module *mod, int sides);
void cylinder(Module *mod, int sides)
{
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init(&p);
  point_set3D(&xtop, 0, 1.0, 0.0);
  point_set3D(&xbot, 0, 0.0, 0.0);

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for (i = 0; i < sides; i++)
  {
    Point pt[4];

    x1 = cos(i * M_PI * 2.0 / sides);
    z1 = sin(i * M_PI * 2.0 / sides);
    x2 = cos(((i + 1) % sides) * M_PI * 2.0 / sides);
    z2 = sin(((i + 1) % sides) * M_PI * 2.0 / sides);

    point_copy(&pt[0], &xtop);
    point_set3D(&pt[1], x1, 1.0, z1);
    point_set3D(&pt[2], x2, 1.0, z2);

    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);

    point_copy(&pt[0], &xbot);
    point_set3D(&pt[1], x1, 0.0, z1);
    point_set3D(&pt[2], x2, 0.0, z2);

    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);

    point_set3D(&pt[0], x1, 0.0, z1);
    point_set3D(&pt[1], x2, 0.0, z2);
    point_set3D(&pt[2], x2, 1.0, z2);
    point_set3D(&pt[3], x1, 1.0, z1);

    polygon_set(&p, 4, pt);
    module_polygon(mod, &p);
  }

  polygon_clear(&p);
}

// makes 3 pencils
int main(int argc, char *argv[])
{

  Image *src;
  Module *scene;
  Module *eraser;
  Module *tips;
  Module *lead;
  Module *body;
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  Color Pink = {{1.0, 0.7, 0.7}};
  Color Orange = {{1.0, 0.5, 0.2}};
  Color Tan = {{0.9, 0.8, 0.6}};
  Color Silver = {{0.7, 0.7, 0.7}};
  Color Gray = {{0.5, 0.5, 0.5}};

  // set up the view
  point_set3D(&(view.vrp), 15, 30, 100);
  vector_set(&(view.vpn), -20, -30, -100);
  vector_set(&(view.vup), 0, 1, 0);
  view.d = 2.2;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 50;
  view.screenx = 640;
  view.screeny = 360;

  matrix_setView3D(&vtm, &view);
  matrix_identity(&gtm);

  // eraser
  eraser = module_create();
  module_scale(eraser, 1.3, 6, 1.3);
  module_rotateX(eraser, 0, 1);
  module_color(eraser, &Pink);
  cylinder(eraser, 10);
  module_translate(eraser, 0, 0, 5);
  module_scale(eraser, 1, 1, 0.7);
  module_color(eraser, &Silver);
  cylinder(eraser, 10);

  // lead
  lead = module_create();
  module_color(lead, &Gray);
  module_scale(lead, 0.5, 5, 0.5);
  module_rotateX(lead, 0, 1);
  cylinder(lead, 6);
  module_scale(lead, 0.4, 0.4, 1.0);
  module_translate(lead, 0, 0, 2.5);
  cylinder(lead, 10);

  // tips
  tips = module_create();

  // add eraser
  module_identity(tips);
  module_translate(tips, 0, 0, -5);
  module_module(tips, eraser);

  // add lead 
  module_translate(tips, 0, 0, 37);
  module_module(tips, lead);


  // body
  body = module_create();

  module_color(body, &Orange);

  // add tips
  module_module(body, tips);


  // add body
  module_scale(body, 1.1, 30, 1.1);
  module_rotateX(body, 0, 1);
  module_translate(body, 0, 0, 3);
  cylinder(body, 10); 

  // add exposed wood
  module_color(body, &Tan);
  module_scale(body, .8, .8, .1);
  module_translate(body, 0, 0, 31);
  cylinder(body, 10);
  module_translate(body, 0, 0, 2);
  module_scale(body, .8, .8, 1);
  cylinder(body, 10);

  // scene with three pencils (body module)
  scene = module_create();
  module_scale(scene, 0.7, 0.7, 0.7);
  module_rotateZ(scene, cos(0.5), sin(0.5));
  module_rotateY(scene, cos(-0.1), sin(-0.1));
  module_rotateX(scene, cos(0.2), sin(0.2));
  module_translate(scene, -15, -10, -10);
  module_module(scene, body);

  module_identity(scene);
  module_scale(scene, 0.7, 0.7, 0.7);
  module_rotateZ(scene, cos(0.3), sin(0.3));
  module_rotateY(scene, cos(-0.05), sin(-0.05));
  module_rotateX(scene, cos(0.2), sin(0.2));
  module_translate(scene, 5, -1, -50);
  module_module(scene, body);

  module_identity(scene);
  module_scale(scene, 0.7, 0.7, 0.7);
  module_rotateZ(scene, cos(0.1), sin(0.1));
  module_rotateX(scene, cos(0.2), sin(0.2));
  module_translate(scene, -35, -3, -60);
  module_module(scene, body);

  // create the image and drawstate
  src = image_create(360, 640);
  ds = drawstate_create();
  ds->shade = ShadeFrame;

  // draw into the scene
  module_draw(scene, &vtm, &gtm, ds, NULL, src);

  // write out the scene
  image_write(src, "pencils.ppm");

  // free the modules
  module_delete(scene);
  module_delete(tips);
  module_delete(lead);
  module_delete(body);
  module_delete(eraser);

  // free the drawstate
  free(ds);

  // free the image
  image_free(src);

  return (0);
}
