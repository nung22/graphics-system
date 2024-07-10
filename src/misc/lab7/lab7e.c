/*
Nicholas Ung
Summer 2024

Draws a curtain using spheres, cylinders, and bezier surfaces.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int frame;
	Color blue, white;
	Point p[16];
	BezierSurface bc;
	DrawState ds;
	Module *curtain;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int slices = 8, stacks = 8;
	double angle = M_PI / 2; // 90 degrees in radians
  int sides = 10;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	color_set(&white, 1.0, 1.0, 1.0 );
	color_set(&blue, .1, .2, .8);

	curtain = module_create();

	// create a rod
	module_color(curtain, &white);
	module_scale(curtain, .025, 1, .025);
	module_rotateX(curtain, cos(angle), sin(angle));
	module_translate(curtain, 1, 1, 0);
	module_cylinder(curtain, sides, 0);
	module_translate(curtain, -1, -1, 0);
	module_rotateX(curtain, cos(-angle), sin(-angle));
	module_scale(curtain, 40, 1, 40);

	// put caps on the rod
	module_scale(curtain, .05, .05, .05);
	module_translate(curtain, 1, 1, -.05);
	module_sphere(curtain, slices, stacks, 0);
	module_translate(curtain, 0, 0, 1.1);
	module_sphere(curtain, slices, stacks, 0);
	module_translate(curtain, -1, -1, -1.05);
	module_scale(curtain, 20, 20, 20);

	// create the curtain
	point_set3D(&p[0], 0.0, 0.0, 0.0); // first row, constant x, even spacing in z
	point_set3D(&p[1], 0.0, 0.2, 0.44);
	point_set3D(&p[2], 0.0, 0.5, 0.66);
	point_set3D(&p[3], 0.0, 0.1, 1.0);
	point_set3D(&p[4], 0.44, 0.8, 0.0); // second row
	point_set3D(&p[5], 0.44, -0.1, 0.44);
	point_set3D(&p[6], 0.44, 0.0, 0.66);
	point_set3D(&p[7], 0.44, 0.3, 1.0);
	point_set3D(&p[8], 0.66, 0.3, 0.0); // third row
	point_set3D(&p[9], 0.66, 0.8, 0.44);
	point_set3D(&p[10], 0.66, 0.9, 0.66);
	point_set3D(&p[11], 0.66, 0.5, 1.0);
	point_set3D(&p[12], 1.0, 1.0, 0.0); // fourth row
	point_set3D(&p[13], 1.0, 0.4, 0.44);
	point_set3D(&p[14], 1.0, 0.7, 0.66);
	point_set3D(&p[15], 1.0, 1.0, 1.0);
	bezierSurface_set(&bc, p);

	// put the curtain into a module
	module_color(curtain, &blue);
	module_bezierSurface(curtain, &bc, divisions, 1);

	// set up the drawstate
	drawstate_setColor(&ds, white);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 1.2, -3.0 );
	vector_set( &(view.vpn), 0.0, -0.8, 2.5 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );

	// Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( curtain, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "bs%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( curtain );

	return(0);
}
