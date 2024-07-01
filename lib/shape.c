// These functions provide methods for creating ellipses, circles & polyline sets.
// Written by Nicholas Ung 2024-05-26

#include <stdlib.h>
#include <math.h>
#include "shape.h"

/* Polyline */

// Returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL.
Polyline *polyline_create(void)
{
  Polyline *p = (Polyline *)malloc(sizeof(Polyline));
  p->zBuffer = 1;
  p->numVertex = 0;
  p->vertex = NULL;
  return p;
}

// Returns an allocated Polyline pointer with the vertex list initialized to the points in vlist.
Polyline *polyline_createp(int numV, Point *vlist)
{
  Polyline *p = (Polyline *)malloc(sizeof(Polyline));
  p->zBuffer = 1;
  p->numVertex = numV;
  p->vertex = (Point *)malloc(numV * sizeof(Point));
  for (int i = 0; i < numV; i++)
  {
    point_copy(&(p->vertex[i]), &(vlist[i]));
  }
  return p;
}

// Frees the internal data and the Polyline pointer.
void polyline_free(Polyline *p)
{
  if (p->vertex)
  {
    free(p->vertex);
  }
  free(p);
}

// Initializes the pre-existing Polyline to an empty Polyline.
void polyline_init(Polyline *p)
{
  p->zBuffer = 1;
  p->numVertex = 0;
  p->vertex = NULL;
}

// Initializes the vertex list to the points in vlist and de-allocates/allocates the vertex list for p, as necessary.
void polyline_set(Polyline *p, int numV, Point *vlist)
{
  if (p->vertex)
  {
    free(p->vertex);
  }
  p->numVertex = numV;
  p->vertex = (Point *)malloc(numV * sizeof(Point));
  for (int i = 0; i < numV; i++)
  {
    point_copy(&(p->vertex[i]), &(vlist[i]));
  }
}

// Frees the internal data for a Polyline, if necessary, and sets numVertex to 0 and vertex to NULL.
void polyline_clear(Polyline *p)
{
  if (p->vertex)
  {
    free(p->vertex);
  }
  p->numVertex = 0;
  p->vertex = NULL;
}

// Set the zBuffer flag to the given value.
void polyline_zBuffer(Polyline *p, int flag)
{
  p->zBuffer = flag;
}

// De-allocates/allocates space as necessary in the destination Polyline data structure and copies the vertex data from the source polyline (from) to the destination (to).
void polyline_copy(Polyline *to, Polyline *from)
{
  if (to->vertex)
  {
    free(to->vertex);
  }
  to->numVertex = from->numVertex;
  to->vertex = (Point *)malloc(to->numVertex * sizeof(Point));
  for (int i = 0; i < to->numVertex; i++)
  {
    point_copy(&(to->vertex[i]), &(from->vertex[i]));
  }
}

// Print the Polyline data to the stream designated by the FILE pointer.
void polyline_print(Polyline *p, FILE *fp)
{
  fprintf(fp, "Polyline\n");
  fprintf(fp, "zBuffer: %d\n", p->zBuffer);
  fprintf(fp, "numVertex: %d\n", p->numVertex);
  for (int i = 0; i < p->numVertex; i++)
  {
    fprintf(fp, "Vertex %d: ", i);
    point_print(&(p->vertex[i]), fp);
  }
}

// Normalize the x and y values of each vertex by the homogeneous coordinate.
void polyline_normalize(Polyline *p)
{
  for (int i = 0; i < p->numVertex; i++)
  {
    point_normalize(&(p->vertex[i]));
  }
}

// Draw the Polyline using Color c and the z-buffer, if appropriate.
void polyline_draw(Polyline *p, Image *src, Color c)
{
  for (int i = 0; i < p->numVertex - 1; i++)
  {
    Line l;
    line_set(&l, p->vertex[i], p->vertex[i + 1]);
    line_draw(&l, src, c);
  }
}

/* Circle */

// Initialize the circle to center tc and radius tr.
void circle_set(Circle *c, Point tc, double tr)
{
  point_copy(&(c->c), &tc);
  c->r = tr;
}

// Draw the circle into src using color p.
void circle_draw(Circle *c, Image *src, Color p)
{
  // Center and radius of the circle
  double x0 = c->c.val[0];
  double y0 = c->c.val[1];
  double r = c->r;

  // Initial angle and the incremental step for the angle
  double t = 0;
  double dt = 0.0001;

  // Continue until the angle has made a full circle (2*pi radians)
  while (t < 2 * M_PI)
  {
    // Calculate x and y coordinates on the perimeter of the circle using the parametric form
    double x = r * cos(t) + x0;
    double y = r * sin(t) + y0;

    // Set the color of the pixel at the calculated coordinate
    image_setColor(src, y, x, p);

    // Increment the angle by a small step to move to the next point on the circle
    t += dt;
  }
}

// Draw the filled circle into src using color p.
void circle_drawFill(Circle *c, Image *src, Color p)
{
  // Center and radius of the circle
  double x0 = c->c.val[0];
  double y0 = c->c.val[1];
  double r = c->r;

  // Line to connect the last and first points on the circle
  Line l;

  int x = 0;
  int y = r;
  int dp = 3 - 2 * r; // Initial decision parameter

  while (x <= y)
  {
    // Set points for horizontal lines and draw them using the line_draw method
    line_set2D(&l, x0 - y, y0 + x, x0 + y, y0 + x);
    line_draw(&l, src, p);
    line_set2D(&l, x0 - y, y0 - x, x0 + y, y0 - x);
    line_draw(&l, src, p);
    line_set2D(&l, x0 - x, y0 + y, x0 + x, y0 + y);
    line_draw(&l, src, p);
    line_set2D(&l, x0 - x, y0 - y, x0 + x, y0 - y);
    line_draw(&l, src, p);

    if (dp < 0)
    {
      // The midpoint is inside the circle; adjust `dp` for a horizontal move
      dp += 4 * x + 6;
    }
    else
    {
      // The midpoint is outside the circle; adjust `dp` for a diagonal move
      dp += 4 * (x - y) + 10;
      // Move diagonally, decrease `y`
      y--;
    }
    // Increment `x` with each iteration to move to the right
    x++;
  }
}

/* Ellipse */

// Initialize an ellipse to location tc and radii ta and tb.
void ellipse_set(Ellipse *e, Point tc, double ta, double tb, double angle)
{
  point_copy(&(e->c), &tc);
  e->ra = ta;
  e->rb = tb;
  e->a = angle;
}

// Draw an ellipse into src using color p.
void ellipse_draw(Ellipse *e, Image *src, Color p)
{
  double x0 = e->c.val[0]; // Center x-coordinate of the ellipse
  double y0 = e->c.val[1]; // Center y-coordinate of the ellipse
  double a = e->ra;        // Semi-major axis
  double b = e->rb;        // Semi-minor axis
  double angle = e->a;     // Rotation angle of the ellipse in radians
  double t = 0;            // Parametric angle parameter
  double dt = 0.001;       // Increment for the parametric angle, controlling the resolution

  // Loop through one full rotation around a circle
  while (t < 2 * M_PI)
  {
    // Calculate the x, y coordinates of the ellipse, adjusted for rotation and position
    double x = a * cos(t) * cos(angle) - b * sin(t) * sin(angle) + x0;
    double y = a * cos(t) * sin(angle) + b * sin(t) * cos(angle) + y0;

    // Set the pixel color at the calculated position
    image_setColor(src, y, x, p);
    t += dt; // Increment the parametric angle
  }
}

// Draw a filled ellipse into src using color p.
void ellipse_drawFill(Ellipse *e, Image *src, Color p)
{
  double x0 = e->c.val[0]; // Center x-coordinate of the ellipse
  double y0 = e->c.val[1]; // Center y-coordinate of the ellipse
  double a = e->ra;        // Semi-major axis
  double b = e->rb;        // Semi-minor axis
  double angle = e->a;     // Rotation angle of the ellipse in radians
  double t = 0;            // Parametric angle parameter starting from 0
  double dt = 0.001;       // Increment for the parametric angle
  Line l;                  // Line structure for drawing horizontal line segments

  // Iterate from 0 to π to cover the full ellipse by symmetry
  while (t <= M_PI)
  {
    // Calculate points on the ellipse, adjusted for rotation and center position
    double x = a * cos(t) * cos(angle) - b * sin(t) * sin(angle) + x0;
    double y = a * cos(t) * sin(angle) + b * sin(t) * cos(angle) + y0;

    // Calculate symmetrical point across the center
    double x1 = 2 * x0 - x;
    double y1 = 2 * y0 - y;

    // Set up a line between the symmetrical points and draw it
    line_set2D(&l, x1, y1, x, y);
    line_draw(&l, src, p);
    t += dt; // Increment the parametric angle
  }
}

/* Flood Fill */

// Fill the region of the image starting from the given pixel with the fill color (4-way fill).
void floodFill(Image *src, int x, int y, Color fillColor, Color borderColor)
{
  // Check if the current pixel is out of image bounds
  if (x < 0 || x >= src->cols || y < 0 || y >= src->rows)
    return;

  // Get the color of the current pixel
  Color currentColor = image_getColor(src, y, x);

  // Check if the pixel should not be filled
  if (color_compare(currentColor, borderColor) || color_compare(currentColor, fillColor))
    return;

  // Fill the pixel with the fill color
  image_setColor(src, y, x, fillColor);

  // Recursively fill the neighboring pixels (4-way)
  floodFill(src, x + 1, y, fillColor, borderColor);
  floodFill(src, x - 1, y, fillColor, borderColor);
  floodFill(src, x, y + 1, fillColor, borderColor);
  floodFill(src, x, y - 1, fillColor, borderColor);
}