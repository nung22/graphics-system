#ifndef LIGHTING_H

#define LIGHTING_H

#include "vector.h"

#define MAX_LIGHTS 64

// Enumerated type for light types
typedef enum
{
  LightNone,
  LightAmbient,
  LightDirect,
  LightPoint,
  LightSpot
} LightType;

// Structure to represent a light
typedef struct
{
  LightType type;
  Color color;
  Vector direction;
  Point position;
  float cutoff;    // stores the cosine of the cutoff angle of a spotlight
  float sharpness; // coefficient of the falloff function (power for cosine)
} Light;

// Structure to represent lighting
typedef struct
{
  int nLights;
  Light light[MAX_LIGHTS];
} Lighting;

/* Function prototypes for lighting operations */
Lighting *lighting_create(void);                                                                                           // creates a new lighting structure
void lighting_delete(Lighting *lights);                                                                                    // deletes a lighting structure
void lighting_init(Lighting *l);                                                                                           // initializes a lighting structure
void lighting_clear(Lighting *l);                                                                                          // reset a lighting structure to 0 lights
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness);          // adds a light to a lighting structure
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *P, Color *Cb, Color *CS, float s, int oneSided, Color *c); // computes the shading of a point given the parameters and put the result in c

#endif // LIGHTING_H