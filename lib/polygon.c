// These functions provide methods for creating and manipulating polygons.
// Written by Nicholas Ung 2024-06-04

#include "polygon.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SUPER_SAMPLING_RATE 4

// Returns an allocated Polygon pointer initialized so that numVertex is 0 and vertex is NULL.
Polygon *polygon_create(void)
{
  Polygon *p = (Polygon *)malloc(sizeof(Polygon)); // Allocate memory for the polygon structure
  if (p)
  {                   // Check if the polygon pointer is not NULL
    p->nVertex = 0;   // Set the number of vertices to 0
    p->vertex = NULL; // Set the vertex pointer to NULL
    p->color = NULL;  // Set the color pointer to NULL
    p->normal = NULL; // Set the normal pointer to NULL
    p->zBuffer = 0;   // Set the zBuffer flag to 0
    p->oneSided = 1;  // Set the oneSided flag to 1
  }
  return p; // Return the polygon pointer
}

// Returns an allocated Polygon pointer with the vertex list initialized to a copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist)
{
  Polygon *p = polygon_create(); // Create a new polygon
  if (p)
  {                              // Check if the polygon pointer is not NULL
    polygon_set(p, numV, vlist); // Set the vertex list to a copy of the points in vlist
  }
  return p; // Return the polygon pointer
}

// Frees the internal data for a Polygon and the Polygon pointer.
void polygon_free(Polygon *p)
{
  if (p)
  {
    if (p->vertex)
      free(p->vertex);
    if (p->color)
      free(p->color);
    if (p->normal)
      free(p->normal);
    free(p);
  }
}

// Initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p)
{
  p->nVertex = 0;
  p->vertex = NULL;
  p->color = NULL;
  p->normal = NULL;
  p->zBuffer = 0;
  p->oneSided = 1;
}

// Initializes the vertex array to the points in vlist.
void polygon_set(Polygon *p, int numV, Point *vlist)
{
  if (p)
  {
    // if (p->vertex)
    //   free(p->vertex);
    p->nVertex = numV;
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    for (int i = 0; i < numV; i++)
    {
      p->vertex[i] = vlist[i];
    }
  }
}

// Frees the internal data and resets the fields.
void polygon_clear(Polygon *p)
{
  if (p)
  {
    if (p->vertex)
    {
      free(p->vertex);
      p->vertex = NULL;
    }
    if (p->color)
    {
      free(p->color);
      p->color = NULL;
    }
    if (p->normal)
    {
      free(p->normal);
      p->normal = NULL;
    }
    p->nVertex = 0;
    p->zBuffer = 0;
    p->oneSided = 1;
  }
}

// Sets the oneSided field to the value.
void polygon_setSided(Polygon *p, int oneSided)
{
  if (p)
  {
    p->oneSided = oneSided;
  }
}

// Initializes the color array to the colors in clist.
void polygon_setColors(Polygon *p, int numV, Color *clist)
{
  if (p)
  {
    p->color = (Color *)malloc(numV * sizeof(Color));
    for (int i = 0; i < numV; i++)
    {
      p->color[i] = clist[i];
    }
  }
}

// Initializes the normal array to the vectors in nlist.
void polygon_setNormals(Polygon *p, int numV, Vector *nlist)
{
  if (p)
  {
    p->normal = (Vector *)malloc(numV * sizeof(Vector));
    for (int i = 0; i < numV; i++)
    {
      p->normal[i] = nlist[i];
    }
  }
}

// Initializes all fields of the polygon.
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided)
{
  polygon_set(p, numV, vlist);
  polygon_setColors(p, numV, clist);
  polygon_setNormals(p, numV, nlist);
  polygon_zBuffer(p, zBuffer);
  polygon_setSided(p, oneSided);
}

// Sets the z-buffer flag to the given value.
void polygon_zBuffer(Polygon *p, int flag)
{
  if (p)
  {
    p->zBuffer = flag;
  }
}

// Copies the data from one polygon to another.
void polygon_copy(Polygon *to, Polygon *from)
{
  if (to && from)
  {
    polygon_set(to, from->nVertex, from->vertex);
    if (from->color)
    {
      polygon_setColors(to, from->nVertex, from->color);
    }
    if (from->normal)
    {
      polygon_setNormals(to, from->nVertex, from->normal);
    }
    polygon_zBuffer(to, from->zBuffer);
    polygon_setSided(to, from->oneSided);
  }
}

// Prints polygon data to the stream designated by the FILE pointer.
void polygon_print(Polygon *p, FILE *fp)
{
  if (p)
  {
    fprintf(fp, "Polygon: %d vertices\n", p->nVertex);
    for (int i = 0; i < p->nVertex; i++)
    {
      fprintf(fp, "  ");
      point_print(&p->vertex[i], fp);
    }
  }
}

// Normalize the x, y, and z values of each vertex by the homogeneous coordinate.
void polygon_normalize(Polygon *p)
{
  if (p)
  {
    for (int i = 0; i < p->nVertex; i++)
    {
      point_normalize(&p->vertex[i]);
    }
  }
}

/********************
Scanline Fill Algorithm
********************/

// // define the struct here, because it is local to only this file
// typedef struct tEdge
// {
//   float x0, y0;                /* start point for the edge */
//   float x1, y1;                /* end point for the edge */
//   int yStart, yEnd;            /* start row and end row */
//   float xIntersect, dxPerScan; /* where the edge intersects the current scanline and how it changes. we'll add more here later */
//   struct tEdge *next;
// } Edge;

// /*
//   This is a comparison function that returns a value < 0 if a < b, a
//   value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
//   Edge structure.  It is used to sort the overall edge list.
//  */
// static int compYStart(const void *a, const void *b)
// {
//   Edge *ea = (Edge *)a;
//   Edge *eb = (Edge *)b;

//   return (ea->yStart - eb->yStart);
// }

// /*
//   This is a comparison function that returns a value < 0 if a < b, a
//   value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
//   Edge structure.  It is used to sort the active edge list.
//  */
// static int compXIntersect(const void *a, const void *b)
// {
//   Edge *ea = (Edge *)a;
//   Edge *eb = (Edge *)b;

//   if (ea->xIntersect < eb->xIntersect)
//     return (-1);
//   else if (ea->xIntersect > eb->xIntersect)
//     return (1);

//   return (0);
// }

// /*
//   Allocates, creates, fills out, and returns an Edge structure given
//   the inputs.

//   Current inputs are just the start and end location in image space.
//   Eventually, the points will be 3D and we'll add color and texture
//   coordinates.
//  */
// static Edge *makeEdgeRec(Point start, Point end, Image *src)
// {
//   Edge *edge;
//   float dscan = end.val[1] - start.val[1];

//   /* INSERT YOUR CODE HERE */

//   // Check if the starting row is below the image or the end row is
//   // above the image and skip the edge if either is true
//   if (start.val[1] < 0 || end.val[1] < 0 || start.val[1] >= src->rows || end.val[1] >= src->rows)
//   {
//     return NULL;
//   }

//   // Allocate an edge structure and set the x0, y0, x1, y1 values
//   edge = (Edge *)malloc(sizeof(Edge));
//   edge->x0 = start.val[0];
//   edge->y0 = start.val[1];
//   edge->x1 = end.val[0];
//   edge->y1 = end.val[1];

//   // Turn on an edge only if the edge starts in the top half of it or
//   // the lower half of the pixel above it.  In other words, round the
//   // start y value to the nearest integer and assign it to
//   // edge->yStart.
//   edge->yStart = (int)(start.val[1] + 0.5);

//   // Turn off the edge if it starts in the lower half or the top half
//   // of the next pixel.  In other words, round the end y value to the
//   // nearest integer and subtract 1, assigning the result to
//   // edge->yEnd.
//   edge->yEnd = (int)(end.val[1] + 0.5) - 1;

//   // Clip yEnd to the number of rows-1.
//   if (edge->yEnd >= src->rows)
//   {
//     edge->yEnd = src->rows - 1;
//   }

//   // Calculate the slope, dxPerScan
//   edge->dxPerScan = (end.val[0] - start.val[0]) / dscan;

//   // Calculate xIntersect, adjusting for the fraction of the point in the pixel.
//   // Scanlines go through the middle of pixels
//   // Move the edge to the first scanline it crosses
//   edge->xIntersect = edge->x0 + edge->dxPerScan * ((float)(edge->yStart) + 0.5 - edge->y0);

//   // Adjust if the edge starts above the image
//   if (edge->y0 < 0)
//   {
//     edge->xIntersect += edge->dxPerScan * ((float)(-edge->y0));
//     edge->y0 = 0;
//   }

//   // Check for really bad cases with steep slopes where xIntersect has gone beyond the end of the edge
//   if (edge->xIntersect < 0)
//   {
//     edge->xIntersect = 0;
//   }
//   else if (edge->xIntersect >= src->cols)
//   {
//     edge->xIntersect = src->cols - 1;
//   }

//   // Return the newly created edge data structure
//   return (edge);
// }

// /*
//   Returns a list of all the edges in the polygon in sorted order by
//   smallest row.
// */
// static LinkedList *setupEdgeList(Polygon *p, Image *src)
// {
//   LinkedList *edges = NULL;
//   Point v1, v2;
//   int i;

//   // create a linked list
//   edges = ll_new();

//   // walk around the polygon, starting with the last point
//   v1 = p->vertex[p->nVertex - 1];

//   for (i = 0; i < p->nVertex; i++)
//   {

//     // the current point (i) is the end of the segment
//     v2 = p->vertex[i];

//     // if it is not a horizontal line
//     if ((int)(v1.val[1] + 0.5) != (int)(v2.val[1] + 0.5))
//     {
//       Edge *edge;

//       // if the first coordinate is smaller (top edge)
//       if (v1.val[1] < v2.val[1])
//         edge = makeEdgeRec(v1, v2, src);
//       else
//         edge = makeEdgeRec(v2, v1, src);

//       // insert the edge into the list of edges if it's not null
//       if (edge)
//         ll_insert(edges, edge, compYStart);
//     }
//     v1 = v2;
//   }

//   // check for empty edges (like nothing in the viewport)
//   if (ll_empty(edges))
//   {
//     ll_delete(edges, NULL);
//     edges = NULL;
//   }

//   return (edges);
// }

// /*
//   Draw one scanline of a polygon given the scanline, the active edges,
//   a DrawState, the image, and some Lights (for Phong shading only).
//  */
// static void fillScan(int scan, LinkedList *active, Image *src, Color c)
// {
//   Edge *p1, *p2;
//   int i, f;

//   // Loop over the list
//   p1 = ll_head(active);
//   while (p1)
//   {
//     // The edges have to come in pairs, draw from one to the next
//     p2 = ll_next(active);
//     if (!p2)
//     {
//       printf("bad bad bad (your edges are not coming in pairs)\n");
//       break;
//     }

//     // If the xIntersect values are the same, don't draw anything.
//     // Just go to the next pair.
//     if (p2->xIntersect == p1->xIntersect)
//     {
//       p1 = ll_next(active);
//       continue;
//     }

//     /* INSERT YOUR CODE HERE */

//     // Identify the starting column and clip to the left side of the image
//     i = (int)(p1->xIntersect + 0.5);
//     if (i < 0)
//     {
//       i = 0;
//     }

//     // Identify the ending column and clip to the right side of the image
//     f = (int)(p2->xIntersect + 0.5);
//     if (f >= src->cols)
//     {
//       f = src->cols - 1;
//     }

//     // Loop from start to end and color in the pixels
//     for (; i <= f; i++)
//     {
//       image_setColor(src, scan, i, c);
//     }

//     // move ahead to the next pair of edges
//     p1 = ll_next(active);
//   }

//   return;
// }

// // Function to save an image frame
// static void save_frame(Image *src, int frameNumber)
// {
//   char filename[50];
//   snprintf(filename, sizeof(filename), "frame_%02d.ppm", frameNumber);
//   image_write(src, filename);
// }

// /*
//   Process the edge list, assumes the edges list has at least one entry
// */
// static int processEdgeList(LinkedList *edges, Image *src, Color c)
// {
//   LinkedList *active = NULL;
//   LinkedList *tmplist = NULL;
//   LinkedList *transfer = NULL;
//   Edge *current;
//   Edge *tedge;
//   int scan = 0;
//   int frameNumber = 0;

//   active = ll_new();
//   tmplist = ll_new();

//   // get a pointer to the first item on the list and reset the current pointer
//   current = ll_head(edges);

//   // start at the first scanline and go until the active list is empty
//   for (scan = current->yStart; scan < src->rows; scan++)
//   {

//     // grab all edges starting on this row
//     while (current != NULL && current->yStart == scan)
//     {
//       ll_insert(active, current, compXIntersect);
//       current = ll_next(edges);
//     }
//     // current is either null, or the first edge to be handled on some future scanline

//     if (ll_empty(active))
//     {
//       break;
//     }

//     // if there are active edges, fill out the scanline
//     fillScan(scan, active, src, c);

//     // Save the current frame
//     // if(scan % 10 == 0)
//     //   save_frame(src, frameNumber++);

//     // remove any ending edges and update the rest
//     for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active))
//     {

//       // keep anything that's not ending
//       if (tedge->yEnd > scan)
//       {
//         float a = 1.0;

//         // update the edge information with the dPerScan values
//         tedge->xIntersect += tedge->dxPerScan;

//         // adjust in the case of partial overlap
//         if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1)
//         {
//           a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
//           tedge->xIntersect = tedge->x1;
//         }
//         else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1)
//         {
//           a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
//           tedge->xIntersect = tedge->x1;
//         }

//         ll_insert(tmplist, tedge, compXIntersect);
//       }
//     }

//     transfer = active;
//     active = tmplist;
//     tmplist = transfer;
//   }

//   // get rid of the lists, but not the edge records
//   ll_delete(active, NULL);
//   ll_delete(tmplist, NULL);

//   return (0);
// }

// Draw a polygon using color c without a fill
void polygon_draw(Polygon *p, Image *src, Color color)
{
  if (p->nVertex < 2)
  {
    // Not enough vertices to form a polygon
    return;
  }

  // Draw edges of the polygon
  for (int i = 0; i < p->nVertex; i++)
  {
    Line l;
    int j = (i + 1) % p->nVertex; // Wrap around to the first vertex
    double x0 = p->vertex[i].val[0];
    double y0 = p->vertex[i].val[1];
    double x1 = p->vertex[j].val[0];
    double y1 = p->vertex[j].val[1];
    line_set2D(&l, x0, y0, x1, y1);
    line_draw(&l, src, color);
  }
}

// Draw a filled polygon using color c with the scanline z-buffer rendering algorithm
void polygon_drawFill(Polygon *p, Image *src, Color c)
{
  // LinkedList *edges = NULL;

  // // set up the edge list
  // edges = setupEdgeList(p, src);
  // if (!edges)
  //   return;

  // // process the edge list (should be able to take an arbitrary edge list)
  // processEdgeList(edges, src, c);

  // // clean up
  // ll_delete(edges, (void (*)(const void *))free);

  polygon_drawFillAA(p, src, c);

  return;
}

/****************************************
End Scanline Fill
*****************************************/

/********************
Barycentric Fill Algorithm
********************/

// Function to compute the barycentric coordinates
static void barycentric(Point p, Point a, Point b, Point c, float *u, float *v, float *w)
{
  float denom = (b.val[1] - c.val[1]) * (a.val[0] - c.val[0]) + (c.val[0] - b.val[0]) * (a.val[1] - c.val[1]);
  *u = ((b.val[1] - c.val[1]) * (p.val[0] - c.val[0]) + (c.val[0] - b.val[0]) * (p.val[1] - c.val[1])) / denom;
  *v = ((c.val[1] - a.val[1]) * (p.val[0] - c.val[0]) + (a.val[0] - c.val[0]) * (p.val[1] - c.val[1])) / denom;
  *w = 1.0f - *u - *v;
}

// Draw a filled polygon using color c with the scanline z-buffer rendering algorithm (alternative method)
void polygon_drawFillB(Polygon *p, Image *src, Color c)
{
  if (!p || !src || p->nVertex < 3)
    return; // Check for invalid input

  // Iterate over each triangle in the polygon
  for (int i = 1; i < p->nVertex - 1; i++)
  {
    Point pA = p->vertex[0];
    Point pB = p->vertex[i];
    Point pC = p->vertex[i + 1];

    // Bounding box for the triangle
    int minX = (int)fmin(fmin(pA.val[0], pB.val[0]), pC.val[0]);
    int maxX = (int)fmax(fmax(pA.val[0], pB.val[0]), pC.val[0]);
    int minY = (int)fmin(fmin(pA.val[1], pB.val[1]), pC.val[1]);
    int maxY = (int)fmax(fmax(pA.val[1], pB.val[1]), pC.val[1]);

    // Clip to image boundaries
    if (minX < 0)
      minX = 0;
    if (maxX >= src->cols)
      maxX = src->cols - 1;
    if (minY < 0)
      minY = 0;
    if (maxY >= src->rows)
      maxY = src->rows - 1;

    // Iterate over each pixel in the bounding box
    for (int y = minY; y <= maxY; y++)
    {
      for (int x = minX; x <= maxX; x++)
      {
        Point p;
        point_set2D(&p, x, y);

        float u, v, w;
        barycentric(p, pA, pB, pC, &u, &v, &w);

        // If the point is inside the triangle, color it
        if (u >= 0 && v >= 0 && w >= 0)
        {
          image_setColor(src, y, x, c);
        }
      }
    }
  }
}

/****************************************
End Barycentric Fill
*****************************************/

/********************
Anti-Aliasing Fill Algorithm
********************/

// Helper function to blend colors based on coverage
static Color color_blend(Color c1, Color c2, float factor)
{
  Color result;
  result.c[0] = c1.c[0] * (1.0 - factor) + c2.c[0] * factor;
  result.c[1] = c1.c[1] * (1.0 - factor) + c2.c[1] * factor;
  result.c[2] = c1.c[2] * (1.0 - factor) + c2.c[2] * factor;
  return result;
}

// Check if a point is inside a polygon (using the ray-casting algorithm)
static int pointInPolygon(Polygon *p, float x, float y)
{
  int inside = 0;
  for (int i = 0, j = p->nVertex - 1; i < p->nVertex; j = i++)
  {
    float xi = p->vertex[i].val[0], yi = p->vertex[i].val[1];
    float xj = p->vertex[j].val[0], yj = p->vertex[j].val[1];

    int intersect = ((yi > y) != (yj > y)) &&
                    (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
    if (intersect)
      inside = !inside;
  }
  return inside;
}

// Function to compute coverage of a pixel using super-sampling
static float computeCoverage(Polygon *p, float x, float y)
{
  float coverage = 0.0;
  for (int sy = 0; sy < SUPER_SAMPLING_RATE; sy++)
  {
    for (int sx = 0; sx < SUPER_SAMPLING_RATE; sx++)
    {
      float sampleX = x + (sx + 0.5f) / SUPER_SAMPLING_RATE;
      float sampleY = y + (sy + 0.5f) / SUPER_SAMPLING_RATE;

      if (pointInPolygon(p, sampleX, sampleY))
      {
        coverage += 1.0;
      }
    }
  }
  return coverage / (SUPER_SAMPLING_RATE * SUPER_SAMPLING_RATE);
}

// Anti-aliased filled polygon drawing using super-sampling
void polygon_drawFillAA(Polygon *p, Image *src, Color c)
{
  if (!p || !src || p->nVertex < 3)
    return; // Check for invalid input

  int xMin = src->cols, xMax = 0, yMin = src->rows, yMax = 0;
  for (int i = 0; i < p->nVertex; i++)
  {
    if (p->vertex[i].val[0] < xMin)
      xMin = p->vertex[i].val[0];
    if (p->vertex[i].val[0] > xMax)
      xMax = p->vertex[i].val[0];
    if (p->vertex[i].val[1] < yMin)
      yMin = p->vertex[i].val[1];
    if (p->vertex[i].val[1] > yMax)
      yMax = p->vertex[i].val[1];
  }

  // Super-sampling and blending
  for (int y = yMin; y <= yMax; y++)
  {
    for (int x = xMin; x <= xMax; x++)
    {
      float coverage = computeCoverage(p, x, y);
      if (coverage > 0.0)
      {
        Color blended = color_blend(image_getColor(src, y, x), c, coverage);
        image_setColor(src, y, x, blended);
      }
    }
  }
}

/****************************************
End Anti-Aliasing Fill
*****************************************/