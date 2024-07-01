/*
  Creates an image of Perlin noise
*/
#include "image.h"
#include "fractals.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  Image *src;

  src = image_create(1080, 1920);

  // Generate Perlin noise

  // Create a Perlin noise image with default parameters
  // perlin_noise(src, 75, 25.0);
  // image_write(src, "perlin-noise.ppm");

  // Create several images with different parameters
  for (int i = 1; i < 5; i++)
  {
    perlin_noise(src, i, 4.0);
    char filename[20];
    sprintf(filename, "portfolio2_%d.ppm", i);
    image_write(src, filename);
  }

  image_free(src);

  return 0;
}
