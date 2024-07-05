// These functions provide methods for writing points.
// Written by Nicholas Ung 2024-05-26

#include "point.h"

// Set the first two values of the vector to x & y, the third value to 0.0, and the fourth value to 1.0.
void point_set2D(Point *p, double x, double y) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = 0.0;
  p->val[3] = 1.0;
}

// Set the point’s values to x & y & z, and the homogeneous coordinate to 1.0.
void point_set3D(Point *p, double x, double y, double z) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = 1.0;
}

// Set the four values of the vector to x, y, z, and h, respectively.
void point_set(Point *p, double x, double y, double z, double h) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = h;
}

// Normalize the x and y values of a point by its homogeneous coordinate: x = x/h, y = y/h.
void point_normalize(Point *p) {
  p->val[0] = p->val[0] / p->val[3];
  p->val[1] = p->val[1] / p->val[3];
}

// Copy the point data structure.
void point_copy(Point *to, Point *from) {
  to->val[0] = from->val[0];
  to->val[1] = from->val[1];
  to->val[2] = from->val[2];
  to->val[3] = from->val[3];
}

// Draw the point into the image using Color c.
void point_draw(Point *p, Image *src, Color c) {
  int x = p->val[0];
  int y = p->val[1];
  if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
    image_setColor(src, y, x, c);
  }
}

// Draw the point into the image using FPixel c.
void point_drawf(Point *p, Image *src, FPixel c) {
  int x = p->val[0];
  int y = p->val[1];
  if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
    image_setf(src, y, x, c);
  }
}

// Print the point data structure to the stream fp.
void point_print(Point *p, FILE *fp) {
  fprintf(fp, "( %.3f, %.3f, %.3f )\n", p->val[0], p->val[1], p->val[2]);
}
