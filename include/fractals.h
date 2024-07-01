#ifndef FRACTALS_H

#define FRACTALS_H

#include "image.h"

// Generate a Mandelbrot set
void mandelbrot(Image *dst, float x0, float y0, float dx);

// Generate a Julia set
void julia(Image *dst, float x0, float y0, float dx);

// Generate Perlin noise
void perlin_noise(Image *img, int seed, float scale);

#endif // FRACTALS_H
