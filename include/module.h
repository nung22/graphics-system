#ifndef MODULE_H

#define MODULE_H

#include "point.h"
#include "shape.h"
#include "polygon.h"
#include "matrix.h"
#include "drawstate.h"
#include "bezier.h"

// Enumerated type for the object type method
typedef enum {
  ObjNone,
  ObjPoint,
  ObjLine,
  ObjPolyline,
  ObjPolygon,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  ObjLight,
  ObjModule
} ObjectType;

// Union to hold the different types of objects
typedef union {
  Point point;
  Line line;
  Polyline polyline;
  Polygon polygon;
  Matrix matrix;
  Color color;
  float coeff;
  void* module;
} Object;

// Structure to represent an element in a module
typedef struct {
  ObjectType type;
  Object obj;
  void* next;
} Element;

// Structure to represent a module
typedef struct {
  Element* head;
  Element* tail;
} Module;

// Structure to represent lighting
typedef struct {
  int nLights;
} Lighting;

/* Function prototypes for elements */
Element* element_create(void);
Element* element_init(ObjectType type, void* obj);
void element_delete(Element* e);

/* Function prototypes for modules */
Module* module_create(void);
void module_clear(Module* md);
void module_delete(Module* md);
void module_insert(Module* md, Element* e);
void module_module(Module* md, Module* sub);
void module_identity(Module* md);
void module_translate2D(Module* md, double tx, double ty);
void module_scale2D(Module* md, double sx, double sy);
void module_rotateZ(Module* md, double cth, double sth);
void module_shear2D(Module* md, double shx, double shy);
void module_draw(Module* md, Matrix* VTM, Matrix* GTM, DrawState* ds, Lighting* lighting, Image* src);

void module_translate(Module* md, double tx, double ty, double tz);
void module_scale(Module* md, double sx, double sy, double sz);
void module_rotateX(Module* md, double cth, double sth);
void module_rotateY(Module* md, double cth, double sth);
void module_rotateXYZ(Module* md, Vector* u, Vector* v, Vector* w);

void module_color(Module* md, Color* c);
void module_bodyColor(Module* md, Color* c);
void module_surfaceColor(Module* md, Color* c);
void module_surfaceCoeff(Module* md, float coeff);

/* Function prototypes for primitive type modules */
void module_point(Module* md, Point* p);
void module_line(Module* md, Line* p);
void module_polyline(Module* md, Polyline* p);
void module_polygon(Module* md, Polygon* p);
void module_cube(Module* md, int solid);
void module_cylinder(Module* md, int sides, int solid);
void module_sphere(Module* md, int slices, int stacks, int solid);
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);

#endif // MODULE_H