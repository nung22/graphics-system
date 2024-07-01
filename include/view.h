#ifndef VIEW_H

#define VIEW_H

#include "matrix.h"

typedef struct
{
  Point vrp;    // Center of the view rectangle in world coordinates
  Vector x;     // Normalized view x-axis
  double dx;    // Width of the view rectangle in world coordinates
  int screenx;  // Number of columns in the output image
  int screeny;  // Number of rows in the output image
} View2D;

typedef struct
{
  Point vrp;   // View Reference Point
  Vector vpn;  // View Plane Normal
  Vector vup;  // View Up Vector
  double d;    // Projection Distance
  double du;   // View Window Extent (width)
  double dv;   // View Window Extent (height)
  double f;    // Front Clip Plane
  double b;    // Back Clip Plane
  int screenx; // Screen size in X dimension (pixels)
  int screeny; // Screen size in Y dimension (pixels)
} View3D;

// Function prototypes
void matrix_setView2D(Matrix *vtm, View2D *view);
void matrix_setView3D(Matrix *vtm, View3D *view);
void view2D_set(View2D *view, Point *vrp, double dx, Vector *x, int screenx, int screeny);

#endif // VIEW_H
