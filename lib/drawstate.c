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
  state->surfaceCoeff = 0.0f;
  state->shade = ShadeConstant;
  state->zBufferFlag = 0;
  state->viewer = defaultViewer;
  return state;
}

// Set the color of a DrawState object
void drawstate_setColor(DrawState* s, Color c) {
  s->color = c;
}

// Set the body color of a DrawState object
void drawstate_setBody(DrawState* s, Color c) {
  s->body = c;
}

// Set the surface color of a DrawState object
void drawstate_setSurface(DrawState* s, Color c) {
  s->surface = c;
}

// Set the surface coefficient of a DrawState object
void drawstate_setSurfaceCoeff(DrawState* s, float f) {
  s->surfaceCoeff = f;
}

// Copy the values from one DrawState object to another
void drawstate_copy(DrawState* to, DrawState* from) {
  to->color = from->color;
  to->flatColor = from->flatColor;
  to->body = from->body;
  to->surface = from->surface;
  to->surfaceCoeff = from->surfaceCoeff;
  to->shade = from->shade;
  to->zBufferFlag = from->zBufferFlag;
  to->viewer = from->viewer;
}