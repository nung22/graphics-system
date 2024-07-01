#ifndef DRAWSTATE_H

#define DRAWSTATE_H

#include "color.h"
#include "point.h"
#include <stdbool.h>

// Enumerated type for shading method
typedef enum
{
  ShadeFrame,
  ShadeConstant,
  ShadeDepth,
  ShadeFlat,
  ShadeGouraud,
  ShadePhong
} ShadeMethod;

// Structure to specify how an object is drawn into the image
typedef struct
{
  Color color;
  Color flatColor;
  Color body;
  Color surface;
  float surfaceCoeff;
  ShadeMethod shade;
  bool zBufferFlag;
  Point viewer;
} DrawState;

/* Function prototypes for DrawState operations */
DrawState *drawstate_create(void);
void drawstate_setColor(DrawState *s, Color c);
void drawstate_setBody(DrawState *s, Color c);
void drawstate_setSurface(DrawState *s, Color c);
void drawstate_setSurfaceCoeff(DrawState *s, float f);
void drawstate_copy(DrawState *to, DrawState *from);

#endif // DRAWSTATE_H