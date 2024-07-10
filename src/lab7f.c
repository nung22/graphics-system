/*
Nicholas Ung
Summer 2024

Draws a flying carpet using pyramids and bezier surfaces.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int frame;
	Color gold, white, red;
	Point p[16];
	BezierSurface bc;
	DrawState ds;
	Module *carpet;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	color_set(&red, 0.75, 0.3, 0.3 );
	color_set(&white, 1.0, 1.0, 1.0 );
	color_set(&gold, 1, .8, 0);

	carpet = module_create();

	// create tassels
	module_color(carpet, &gold);
	module_scale(carpet, .025, .1, .025);
	module_pyramid(carpet, 1);
	module_translate(carpet, 1, .5, 0);
	module_pyramid(carpet, 1);
	module_translate(carpet, 0, -.1, 1);
	module_pyramid(carpet, 1);
	module_translate(carpet, -1, -.4, 0);
	module_pyramid(carpet, 1);
	module_translate(carpet, 0, 0, -1);
	module_scale(carpet, 40, 10, 40);

	// create the carpet
	point_set3D(&p[0], 0.0, 0.1, 0.0); // first row, constant x, even spacing in z
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
	point_set3D(&p[12], 1.0, 0.6, 0.0); // fourth row
	point_set3D(&p[13], 1.0, 0.4, 0.44);
	point_set3D(&p[14], 1.0, 0.7, 0.66);
	point_set3D(&p[15], 1.0, 0.5, 1.0);
	bezierSurface_set(&bc, p);

	// put the carpet into a module
	module_color(carpet, &red);
	module_bezierSurface(carpet, &bc, divisions, 1);

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
		module_draw( carpet, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "bs%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( carpet );

	return(0);
}
