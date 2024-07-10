/*
Nicholas Ung
Summer 2024

Draws two 3D cylinders: one filled and one made of lines
*/

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

/*
  Adds to the module a unit cylinder, aligned along the Y-axis
  Takes in the number of subdivisions to use when creating the cylinder
  and a flag indicating if it should be solid
 */
void module_cylinder(Module *mod, int sides, int solid);

int main(int argc, char *argv[]) {
    Image *src;
    Module *scene, *cylinderFilled, *cylinderWireframe;
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    Color Blue = {{0.0, 0.0, 1.0}};
    Color Red = {{1.0, 0.0, 0.0}};
    int sides = 20;

    // set up the view
    point_set3D(&(view.vrp), 3, 3, 10);
    vector_set(&(view.vpn), -3, -3, -10);
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

    // create the filled cylinder module
    cylinderFilled = module_create();
    module_color(cylinderFilled, &Blue);
    module_cylinder(cylinderFilled, sides, 1);

    // create the wireframe cylinder module
    cylinderWireframe = module_create();
    module_color(cylinderWireframe, &Red);
    module_cylinder(cylinderWireframe, sides, 0);

    // create the scene module
    scene = module_create();

    // add the filled cylinder
    module_translate(scene, -1.5, 0, 0);
    module_module(scene, cylinderFilled);

    // add the wireframe cylinder
    module_translate(scene, 3.0, 0, 0);
    module_module(scene, cylinderWireframe);

    // create the image and drawstate
    src = image_create(view.screeny, view.screenx);
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    // draw into the scene
    module_draw(scene, &vtm, &gtm, ds, NULL, src);

    // write out the scene
    image_write(src, "cylinders.ppm");

    // free the modules
    module_delete(scene);
    module_delete(cylinderFilled);
    module_delete(cylinderWireframe);

    // free the drawstate
    free(ds);

    // free the image
    image_free(src);

    return (0);
}
