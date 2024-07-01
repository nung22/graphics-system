#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

// Print the matrix to the file pointer
void matrix_print(Matrix *m, FILE *fp)
{
  for (int i = 0; i < 4; i++)
  {
    fprintf(fp, "| ");
    for (int j = 0; j < 4; j++)
    {
      fprintf(fp, "%.3f ", m->m[i][j]);
    }
    fprintf(fp, "|\n");
  }
  fprintf(fp, "\n");
}

// Clear the matrix to all zeros
void matrix_clear(Matrix *m)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      m->m[i][j] = 0.0;
    }
  }
}

// Set the matrix to the identity matrix
void matrix_identity(Matrix *m)
{
  matrix_clear(m);
  for (int i = 0; i < 4; i++)
  {
    m->m[i][i] = 1.0;
  }
}

// Get the value at the specified row and column
double matrix_get(Matrix *m, int row, int col)
{
  return m->m[row][col];
}

// Set the value at the specified row and column
void matrix_set(Matrix *m, int row, int col, double val)
{
  m->m[row][col] = val;
}

// Copy the data from the source matrix to the destination matrix
void matrix_copy(Matrix *to, Matrix *from)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      to->m[i][j] = from->m[i][j];
    }
  }
}

// Transpose the matrix
void matrix_transpose(Matrix *m)
{
  Matrix temp;
  matrix_copy(&temp, m);
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      m->m[i][j] = temp.m[j][i];
    }
  }
}

// Multiply the left matrix by the right matrix and store the result in the result matrix
void matrix_multiply(Matrix *left, Matrix *right, Matrix *result)
{
  Matrix temp;
  matrix_clear(&temp);
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      for (int k = 0; k < 4; k++)
      {
        temp.m[i][j] += left->m[i][k] * right->m[k][j];
      }
    }
  }
  matrix_copy(result, &temp);
}

// Transform the point by the matrix
void matrix_xformPoint(Matrix *m, Point *p, Point *q)
{
  Point temp;
  point_copy(&temp, q);
  for (int i = 0; i < 4; i++) {
    temp.val[i] = 0.0;
    for (int j = 0; j < 4; j++) {
      temp.val[i] += m->m[i][j] * p->val[j];
    }
  }
  point_copy(q, &temp);
}

// Transform the vector by the matrix
void matrix_xformVector(Matrix *m, Vector *v, Vector *q)
{
  matrix_xformPoint(m, (Point *)v, (Point *)q);
}

// Transform the polygon by the matrix
void matrix_xformPolygon(Matrix *m, Polygon *p)
{
  for (int i = 0; i < p->nVertex; i++)
  {
    Point transformedPoint;
    matrix_xformPoint(m, &(p->vertex[i]), &transformedPoint);
    point_copy(&(p->vertex[i]), &transformedPoint);
  }
}

// Transform the polyline by the matrix
void matrix_xformPolyline(Matrix *m, Polyline *p)
{
  for (int i = 0; i < p->numVertex; i++)
  {
    Point transformedPoint;
    matrix_xformPoint(m, &(p->vertex[i]), &transformedPoint);
    point_copy(&(p->vertex[i]), &transformedPoint);
  }
}

// Transform the line by the matrix
void matrix_xformLine(Matrix *m, Line *l)
{
  Point transformedPoint;
  matrix_xformPoint(m, &(l->a), &transformedPoint);
  point_copy(&(l->a), &transformedPoint);
  matrix_xformPoint(m, &(l->b), &transformedPoint);
  point_copy(&(l->b), &transformedPoint);
}

// Scale the matrix in 2D
void matrix_scale2D(Matrix *m, double sx, double sy)
{
  // Create a scale matrix
  Matrix scale, result;
  matrix_identity(&scale);
  scale.m[0][0] = sx;
  scale.m[1][1] = sy;

  // Multiply the scale matrix with the input matrix
  matrix_multiply(&scale, m, &result);

  // Copy the result back to the input matrix
  matrix_copy(m, &result);
}

// Rotate the matrix about the Z axis
void matrix_rotateZ(Matrix *m, double cth, double sth)
{
  Matrix rotate, result;
  matrix_identity(&rotate);
  rotate.m[0][0] = cth;
  rotate.m[0][1] = -sth;
  rotate.m[1][0] = sth;
  rotate.m[1][1] = cth;
  matrix_multiply(&rotate, m, &result);
  matrix_copy(m, &result);
}

// Translate the matrix in 2D
void matrix_translate2D(Matrix *m, double tx, double ty)
{
  Matrix translate, result;
  matrix_identity(&translate);
  translate.m[0][3] = tx;
  translate.m[1][3] = ty;
  matrix_multiply(&translate, m, &result);
  matrix_copy(m, &result);
}

// Shear the matrix in 2D
void matrix_shear2D(Matrix *m, double shx, double shy)
{
  Matrix shear, result;
  matrix_identity(&shear);
  shear.m[0][1] = shx;
  shear.m[1][0] = shy;
  matrix_multiply(&shear, m, &result);
  matrix_copy(m, &result);
}

// Translate the matrix in 3D
void matrix_translate(Matrix *m, double tx, double ty, double tz)
{
  Matrix translate, result;
  matrix_identity(&translate);
  translate.m[0][3] = tx;
  translate.m[1][3] = ty;
  translate.m[2][3] = tz;
  matrix_multiply(&translate, m, &result);
  matrix_copy(m, &result);
}

// Scale the matrix in 3D
void matrix_scale(Matrix *m, double sx, double sy, double sz)
{
  Matrix scale, result;
  matrix_identity(&scale);
  scale.m[0][0] = sx;
  scale.m[1][1] = sy;
  scale.m[2][2] = sz;
  matrix_multiply(&scale, m, &result);
  matrix_copy(m, &result);
}

// Rotate the matrix about the X axis
void matrix_rotateX(Matrix *m, double cth, double sth)
{
  Matrix rotate, result;
  matrix_identity(&rotate);
  rotate.m[1][1] = cth;
  rotate.m[1][2] = -sth;
  rotate.m[2][1] = sth;
  rotate.m[2][2] = cth;
  matrix_multiply(&rotate, m, &result);
  matrix_copy(m, &result);
}

// Rotate the matrix about the Y axis
void matrix_rotateY(Matrix *m, double cth, double sth)
{
  Matrix rotate, result;
  matrix_identity(&rotate);
  rotate.m[0][0] = cth;
  rotate.m[0][2] = sth;
  rotate.m[2][0] = -sth;
  rotate.m[2][2] = cth;
  matrix_multiply(&rotate, m, &result);
  matrix_copy(m, &result);
}

// Rotate the matrix about the X, Y, and Z axes
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w)
{
  Matrix rotation, result;
  matrix_identity(&rotation);
  rotation.m[0][0] = u->val[0];
  rotation.m[0][1] = u->val[1];
  rotation.m[0][2] = u->val[2];
  rotation.m[1][0] = v->val[0];
  rotation.m[1][1] = v->val[1];
  rotation.m[1][2] = v->val[2];
  rotation.m[2][0] = w->val[0];
  rotation.m[2][1] = w->val[1];
  rotation.m[2][2] = w->val[2];
  matrix_multiply(&rotation, m, &result);
  matrix_copy(m, &result);
}

// Shear the matrix about the Z axis
void matrix_shearZ(Matrix *m, double shx, double shy)
{
  Matrix shear, result;
  matrix_identity(&shear);
  shear.m[0][2] = shx;
  shear.m[1][2] = shy;
  matrix_multiply(&shear, m, &result);
  matrix_copy(m, &result);
}

// Apply perspective transformation to the matrix
void matrix_perspective(Matrix *m, double d)
{
  Matrix perspective, result;
  matrix_identity(&perspective);
  perspective.m[2][2] = 0.0;
  perspective.m[2][3] = -1.0 / d;
  perspective.m[3][2] = 1.0 / d;
  perspective.m[3][3] = 0.0;
  matrix_multiply(&perspective, m, &result);
  matrix_copy(m, &result);
}
