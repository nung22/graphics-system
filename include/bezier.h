#ifndef BEZIER_H

#define BEZIER_H

#include "line.h"

// Structure to represent a Bezier curve
typedef struct
{
  Point cp[4]; // Four control points for a cubic Bezier curve
  int zbuffer;
} BezierCurve;

// Structure to represent a Bezier surface
typedef struct
{
  Point cp[16]; // 4x4 control points for a cubic Bezier surface
  int zbuffer;
} BezierSurface;

/* Function prototypes for Bezier operations */
void bezierCurve_init(BezierCurve *b);
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_copy(BezierCurve *to, BezierCurve *from);
void bezierCurve_zBuffer(BezierCurve *b, int flag);
void bezierSurface_zBuffer(BezierSurface *b, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);


/* Function prototypes for subdivision */
void bezierCurve_subdivide(BezierCurve *b, BezierCurve *left, BezierCurve *right);
void bezierSurface_subdivide(BezierSurface *b, BezierSurface *topLeft, BezierSurface *topRight, BezierSurface *bottomLeft, BezierSurface *bottomRight);

#endif // BEZIER_H
