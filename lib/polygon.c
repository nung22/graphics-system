// These functions provide methods for creating and manipulating polygons.
// Written by Nicholas Ung 2024-06-04

#include "polygon.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
  p->zBuffer = 1;
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

/********************
Scanline Fill Algorithm
********************/

// Structure for storing edge information
typedef struct tEdge {
  float x0, y0;                // Start point coordinates
  float x1, y1;                // End point coordinates
  float z0, z1;                // Z values at start and end points
  int yStart, yEnd;            // Start and end row for the edge
  float xIntersect, dxPerScan; // X intersection and its change per scanline
  float zIntersect, dzPerScan; // Z intersection and its change per scanline
  struct tEdge *next;          // Pointer to the next edge
} Edge;

// Compare edges by yStart for sorting
static int compYStart(const void *a, const void *b) {
  Edge *ea = (Edge *)a;
  Edge *eb = (Edge *)b;
  return (ea->yStart - eb->yStart);
}

// Compare edges by xIntersect for sorting
static int compXIntersect(const void *a, const void *b) {
  Edge *ea = (Edge *)a;
  Edge *eb = (Edge *)b;
  if (ea->xIntersect < eb->xIntersect) return -1;
  else if (ea->xIntersect > eb->xIntersect) return 1;
  return 0;
}

// Create an edge structure from start to end points, considering depth and clipping
static Edge *makeEdgeRec(Point start, Point end, Image *src, DrawState *ds) {
    Edge *edge;
    float dscan = end.val[1] - start.val[1];

    // Clip vertices to the vertical bounds of the image
    // BAM never modify the start and end values, only modify the edge rec values
    // BAM you lose the ability to properly clip the upper plane if you do this
    // BAM ditch these four lines
    // if (start.val[1] < 0) start.val[1] = 0;
    // if (end.val[1] < 0) end.val[1] = 0;
    // if (start.val[1] >= src->rows) start.val[1] = src->rows - 1;
    // if (end.val[1] >= src->rows) end.val[1] = src->rows - 1;

    // BAM you can check for lines that end < 0 and lines that start >= src->rows and return NULL

    // If both vertices are outside the vertical bounds, skip this edge
    if (start.val[1] == end.val[1] || start.val[1] < 0 || end.val[1] < 0 || start.val[1] >= src->rows || end.val[1] >= src->rows) {
        return NULL;
    }

    edge = (Edge *)malloc(sizeof(Edge));
    edge->x0 = start.val[0];
    edge->y0 = start.val[1];
    edge->x1 = end.val[0];
    edge->y1 = end.val[1];
    edge->z0 = 1.0 / start.val[2];
    edge->z1 = 1.0 / end.val[2];
    edge->yStart = (int)(start.val[1] + 0.5);
    edge->yEnd = (int)(end.val[1] + 0.5) - 1;

    if (edge->yEnd >= src->rows) {
        edge->yEnd = src->rows - 1;
    }

    edge->dxPerScan = (end.val[0] - start.val[0]) / dscan;
    edge->dzPerScan = (edge->z1 - edge->z0) / dscan;
    edge->xIntersect = edge->x0 + edge->dxPerScan * ((float)(edge->yStart) + 0.5 - edge->y0);
    edge->zIntersect = edge->z0 + edge->dzPerScan * ((float)(edge->yStart) + 0.5 - edge->y0);

    if (edge->y0 < 0) { // BAM this would never execute given the initial clipping, but should work once they're gone
        edge->xIntersect += edge->dxPerScan * ((float)(-edge->y0));
        edge->zIntersect += edge->dzPerScan * ((float)(-edge->y0));
        edge->y0 = 0;
    }


    // checking for very bad edges is more subtle.  If dxPerScan is positive and xIntersect > edge x1, then set xIntersect to edge->x1.  if dxPerScan is negative and xIntersect < edge x1, then set xIntersect to edge x1.
    if (edge->dxPerScan > 0.0 && edge->xIntersect > edge->x1) {
        edge->xIntersect = edge->x1;
        edge->zIntersect = edge->z1;
    } else if (edge->dxPerScan < 0.0 && edge->xIntersect < edge->x1) {
        edge->xIntersect = edge->x1;
        edge->zIntersect = edge->z1;
    }

    return edge;
}

// Create a sorted edge list from the polygon vertices
static LinkedList *setupEdgeList(Polygon *p, Image *src, DrawState *ds) {
  LinkedList *edges = ll_new();   // Create new linked list for edges
  Point v1, v2;
  int i;

  v1 = p->vertex[p->nVertex - 1]; // Start with the last vertex

  for (i = 0; i < p->nVertex; i++) {
    v2 = p->vertex[i]; // Get current vertex

    // Clip vertices to image vertical bounds
    if (v1.val[1] < 0 && v2.val[1] >= 0) v1.val[1] = 0;
    if (v1.val[1] >= 0 && v2.val[1] < 0) v2.val[1] = 0;
    if (v1.val[1] >= src->rows && v2.val[1] < src->rows) v1.val[1] = src->rows - 1;
    if (v1.val[1] < src->rows && v2.val[1] >= src->rows) v2.val[1] = src->rows - 1;

    // Create edge if not horizontal
    if ((int)(v1.val[1] + 0.5) != (int)(v2.val[1] + 0.5)) {
      Edge *edge;
      if (v1.val[1] < v2.val[1]) edge = makeEdgeRec(v1, v2, src, ds);
      else edge = makeEdgeRec(v2, v1, src, ds);
      if (edge) ll_insert(edges, edge, compYStart);
    }

    v1 = v2; // Move to the next vertex
  }

  // Delete empty edge list
  if (ll_empty(edges)) {
    ll_delete(edges, NULL);
    edges = NULL;
  }

  return edges; // Return the edge list
}

// Draw one scanline of a polygon
static void fillScan(int scan, LinkedList *active, Image *src, DrawState *ds, Lighting *lighting) {
  Edge *p1, *p2;
  int i, f;

  p1 = ll_head(active);

  while (p1) {
    p2 = ll_next(active);

    if (!p2) {
      printf("Edges not in pairs\n");
      break;
    }

    if (p2->xIntersect == p1->xIntersect) {
      p1 = ll_next(active);
      continue;
    }

    i = (int)(p1->xIntersect);
    if (i < 0) i = 0;

    f = (int)(p2->xIntersect);
    if (f >= src->cols) f = src->cols - 1;

    float curZ = p1->zIntersect;
    float dzPerColumn = (p2->zIntersect - p1->zIntersect) / (p2->xIntersect - p1->xIntersect);

    if ((int)(p1->xIntersect - 0.5) < 0) {
      curZ += dzPerColumn * (-p1->xIntersect);
    }

    for (; i <= f; i++) {
      float avgZ = (p1->zIntersect + p2->zIntersect) / 2;
      if (curZ > image_getz(src, scan, i) && curZ - 0.0001 * avgZ > image_getz(src, scan, i) && curZ < 1000) {
        image_setz(src, scan, i, curZ);

        Color blendedColor = ds->color;
        if (ds->shade == ShadeDepth) {
          float depthColorValue = 1.0 - (1.0 / curZ);
          blendedColor.c[0] *= depthColorValue;
          blendedColor.c[1] *= depthColorValue;
          blendedColor.c[2] *= depthColorValue;
        }

        image_setColor(src, scan, i, blendedColor);
      }
      curZ += dzPerColumn;
    }
    p1 = ll_next(active);
  }
}

// Update the active edge list for the next scanline
static void updateActiveList(LinkedList *active, int scan) {
  LinkedList *tmplist = ll_new();
  Edge *tedge;

  for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active)) {
    if (tedge->yEnd > scan) {
      tedge->xIntersect += tedge->dxPerScan;
      tedge->zIntersect += tedge->dzPerScan;
      if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1) {
        tedge->xIntersect = tedge->x1;
        tedge->zIntersect = 1.0 / tedge->z1;
      } else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1) {
        tedge->xIntersect = tedge->x1;
        tedge->zIntersect = 1.0 / tedge->z1;
      }
      ll_insert(tmplist, tedge, compXIntersect);
    }
  }

  while ((tedge = ll_pop(tmplist)) != NULL) {
    ll_insert(active, tedge, compXIntersect);
  }

  ll_delete(tmplist, NULL);
}

// Process the edge list and fill polygons using the scanline algorithm
static int processEdgeList(LinkedList *edges, Image *src, DrawState *ds, Lighting *lighting) {
  LinkedList *active = ll_new();
  Edge *current;
  int scan = 0;

  current = ll_head(edges);

  for (scan = current->yStart; scan < src->rows; scan++) {
    while (current != NULL && current->yStart == scan) {
      ll_insert(active, current, compXIntersect);
      current = ll_next(edges);
    }

    if (ll_empty(active)) {
      break;
    }

    fillScan(scan, active, src, ds, lighting);
    updateActiveList(active, scan);
  }

  ll_delete(active, NULL);
  return 0;
}

// Draw a filled polygon with shading using the scanline z-buffer algorithm
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *lighting) {
  LinkedList *edges = setupEdgeList(p, src, ds);
  if (!edges) return;
  processEdgeList(edges, src, ds, lighting);
  ll_delete(edges, (void (*)(const void *))free);
}

// Draw a filled polygon with constant shading
void polygon_drawFill(Polygon *p, Image *src, Color c) {
  DrawState *ds = drawstate_create();
  ds->shade = ShadeConstant;
  ds->color = c;

  Lighting *lighting = NULL;
  polygon_drawShade(p, src, ds, lighting);
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
