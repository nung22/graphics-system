// These functions provide methods for manipulating drawstates.
// Written by Nicholas Ung 2024-06-27

#include <stdlib.h>
#include "drawstate.h"

// Create a new DrawState object
DrawState *drawstate_create(void) {
  DrawState* state = (DrawState*)malloc(sizeof(DrawState));
  Color defaultColor;
  color_set(&defaultColor, 1.0, 1.0, 1.0); // Default color is white
  Point defaultViewer = {0.0, 0.0, 0.0};
  state->color = defaultColor;
  state->flatColor = defaultColor;
  state->body = defaultColor;
  state->surface = defaultColor;
  state->surfaceCoeff = 32.0f;
  state->shade = ShadeConstant;
  state->zBufferFlag = 0;
  state->viewer = defaultViewer;
  return state;
}

// Initialize a DrawState object
int drawstate_init(DrawState* s) {
  if (s == NULL) {
    return -1;
  }
  s = drawstate_create();
  return 0;
}

// Set the color of a DrawState object
void drawstate_setColor(DrawState* s, Color c) {
  color_copy(&s->color, &c);
}

// Set the body color of a DrawState object
void drawstate_setBody(DrawState* s, Color c) {
  color_copy(&s->body, &c);
}

// Set the surface color of a DrawState object
void drawstate_setSurface(DrawState* s, Color c) {
  color_copy(&s->surface, &c);
}

// Set the surface coefficient of a DrawState object
void drawstate_setSurfaceCoeff(DrawState* s, float f) {
  s->surfaceCoeff = f;
}

// Copy the values from one DrawState object to another
void drawstate_copy(DrawState* to, DrawState* from) {
  color_copy(&to->color, &from->color);
  color_copy(&to->flatColor, &from->flatColor);
  color_copy(&to->body, &from->body);
  color_copy(&to->surface, &from->surface);
  to->surfaceCoeff = from->surfaceCoeff;
  to->shade = from->shade;
  to->zBufferFlag = from->zBufferFlag;
  point_copy(&to->viewer, &from->viewer);
}

// Set the viewer of a DrawState object
void drawstate_setViewer(DrawState* s, Point* p) {
  s->viewer = *p;
}

// Set the shading method of a DrawState object
void drawstate_setShading(DrawState* s, ShadeMethod m) {
  s->shade = m;
}