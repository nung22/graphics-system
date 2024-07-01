#ifndef SHAPE_H

#define SHAPE_H

#include "line.h"

// Structure to represent polyline fields
typedef struct
{
  int zBuffer;   // whether to use z-buffer; defaults to true
  int numVertex; // number of vertices
  Point *vertex; // vertex information
} Polyline;

// Structure to represent a circle
typedef struct
{
  double r; // radius
  Point c;  // center
} Circle;

// Structure to represent an ellipse
typedef struct
{
  double ra, rb; // major axis radius, minor axis radius
  Point c;       // center
  double a;      // angle of major axis relative to x-axis
} Ellipse;

/* Function prototypes for graphics operations */ 

// Polyline
Polyline *polyline_create(void);
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, Color c);

// Circle
void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, Color p);
void circle_drawFill(Circle *c, Image *src, Color p);

// Ellipse
void ellipse_set(Ellipse *e, Point tc, double ta, double tb, double angle);
void ellipse_draw(Ellipse *e, Image *src, Color p);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);

// Flood Fill
void floodFill(Image *src, int x, int y, Color fillColor, Color borderColor);

#endif // SHAPE_H