/*
  Nicholas Ung
  Summer 2024

  Creates 9 Y-wing fighters as line drawings
*/

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  View2D view;
  Matrix vtm;
  Matrix gtm;
  Module *scene;
  Module *wing;
  Module *engine;
  Module *body;
  Module *ywing;
  Module *formation;
  Module *fleet;
  int i;
  Point vrp;
  Vector xaxis;
  Point p[8];
  Line l;
  DrawState *ds;
  Image *src;

	srand48(42);

  // setup gtm
  matrix_identity( &gtm );

  // setup vtm
  point_set2D( &vrp, 0, 0 );
  vector_set( &xaxis, 1, 0, 0 );
  view2D_set( &view, &vrp, 2, &xaxis, 1000, 800 );
  matrix_setView2D( &vtm, &view );

  // create a body
  body = module_create();

  point_set2D(&p[0], 0, 0);
  point_set2D(&p[1], 2, .1);
  point_set2D(&p[2], 2.2, 0.25 );
  point_set2D(&p[3], 2, 0.4 );
  point_set2D(&p[4], 0, .5 );

  for(i=0;i<5;i++) {
    int a = i;
    int b = (i+1) % 5;
    
    line_set( &l, p[a], p[b] );
    module_line( body, &l );
  }
  line_set2D( &l, 0.6, 0.05, 0.6, 0.45 );
  module_line( body, &l );
  line_set2D( &l, 1.1, 0.08, 1.1, 0.42 );
  module_line( body, &l );

  // create an engine
  engine = module_create();
  
  point_set2D( &p[0], 0.5, 0.25 );
  point_set2D( &p[1], -0.7, 0.25 );
  point_set2D( &p[2], -0.7, 0.6 );
  point_set2D( &p[3], 0.5, 0.6 );

  for(i=0;i<4;i++) {
    int a = i;
    int b = (i+1) % 4;
    
    line_set( &l, p[a], p[b] );
    module_line( engine, &l );
  }

  // make a wing
  wing = module_create();

  point_set2D(&p[0], 0.3, 0 );
  point_set2D(&p[1], 0.3, 0.3 );
  point_set2D(&p[2], 0, 0.3 );
  point_set2D(&p[3], 0, 0 );

  for(i=0;i<4;i++) {
    int a = i;
    int b = (i+1) % 4;
    
    line_set( &l, p[a], p[b] );
    module_line( wing, &l );
  }
  module_scale2D( wing, 1.5, 1.0 );
  module_translate2D( wing, -0.05, 0.05 );
  module_module( wing, engine );

  // make an x-wing
  ywing = module_create();

  module_module(ywing, body );
  module_translate2D( ywing, 0, .5 );
  module_module( ywing, wing);
  module_identity(ywing);
  module_scale2D( ywing, 1, -1 );
  module_translate2D( ywing, 0, 0 );
  module_module( ywing, wing );

  // make a formation
  formation = module_create();

  module_module(formation, ywing );
  module_translate2D(formation, -4, 3 );
  module_module( formation, ywing );
  module_translate2D(formation, 0, -5 );
  module_module( formation, ywing );

  // make a fleet
  fleet = module_create();

  module_module(fleet, formation );
  module_scale2D(fleet, 0.5, 0.5 );
  module_translate2D(fleet, 5, 3 );
  module_module( fleet, formation );
  module_scale2D(fleet, -1.2, 1.2 );
  module_translate2D(fleet, -4, 1 );
  module_module( fleet, formation );

  // make a scene
  scene = module_create();
  module_scale2D( scene, 0.1, 0.1 );
  module_translate2D( scene, 0.2, 0 );
  module_module( scene, fleet );
  
	// draw stars into the scene
  module_identity(scene);
  for(i=0;i<50;i++) {
    point_set2D( &(p[0]), drand48()*2 - 1, drand48()*1 - 0.5 );
    module_point( scene, &(p[0]) );
  }


	// create the image and draw the module
  src = image_create( view.screeny, view.screenx );
  ds = drawstate_create(); // default color is white
  module_draw( scene, &vtm, &gtm, ds, NULL, src );

	// write out the image
  image_write( src, "ywings.ppm" );

	// free modules
  module_delete( scene );
  module_delete( formation );
  module_delete( ywing );
  module_delete( body );
  module_delete( wing );

	// free drawstate
  free( ds );

	// free image
  image_free( src );

  return(0);
}
