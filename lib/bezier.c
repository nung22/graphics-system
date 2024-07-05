// These functions provide methods for manipulating Bezier curves and surfaces.
// Written by Nicholas Ung 2024-05-26

#include "bezier.h"
#include <math.h>
#include <stdio.h>

/* Function definitions for Bezier operations */

// Initialize a Bezier curve
void bezierCurve_init(BezierCurve *b)
{
  if (b)
  {
    for (int i = 0; i < 4; i++)
    {
      point_set2D(&b->cp[i], i / 3.0, 0);
    }
    b->zbuffer = 1;
  }
}

// Initialize a Bezier surface
void bezierSurface_init(BezierSurface *b)
{
  if (b)
  {
    for (int i = 0; i < 16; i++)
    {
      point_set3D(&b->cp[i], (i % 4) / 3.0, 0, (i / 4) / 3.0);
    }
    b->zbuffer = 1;
  }
}

// Set the control points of a Bezier curve
void bezierCurve_set(BezierCurve *b, Point *vlist)
{
  if (b && vlist)
  {
    for (int i = 0; i < 4; i++)
    {
      point_copy(&b->cp[i], &vlist[i]);
    }
  }
}

// Set the control points of a Bezier surface
void bezierSurface_set(BezierSurface *b, Point *vlist)
{
  if (b && vlist)
  {
    for (int i = 0; i < 16; i++)
    {
      point_copy(&b->cp[i], &vlist[i]);
    }
  }
}

// Set the z-buffer flag of a Bezier surface
void bezierSurface_zBuffer(BezierSurface *b, int flag)
{
  if (b)
  {
    b->zbuffer = flag;
  }
}

// Copy the control points of a Bezier curve
void bezierCurve_copy(BezierCurve *to, BezierCurve *from)
{
  if (to && from)
  {
    for (int i = 0; i < 4; i++)
    {
      point_copy(&to->cp[i], &from->cp[i]);
    }
    to->zbuffer = from->zbuffer;
  }
}

// Set the z-buffer flag of a Bezier curve
void bezierCurve_zBuffer(BezierCurve *b, int flag)
{
  if (b)
  {
    b->zbuffer = flag;
  }
}
// Calculate a point on the Bezier curve using de Casteljau's algorithm
static void bezier_curve_point(Point *ctrl, double t, Point *out)
{
  Point temp[4];

  // Copy the control points to the temp array
  for (int i = 0; i < 4; i++)
  {
    point_copy(&temp[i], &ctrl[i]);
  }

  // Perform the de Casteljau algorithm
  for (int r = 1; r < 4; r++)
  {
    for (int i = 0; i < 4 - r; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        temp[i].val[j] = (1 - t) * temp[i].val[j] + t * temp[i + 1].val[j];
      }
    }
  }

  // Copy the final point to the output
  point_copy(out, &temp[0]);
}

// Draw a Bezier curve onto an image using adaptive subdivision
void bezierCurve_draw(BezierCurve *b, Image *src, Color c)
{
  if (!b || !src)
    return;

  int steps = 10;

  Point pt1, pt2;
  for (int i = 0; i <= steps; i++)
  {
    double t = (double)i / steps;
    bezier_curve_point(b->cp, t, &pt2);
    if (i > 0)
    {
      Line line;
      line_set(&line, pt1, pt2);
      line_draw(&line, src, c);
    }
    point_copy(&pt1, &pt2);
  }
}

// Subdivide a Bezier curve using the de Casteljau algorithm
void bezierCurve_subdivide(BezierCurve *b, BezierCurve *left, BezierCurve *right)
{
  Point q0, q1, q2, q3;
  Point r0, r1, r2, r3;
  Point temp;

  // q0 = p0
  point_copy(&q0, &b->cp[0]);

  // r3 = p3
  point_copy(&r3, &b->cp[3]);

  // q1 = 1/2 (p0 + p1)
  point_set(&q1,
            0.5 * (b->cp[0].val[0] + b->cp[1].val[0]),
            0.5 * (b->cp[0].val[1] + b->cp[1].val[1]),
            0.5 * (b->cp[0].val[2] + b->cp[1].val[2]),
            0.5 * (b->cp[0].val[3] + b->cp[1].val[3]));

  // r2 = 1/2 (p2 + p3)
  point_set(&r2,
            0.5 * (b->cp[2].val[0] + b->cp[3].val[0]),
            0.5 * (b->cp[2].val[1] + b->cp[3].val[1]),
            0.5 * (b->cp[2].val[2] + b->cp[3].val[2]),
            0.5 * (b->cp[2].val[3] + b->cp[3].val[3]));

  // q2 = 1/2 (q1 + 1/2 (p1 + p2))
  point_set(&temp,
            0.5 * (b->cp[1].val[0] + b->cp[2].val[0]),
            0.5 * (b->cp[1].val[1] + b->cp[2].val[1]),
            0.5 * (b->cp[1].val[2] + b->cp[2].val[2]),
            0.5 * (b->cp[1].val[3] + b->cp[2].val[3]));
  point_set(&q2,
            0.5 * (q1.val[0] + temp.val[0]),
            0.5 * (q1.val[1] + temp.val[1]),
            0.5 * (q1.val[2] + temp.val[2]),
            0.5 * (q1.val[3] + temp.val[3]));

  // r1 = 1/2 (r2 + 1/2 (p1 + p2))
  point_set(&temp,
            0.5 * (b->cp[1].val[0] + b->cp[2].val[0]),
            0.5 * (b->cp[1].val[1] + b->cp[2].val[1]),
            0.5 * (b->cp[1].val[2] + b->cp[2].val[2]),
            0.5 * (b->cp[1].val[3] + b->cp[2].val[3]));
  point_set(&r1,
            0.5 * (r2.val[0] + temp.val[0]),
            0.5 * (r2.val[1] + temp.val[1]),
            0.5 * (r2.val[2] + temp.val[2]),
            0.5 * (r2.val[3] + temp.val[3]));

  // q3 = 1/2 (q2 + r1)
  point_set(&q3,
            0.5 * (q2.val[0] + r1.val[0]),
            0.5 * (q2.val[1] + r1.val[1]),
            0.5 * (q2.val[2] + r1.val[2]),
            0.5 * (q2.val[3] + r1.val[3]));

  // r0 = q3
  point_copy(&r0, &q3);

  // Copy the control points to the left and right curves
  point_copy(&left->cp[0], &q0);
  point_copy(&left->cp[1], &q1);
  point_copy(&left->cp[2], &q2);
  point_copy(&left->cp[3], &q3);

  point_copy(&right->cp[0], &r0);
  point_copy(&right->cp[1], &r1);
  point_copy(&right->cp[2], &r2);
  point_copy(&right->cp[3], &r3);
}

// Subdivide a Bezier surface using the de Casteljau algorithm
void bezierSurface_subdivide(BezierSurface *surface, BezierSurface *topLeft, BezierSurface *topRight, BezierSurface *bottomLeft, BezierSurface *bottomRight)
{
  BezierCurve curvesU[4], leftCurvesU[4], rightCurvesU[4];
  BezierCurve curvesV[4], topLeftCurves[4], topRightCurves[4], bottomLeftCurves[4], bottomRightCurves[4];

  // Step 1: Subdivide the rows (u-direction)
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      curvesU[i].cp[j] = surface->cp[i * 4 + j];
    }
    bezierCurve_subdivide(&curvesU[i], &leftCurvesU[i], &rightCurvesU[i]);
  }

  // Step 2: Transpose and subdivide the columns (v-direction) for the left and right halves
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      curvesV[i].cp[j] = leftCurvesU[j].cp[i];
    }
    bezierCurve_subdivide(&curvesV[i], &topLeftCurves[i], &bottomLeftCurves[i]);

    for (int j = 0; j < 4; j++)
    {
      curvesV[i].cp[j] = rightCurvesU[j].cp[i];
    }
    bezierCurve_subdivide(&curvesV[i], &topRightCurves[i], &bottomRightCurves[i]);
  }

  // Step 3: Copy the subdivided points to the resulting surfaces
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      topLeft->cp[i * 4 + j] = topLeftCurves[j].cp[i];
      topRight->cp[i * 4 + j] = topRightCurves[j].cp[i];
      bottomLeft->cp[i * 4 + j] = bottomLeftCurves[j].cp[i];
      bottomRight->cp[i * 4 + j] = bottomRightCurves[j].cp[i];
    }
  }
}
