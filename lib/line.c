// These functions provide methods for writing lines.
// Written by Nicholas Ung 2024-05-26

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "line.h"

// Initialize a 2D line.
void line_set2D(Line *l, double x0, double y0, double x1, double y1)
{
  point_set2D(&(l->a), x0, y0);
  point_set2D(&(l->b), x1, y1);
}

// Initalize a line to ta and tb.
void line_set(Line *l, Point ta, Point tb)
{
  point_copy(&(l->a), &ta);
  point_copy(&(l->b), &tb);
}

// Set the zBuffer flag to the given value.
void line_zBuffer(Line *l, int flag)
{
  l->zBuffer = flag;
}

// Normalize the x and y values of the endpoints by their homogenous coordinate.
void line_normalize(Line *l)
{
  point_normalize(&(l->a));
  point_normalize(&(l->b));
}

// Copy the line data structure.
void line_copy(Line *to, Line *from)
{
  point_copy(&(to->a), &(from->a));
  point_copy(&(to->b), &(from->b));
  to->zBuffer = from->zBuffer;
}

// Draw the line into src using color c with Bresenham's line algorithm.
void line_draw(Line *l, Image *src, Color c) {
  // Extract the start and end points' coordinates and z-values
  int x0 = l->a.val[0];
  int y0 = l->a.val[1];
  int x1 = l->b.val[0];
  int y1 = l->b.val[1];
  float z0 = l->a.val[2];
  float z1 = l->b.val[2];

  // Calculate absolute differences in x and y coordinates
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  // Determine step direction along x and y axes
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;

  // Set initial error, influenced by differences in x and y
  int err = dx - dy;
  int e2;  // Temporary variable for double the error value

  // Calculate delta 1/z value
  float deltaZ;
  if (dx > dy) {
    deltaZ = (1.0 / z1 - 1.0 / z0) / dx;
  } else {
    deltaZ = (1.0 / z1 - 1.0 / z0) / dy;
  }

  // Initial 1/z value
  float curZ = 1.0 / z0;

  // Loop until the line is completely drawn
  while (1) {
    // Set pixel color if within the image boundaries
    if (x0 >= 0 && x0 < src->cols && y0 >= 0 && y0 < src->rows) {
      if (l->zBuffer) {
        // Perform z-buffer test
        if (curZ > src->z[y0][x0] ) {
          // Update the z-buffer and draw the pixel
          src->z[y0][x0] = curZ;
          image_setColor(src, y0, x0, c);
        }
      } else {
        // Draw the pixel without z-buffer
        image_setColor(src, y0, x0, c);
      }
    }

    // Check if the endpoint is reached
    if (x0 == x1 && y0 == y1) {
      break;
    }

    // Calculate twice the error to decide next pixel's position
    e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy; // Update the error
      x0 += sx;  // Move horizontally
    }
    if (e2 < dx) {
      err += dx; // Update the error
      y0 += sy;  // Move vertically
    }

    // Update the current 1/z value
    curZ += deltaZ;
  }
}