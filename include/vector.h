#ifndef VECTOR_H

#define VECTOR_H

#include "line.h"

typedef Point Vector;

/* Vector Methods */
void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);
void vector_add(Vector *a, Vector *b, Vector *result);
void vector_subtract(Vector *a, Vector *b, Vector *result);
void vector_scale(Vector *v, float scale, Vector *result);
void point_add_vector(Point *p, Vector *v, Point *result);

#endif