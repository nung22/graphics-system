// These functions provide methods for manipulating lighting.
// Written by Nicholas Ung 2024-07-22

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lighting.h"

// Create a new Lighting object
Lighting *lighting_create(void)
{
  Lighting *l = (Lighting *)malloc(sizeof(Lighting));
  lighting_init(l);
  return l;
}

// Delete a Lighting object
void lighting_delete(Lighting *lights)
{
  if (lights)
  {
    free(lights);
  }
}

// Initialize a Lighting object
void lighting_init(Lighting *l)
{
  l->nLights = 0;
  memcpy(l->light, malloc(MAX_LIGHTS * sizeof(Light)), MAX_LIGHTS * sizeof(Light));
}

// Reset a lighting structure to 0 lights
void lighting_clear(Lighting *l)
{
  l->nLights = 0;
}

// Add a light to a lighting structure
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness)
{
  if (l->nLights < MAX_LIGHTS)
  {
    Light *light = &l->light[l->nLights++];
    light->type = type;
    light->color = *c;
    if (dir)
    {
      light->direction = *dir;
    }
    if (pos)
    {
      light->position = *pos;
    }
    light->cutoff = cutoff;
    light->sharpness = sharpness;
  }
}

// Compute the shading of a point given the parameters and put the result in c
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *P, Color *Cb, Color *Cs, float s, int oneSided, Color *c)
{
  // Initialize the final color to black
  color_set(c, 0.0f, 0.0f, 0.0f);
  vector_normalize(N);
  vector_normalize(V);

  // Iterate through each light
  for (int i = 0; i < l->nLights; i++)
  {
    Light *light = &l->light[i];
    Color intensity;
    color_set(&intensity, 0.0, 0.0, 0.0);

    Vector L, H;
    float theta, beta, sigma;

    switch (light->type)
    {
    case LightAmbient:
      // Ambient light contribution
      color_copy(&intensity, Cb);
      for (int i = 0; i < 3; i++)
      {
        intensity.c[i] *= light->color.c[i];
        c->c[i] += intensity.c[i];
      }

      // Print ambient light contribution
      printf("ambient light %d: %.2f %.2f %.2f\n", i, intensity.c[0], intensity.c[1], intensity.c[2]);
      break;

    case LightDirect:
      // Directional light
      L = light->direction;
      vector_normalize(&L);

      theta = vector_dot(N, &L);
      if (theta > 0 || !oneSided)
      {
        vector_add(&L, V, &H);
        vector_normalize(&H);
        beta = vector_dot(N, &H);
        beta = pow(beta, s);

        // Diffuse and specular components
        color_set(&intensity, Cb->c[0] * theta + Cs->c[0] * beta,
                  Cb->c[1] * theta + Cs->c[1] * beta,
                  Cb->c[2] * theta + Cs->c[2] * beta);
        color_set(&intensity, intensity.c[0] * light->color.c[0],
                  intensity.c[1] * light->color.c[1],
                  intensity.c[2] * light->color.c[2]);
        for (int i = 0; i < 3; i++)
        {
          c->c[i] += intensity.c[i];
        }
      }
      break;

    case LightPoint:
      // Point light
      vector_subtract(&light->position, P, &L);
      vector_normalize(&L);

      theta = vector_dot(N, &L);
      // Continue if the polygon is one-sided or theta is negative,
      if (theta < 0 && oneSided)
      {
        continue;
      }

      // Compute sigma
      sigma = vector_dot(V, N);

      // Continue if viewer is on the same side as the light source
      if ((theta < 0 && sigma > 0) || (theta > 0 && sigma < 0))
      {
        continue;
      }

      // Calculate H
      vector_add(&L, V, &H);
      vector_set(&H, H.val[0] / 2, H.val[1] / 2, H.val[2] / 2);
      vector_normalize(&H);

      // Calculate beta
      beta = vector_dot(N, &H);
      fprintf(stderr, "beta: %.2f\n", beta);


      // Invert the light source if theta is negative and not one-sided
      if (theta < 0 && !oneSided)
      {
        beta *= -1;
        theta *= -1;
      }
      beta = pow(beta, s); 

      // Diffuse and specular components
      for (int i = 0; i < 3; i++)
      {
        float body = Cb->c[i] * light->color.c[i] * theta;
        float surface = Cs->c[i] * light->color.c[i] * beta;
        intensity.c[i] += body + surface;
        c->c[i] += intensity.c[i];

      }

      break;

    case LightSpot:
      // Spot light
      vector_subtract(&light->position, P, &L);
      vector_normalize(&L);
      theta = vector_dot(N, &L);
      float spotEffect = vector_dot(&light->direction, &L);
      if ((theta > 0 || !oneSided) && spotEffect > light->cutoff)
      {
        vector_add(&L, V, &H);
        vector_normalize(&H);
        beta = vector_dot(N, &H);
        beta = (beta < 0) ? 0 : pow(beta, s);

        // Diffuse and specular components
        color_set(&intensity, Cb->c[0] * theta + Cs->c[0] * beta,
                  Cb->c[1] * theta + Cs->c[1] * beta,
                  Cb->c[2] * theta + Cs->c[2] * beta);

        // Apply spotlight effect
        float attenuation = pow(spotEffect, light->sharpness);
        for (int i = 0; i < 3; i++)
        {
          intensity.c[i] *= light->color.c[i] * attenuation;
          c->c[i] += intensity.c[i];
        }
        break;
      }
    default:
      // Skip if unknown light type
      continue;
    }
  }

  // Clip the final color to the range [0, 1]
  for (int i = 0; i < 3; i++)
  {
    c->c[i] = fmin(fmax(c->c[i], 0.0), 1.0);
  }
}
