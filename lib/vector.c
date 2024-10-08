// These functions provide methods for creating and manipulating vectors.
// Written by Nicholas Ung 2024-06-04

#include "vector.h"
#include <math.h>

// Sets the vector data to the specified values.
void vector_set(Vector *v, double x, double y, double z)
{
  v->val[0] = x;
  v->val[1] = y;
  v->val[2] = z;
  v->val[3] = 0.0; // Ensure the w component is zero for vectors
}

// Prints the vector data to the stream designated by the FILE pointer.
void vector_print(Vector *v, FILE *fp)
{
  fprintf(fp, "( %.3f, %.3f, %.3f )\n", v->val[0], v->val[1], v->val[2]);
}

// Copies the data from the source vector to the destination vector.
void vector_copy(Vector *dest, Vector *src)
{
  for (int i = 0; i < 4; i++)
  {
    dest->val[i] = src->val[i];
  }
}

// Returns the length of the vector.
double vector_length(Vector *v)
{
  return sqrt(v->val[0] * v->val[0] + v->val[1] * v->val[1] + v->val[2] * v->val[2]);
}

// Normalizes the vector.
void vector_normalize(Vector *v)
{
  double length = vector_length(v);
  if (length > 0)
  {
    for (int i = 0; i < 3; i++)
    {
      v->val[i] /= length;
    }
  }
}

// Returns the dot product of the two vectors.
double vector_dot(Vector *a, Vector *b)
{
  return a->val[0] * b->val[0] + a->val[1] * b->val[1] + a->val[2] * b->val[2];
}

// Returns the cross product of the two vectors.
void vector_cross(Vector *a, Vector *b, Vector *c)
{
  c->val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1];
  c->val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2];
  c->val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0];
  c->val[3] = 0.0; // Ensure the w component is zero for vectors
}

// Adds two vectors and stores the result in the third vector
void vector_add(Vector *a, Vector *b, Vector *result)
{
  result->val[0] = a->val[0] + b->val[0];
  result->val[1] = a->val[1] + b->val[1];
  result->val[2] = a->val[2] + b->val[2];
  result->val[3] = 0.0; // homogeneous coordinate for vectors
}

// Subtracts two vectors and stores the result in the third vector
void vector_subtract(Vector *a, Vector *b, Vector *result)
{
  result->val[0] = a->val[0] - b->val[0];
  result->val[1] = a->val[1] - b->val[1];
  result->val[2] = a->val[2] - b->val[2];
  result->val[3] = 0.0; // homogeneous coordinate for vectors
}

// Scales a vector by a scalar value
void vector_scale(Vector *v, float scale, Vector *result)
{
  vector_set(result, v->val[0] * scale, v->val[1] * scale, v->val[2] * scale);
}

// Adds a scaled vector to a point
void point_add_vector(Point *p, Vector *v, Point *result)
{
  point_set3D(result, p->val[0] + v->val[0], p->val[1] + v->val[1], p->val[2] + v->val[2]);
}