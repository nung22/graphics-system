// These functions provide methods for creating and manipulating modules.
// Written by Nicholas Ung 2024-06-27

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "module.h"

// Create a new element and initialize it
Element *element_create(void)
{
  Element *e = (Element *)malloc(sizeof(Element));
  if (!e)
  {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  e->type = ObjNone;
  e->next = NULL;
  return e;
}

// Initialize an element with the given type and object
Element *element_init(ObjectType type, void *obj)
{
  Element *e = element_create();
  e->type = type;
  switch (type)
  {
  case ObjPoint:
    point_copy(&(e->obj.point), obj); // Copy the point
    break;
  case ObjLine:
    line_copy(&(e->obj.line), obj); // Copy the line
    break;
  case ObjPolyline:
    polyline_init(&(e->obj.polyline));
    polyline_copy(&(e->obj.polyline), obj); // Copy the polyline
    break;
  case ObjPolygon:
    polygon_init(&(e->obj.polygon));
    polygon_copy(&(e->obj.polygon), obj); // Copy the polygon
    break;
  case ObjIdentity:
    matrix_identity(&(e->obj.matrix)); // Set the matrix to identity
  case ObjMatrix:
    matrix_copy(&(e->obj.matrix), obj); // Copy the matrix
    break;
  case ObjColor:
  case ObjBodyColor:
  case ObjSurfaceColor:
    color_copy(&(e->obj.color), obj); // Copy the color
    break;
  case ObjSurfaceCoeff:
    e->obj.coeff = *((float *)obj); // Copy the coefficient
    break;
  case ObjModule:
    e->obj.module = obj; // Store the pointer to the sub-module
    break;
  default:
    free(e);
    return NULL;
  }
  return e;
}

// Delete an element and free its memory
void element_delete(Element *e)
{
  if (!e)
  {
    return;
  }
  // Free the contained object if necessary
  if (e->type == ObjPolyline)
  {
    polyline_clear(&(e->obj.polyline));
  }
  else if (e->type == ObjPolygon)
  {
    polygon_clear(&(e->obj.polygon));
  }
  free(e);
}

// Create a new module and initialize it
Module *module_create(void)
{
  Module *md = (Module *)malloc(sizeof(Module));
  md->head = NULL;
  md->tail = NULL;
  return md;
}

// Clear a module's elements
void module_clear(Module *md)
{
  Element *current = md->head;
  while (current)
  {
    Element *next = current->next;
    element_delete(current);
    current = next;
  }
  md->head = NULL;
  md->tail = NULL;
}

// Delete a module and free its memory
void module_delete(Module *md)
{
  if (!md)
  {
    return;
  }
  module_clear(md);
  free(md);
}

// Insert an element into a module
void module_insert(Module *md, Element *e)
{
  if (!md || !e)
  {
    return;
  }
  if (!md->head)
  {
    md->head = e;
    md->tail = e;
  }
  else
  {
    md->tail->next = e;
    md->tail = e;
  }
}

// Insert a submodule into a module
void module_module(Module *md, Module *sub)
{
  Element *e = element_init(ObjModule, sub);
  module_insert(md, e);
}

// Insert a point into a module
void module_point(Module *md, Point *point)
{
  Element *e = element_init(ObjPoint, point);
  module_insert(md, e);
}

// Insert a line into a module
void module_line(Module *md, Line *line)
{
  Element *e = element_init(ObjLine, line);
  module_insert(md, e);
}

// Insert a polyline into a module
void module_polyline(Module *md, Polyline *polyline)
{
  Element *e = element_init(ObjPolyline, polyline);
  module_insert(md, e);
}

// Insert a polygon into a module
void module_polygon(Module *md, Polygon *polygon)
{
  Element *e = element_init(ObjPolygon, polygon);
  module_insert(md, e);
}

// Insert an identity matrix into a module
void module_identity(Module *md)
{
  Matrix identity;
  matrix_identity(&identity);
  Element *e = element_init(ObjIdentity, &identity);
  module_insert(md, e);
}

// Insert a 2D translation into a module
void module_translate2D(Module *md, double tx, double ty)
{
  Matrix translate;
  matrix_identity(&translate);
  matrix_translate2D(&translate, tx, ty);
  Element *e = element_init(ObjMatrix, &translate);
  module_insert(md, e);
}

// Insert a 2D scale into a module
void module_scale2D(Module *md, double sx, double sy)
{
  Matrix scale;
  matrix_identity(&scale);
  matrix_scale2D(&scale, sx, sy);
  Element *e = element_init(ObjMatrix, &scale);
  module_insert(md, e);
}

// Insert a Z-axis rotation into a module
void module_rotateZ(Module *md, double cth, double sth)
{
  Matrix rotate;
  matrix_identity(&rotate);
  matrix_rotateZ(&rotate, cth, sth);
  Element *e = element_init(ObjMatrix, &rotate);
  module_insert(md, e);
}

// Insert a 2D shear into a module
void module_shear2D(Module *md, double shx, double shy)
{
  Matrix shear;
  matrix_identity(&shear);
  matrix_shear2D(&shear, shx, shy);
  Element *e = element_init(ObjMatrix, &shear);
  module_insert(md, e);
}

// Draw the module
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src)
{
  if (!md || !VTM || !GTM || !ds || !src)
  {
    printf("Null argument passed to module_draw\n");
    return;
  }

  Matrix LTM, GTMpass;
  matrix_identity(&LTM); // set the matrix LTM to identity

  Element *e = md->head;
  while (e)
  {
    switch (e->type)
    {
    case ObjColor:
      ds->color = e->obj.color; // set the color in DrawState
      break;

    case ObjPoint:
    {
      Point X, Y;
      point_copy(&X, &e->obj.point);   // Copy the point data to a temporary point X
      matrix_xformPoint(&LTM, &X, &Y); // Transform by LTM
      matrix_xformPoint(GTM, &Y, &X);  // Transform by GTM
      matrix_xformPoint(VTM, &X, &Y);  // Transform by VTM
      point_normalize(&Y);             // Normalize by the homogeneous coordinate
      point_draw(&Y, src, ds->color);  // Draw the point
      break;
    }

    case ObjLine:
    {
      Line L;
      line_copy(&L, &e->obj.line); // Copy the line data
      matrix_xformLine(&LTM, &L);  // Transform by LTM
      matrix_xformLine(GTM, &L);   // Transform by GTM
      matrix_xformLine(VTM, &L);   // Transform by VTM
      line_normalize(&L);          // Normalize by the homogeneous coordinate
      printf("drawing line (%.2f %.2f) to (%.2f %.2f)\n",
             L.a.val[0], L.a.val[1], L.b.val[0], L.b.val[1]);
      line_draw(&L, src, ds->color); // Draw the line
      break;
    }

    case ObjPolygon:
    {
      Polygon P;
      polygon_copy(&P, &e->obj.polygon); // copy the polygon data
      matrix_xformPolygon(&LTM, &P);     // transform by LTM
      matrix_xformPolygon(GTM, &P);      // transform by GTM
      matrix_xformPolygon(VTM, &P);      // transform by VTM
      polygon_normalize(&P);             // normalize by the homogeneous coordinate
      polygon_print(&P, stdout);         // print the polygon data
      if (ds->shade == ShadeFrame)
      {
        polygon_draw(&P, src, ds->color); // draw the boundary of the polygon
      }
      else if (ds->shade == ShadeConstant)
      {
        polygon_drawFill(&P, src, ds->color); // draw the filled polygon
      }
      break;
    }

    case ObjMatrix:
      matrix_multiply(&(e->obj.matrix), &LTM, &LTM); // update LTM
      break;

    case ObjIdentity:
      matrix_identity(&LTM); // set LTM to identity
      break;

    case ObjModule:
    {
      DrawState tempDS;
      matrix_multiply(GTM, &LTM, &GTMpass);                              // GTM * LTM
      drawstate_copy(&tempDS, ds);                                       // copy the draw state
      module_draw(e->obj.module, VTM, &GTMpass, &tempDS, lighting, src); // recursive call
      break;
    }

    default:
      break;
    }
    e = (Element *)e->next;
  }
}

// Insert a 3D translation into a module
void module_translate(Module *md, double tx, double ty, double tz)
{
  Matrix translate;
  matrix_identity(&translate);
  matrix_translate(&translate, tx, ty, tz);
  Element *e = element_init(ObjMatrix, &translate);
  module_insert(md, e);
}

// Insert a 3D scale into a module
void module_scale(Module *md, double sx, double sy, double sz)
{
  Matrix scale;
  matrix_identity(&scale);
  matrix_scale(&scale, sx, sy, sz);
  Element *e = element_init(ObjMatrix, &scale);
  module_insert(md, e);
}

// Insert a rotation about the X-axis into a module
void module_rotateX(Module *md, double cth, double sth)
{
  Matrix rotate;
  matrix_identity(&rotate);
  matrix_rotateX(&rotate, cth, sth);
  Element *e = element_init(ObjMatrix, &rotate);
  module_insert(md, e);
}

// Insert a rotation about the Y-axis into a module
void module_rotateY(Module *md, double cth, double sth)
{
  Matrix rotate;
  matrix_identity(&rotate);
  matrix_rotateY(&rotate, cth, sth);
  Element *e = element_init(ObjMatrix, &rotate);
  module_insert(md, e);
}

// Insert a rotation about the X, Y, and Z axes into a module
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w)
{
  Matrix rotation;
  matrix_identity(&rotation);
  matrix_rotateXYZ(&rotation, u, v, w);
  Element *e = element_init(ObjMatrix, &rotation);
  module_insert(md, e);
}

// Insert a color into a module
void module_color(Module *md, Color *c)
{
  Element *e = element_init(ObjColor, c);
  module_insert(md, e);
}

// Insert a body color into a module
void module_bodyColor(Module *md, Color *c)
{
  Element *e = element_init(ObjBodyColor, c);
  module_insert(md, e);
}

// Insert a surface color into a module
void module_surfaceColor(Module *md, Color *c)
{
  Element *e = element_init(ObjSurfaceColor, c);
  module_insert(md, e);
}

// Insert a surface coefficient into a module
void module_surfaceCoeff(Module *md, float coeff)
{
  Element *e = element_init(ObjSurfaceCoeff, &coeff);
  module_insert(md, e);
}

/* Function definitions for module operations */

// Use the de Casteljau algorithm to subdivide a Bezier curve and add the lines to a module
void module_bezierCurve(Module *m, BezierCurve *b, int divisions)
{
  if (!m || !b)
    return;

  if (divisions == 0)
  {
    for (int i = 0; i < 3; i++)
    {
      Line line;
      line_set(&line, b->cp[i], b->cp[i + 1]);
      module_line(m, &line);
    }
  }
  else
  {
    BezierCurve left, right;
    bezierCurve_init(&left);
    bezierCurve_init(&right);
    bezierCurve_subdivide(b, &left, &right);
    module_bezierCurve(m, &left, divisions - 1);
    module_bezierCurve(m, &right, divisions - 1);
  }
}

// Use the de Casteljau algorithm to subdivide a Bezier surface and add to the module
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid)
{
  if (!m || !b)
    return;

  if (divisions == 0)
  {
    if (solid)
    {
      // Add triangles
      for (int i = 0; i < 3; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          Polygon poly;
          polygon_init(&poly);
          Point vertices[4] = {
              b->cp[i * 4 + j],
              b->cp[i * 4 + j + 1],
              b->cp[(i + 1) * 4 + j + 1],
              b->cp[(i + 1) * 4 + j]};
          polygon_set(&poly, 4, vertices);
          module_polygon(m, &poly);
        }
      }
    }
    else
    {
      // Add lines
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          Line line;
          line_set(&line, b->cp[i * 4 + j], b->cp[i * 4 + j + 1]);
          module_line(m, &line);
          line_set(&line, b->cp[j * 4 + i], b->cp[(j + 1) * 4 + i]);
          module_line(m, &line);
        }
      }
    }
  }
  else
  {
    BezierSurface topLeft, topRight, bottomLeft, bottomRight;
    bezierSurface_init(&topLeft);
    bezierSurface_init(&topRight);
    bezierSurface_init(&bottomLeft);
    bezierSurface_init(&bottomRight);
    bezierSurface_subdivide(b, &topLeft, &topRight, &bottomLeft, &bottomRight);
    module_bezierSurface(m, &topLeft, divisions - 1, solid);
    module_bezierSurface(m, &topRight, divisions - 1, solid);
    module_bezierSurface(m, &bottomLeft, divisions - 1, solid);
    module_bezierSurface(m, &bottomRight, divisions - 1, solid);
  }
}

// Insert a cube into a module
void module_cube(Module *md, int solid)
{
  Point pts[8];

  // Define the vertices of the cube
  point_set3D(&pts[0], -1, -1, -1);
  point_set3D(&pts[1], 1, -1, -1);
  point_set3D(&pts[2], 1, 1, -1);
  point_set3D(&pts[3], -1, 1, -1);
  point_set3D(&pts[4], -1, -1, 1);
  point_set3D(&pts[5], 1, -1, 1);
  point_set3D(&pts[6], 1, 1, 1);
  point_set3D(&pts[7], -1, 1, 1);

  if (solid)
  {
    Polygon p;
    Vector normals[6];
    polygon_init(&p);

    // Define normals for each face
    vector_set(&normals[0], 0, 0, -1); // Front face
    vector_set(&normals[1], 0, 0, 1);  // Back face
    vector_set(&normals[2], 0, 1, 0);  // Top face
    vector_set(&normals[3], 0, -1, 0); // Bottom face
    vector_set(&normals[4], -1, 0, 0); // Left face
    vector_set(&normals[5], 1, 0, 0);  // Right face

    // Front face
    polygon_set(&p, 4, &(pts[0]));
    polygon_setNormals(&p, p.nVertex, &normals[0]);
    module_polygon(md, &p);

    // Back face
    polygon_set(&p, 4, &(pts[4]));
    polygon_setNormals(&p, p.nVertex, &normals[1]);
    module_polygon(md, &p);

    // Top face
    point_set3D(&pts[0], -1, 1, -1);
    point_set3D(&pts[1], 1, 1, -1);
    point_set3D(&pts[2], 1, 1, 1);
    point_set3D(&pts[3], -1, 1, 1);
    polygon_set(&p, 4, &(pts[0]));
    polygon_setNormals(&p, p.nVertex, &normals[2]);
    module_polygon(md, &p);

    // Bottom face
    point_set3D(&pts[0], -1, -1, -1);
    point_set3D(&pts[1], 1, -1, -1);
    point_set3D(&pts[2], 1, -1, 1);
    point_set3D(&pts[3], -1, -1, 1);
    polygon_set(&p, 4, &(pts[0]));
    polygon_setNormals(&p, p.nVertex, &normals[3]);
    module_polygon(md, &p);

    // Left face
    point_set3D(&pts[0], -1, -1, -1);
    point_set3D(&pts[1], -1, 1, -1);
    point_set3D(&pts[2], -1, 1, 1);
    point_set3D(&pts[3], -1, -1, 1);
    polygon_set(&p, 4, &(pts[0]));
    polygon_setNormals(&p, p.nVertex, &normals[4]);
    module_polygon(md, &p);

    // Right face
    point_set3D(&pts[0], 1, -1, -1);
    point_set3D(&pts[1], 1, 1, -1);
    point_set3D(&pts[2], 1, 1, 1);
    point_set3D(&pts[3], 1, -1, 1);
    polygon_set(&p, 4, &(pts[0]));
    polygon_setNormals(&p, p.nVertex, &normals[5]);
    module_polygon(md, &p);

    polygon_clear(&p);
  }
  else
  {
    Line l;

    // Front face lines
    line_set(&l, pts[0], pts[1]);
    module_line(md, &l);
    line_set(&l, pts[1], pts[2]);
    module_line(md, &l);
    line_set(&l, pts[2], pts[3]);
    module_line(md, &l);
    line_set(&l, pts[3], pts[0]);
    module_line(md, &l);

    // Back face lines
    line_set(&l, pts[4], pts[5]);
    module_line(md, &l);
    line_set(&l, pts[5], pts[6]);
    module_line(md, &l);
    line_set(&l, pts[6], pts[7]);
    module_line(md, &l);
    line_set(&l, pts[7], pts[4]);
    module_line(md, &l);

    // Connecting edges
    line_set(&l, pts[0], pts[4]);
    module_line(md, &l);
    line_set(&l, pts[1], pts[5]);
    module_line(md, &l);
    line_set(&l, pts[2], pts[6]);
    module_line(md, &l);
    line_set(&l, pts[3], pts[7]);
    module_line(md, &l);
  }
}

void module_cylinder(Module *mod, int sides, int solid)
{
  if (!mod || sides < 3)
  {
    return;
  }

  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init(&p);
  point_set3D(&xtop, 0, 1.0, 0.0);
  point_set3D(&xbot, 0, 0.0, 0.0);

  for (i = 0; i < sides; i++)
  {
    Point pt[4];

    x1 = cos(i * M_PI * 2.0 / sides);
    z1 = sin(i * M_PI * 2.0 / sides);
    x2 = cos(((i + 1) % sides) * M_PI * 2.0 / sides);
    z2 = sin(((i + 1) % sides) * M_PI * 2.0 / sides);

    if (solid)
    {
      // Top fan triangle
      point_copy(&pt[0], &xtop);
      point_set3D(&pt[1], x1, 1.0, z1);
      point_set3D(&pt[2], x2, 1.0, z2);

      polygon_set(&p, 3, pt);
      module_polygon(mod, &p);

      // Bottom fan triangle
      point_copy(&pt[0], &xbot);
      point_set3D(&pt[1], x1, 0.0, z1);
      point_set3D(&pt[2], x2, 0.0, z2);

      polygon_set(&p, 3, pt);
      module_polygon(mod, &p);

      // Side quadrilateral
      point_set3D(&pt[0], x1, 0.0, z1);
      point_set3D(&pt[1], x2, 0.0, z2);
      point_set3D(&pt[2], x2, 1.0, z2);
      point_set3D(&pt[3], x1, 1.0, z1);

      polygon_set(&p, 4, pt);
      module_polygon(mod, &p);
    }
    else
    {
      // Wireframe: top circle
      Line line;
      point_set3D(&pt[0], x1, 1.0, z1);
      point_set3D(&pt[1], x2, 1.0, z2);

      line_set(&line, pt[0], pt[1]);
      module_line(mod, &line);

      // Wireframe: bottom circle
      point_set3D(&pt[0], x1, 0.0, z1);
      point_set3D(&pt[1], x2, 0.0, z2);

      line_set(&line, pt[0], pt[1]);
      module_line(mod, &line);

      // Wireframe: side lines
      point_set3D(&pt[0], x1, 0.0, z1);
      point_set3D(&pt[1], x1, 1.0, z1);

      line_set(&line, pt[0], pt[1]);
      module_line(mod, &line);

      point_set3D(&pt[0], x2, 0.0, z2);
      point_set3D(&pt[1], x2, 1.0, z2);

      line_set(&line, pt[0], pt[1]);
      module_line(mod, &line);
    }
  }

  polygon_clear(&p);
}