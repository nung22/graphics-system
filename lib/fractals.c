// These functions generate fractal images (Mandelbrot and Julia sets) and Perlin noise.
// Written by Nicholas Ung 2024-05-20

#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include "fractals.h"

/*
 * Function: getColor
 * ------------------
 * Maps the number of iterations to an RGB color.
 *
 * iterations: The number of iterations at a point.
 * maxIterations: The maximum number of iterations.
 *
 * Returns: An FPixel containing the color.
 */
FPixel static getColor(int iterations, int maxIterations)
{
  FPixel color;
  float t = (float)iterations / maxIterations;

  if (iterations == maxIterations)
  {
    // Point is inside the set (use black)
    color.rgb[0] = 0;
    color.rgb[1] = 0;
    color.rgb[2] = 0;

    // Point is inside the set (use skyblue)
    // color.rgb[0] = .4;
    // color.rgb[1] = .6;
    // color.rgb[2] = .8;
  }
  else
  {
    // Point is outside the set
    
    // Smooth transition from black to skyblue
    // color.rgb[0] = t * .4;
    // color.rgb[1] = t * .6;
    // color.rgb[2] = t * .8;

    // Smooth transition from blue to red
    color.rgb[0] = 9 * (1 - t) * t * t * t;
    color.rgb[1] = 15 * (1 - t) * (1 - t) * t * t;
    color.rgb[2] = 8.5 * (1 - t) * (1 - t) * (1 - t) * t;
  }

  return color;
}

/*
 * Function: mandelbrot
 * --------------------
 * Generates a Mandelbrot set and fills the given image.
 *
 * dst: The image to fill.
 * x0, y0: The coordinates of the top-left corner of the rectangle in the complex plane.
 * dx: The width of the rectangle in the complex plane.
 *
 * Returns: void
 */
void mandelbrot(Image *dst, float x0, float y0, float dx)
{
  // Flip the y-axis to match the image coordinate system
  x0 *= -1;
  y0 *= -1;
  dx *= -1;

  int cols = dst->cols;        // Get the number of columns (width) of the image
  int rows = dst->rows;        // Get the number of rows (height) of the image
  float dy = dx * rows / cols; // Calculate the height of the rectangle in the complex plane
  int maxIterations = 128;     // Maximum number of iterations to determine set membership

  // Iterate over each pixel in the image
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      // Calculate the complex plane coordinates
      float x = x0 + j * dx / cols;
      float y = (dy + y0) - i * dy / rows;
      float complex c = x + y * I; // Create the complex number c
      float complex z = 0;         // Initialize z to 0
      int k = 0;

      // Iterate to determine if the point is in the Mandelbrot set
      for (k = 0; k < maxIterations; k++)
      {
        if (cabs(z) > 2.0)
          break;
        z = z * z + c;
      }

      // Determine the color
      FPixel color = getColor(k, maxIterations);

      image_setf(dst, i, j, color); // Set the pixel color in the image
    }
  }
}

/*
 * Function: julia
 * ---------------
 * Generates a Julia set and fills the given image.
 *
 * dst: The image to fill.
 * x0, y0: The coordinates of the top-left corner of the rectangle in the complex plane.
 * dx: The width of the rectangle in the complex plane.
 *
 * Returns: void
 */
void julia(Image *dst, float x0, float y0, float dx)
{
  // Flip the y-axis to match the image coordinate system
  x0 *= -1;
  y0 *= -1;
  dx *= -1;

  int cols = dst->cols;        // Get the number of columns (width) of the image
  int rows = dst->rows;        // Get the number of rows (height) of the image
  float dy = dx * rows / cols; // Calculate the height of the rectangle in the complex plane
  int maxIterations = 256;     // Maximum number of iterations to determine set membership

  float complex c = -0.7454054 + 0.1130063 * I; // Constant c, can be modified
  // float complex c = 0.285 + 0.01 * I; // Different constant c
  // float complex c = -0.4 + 0.6 * I; // Different constant c

  // Iterate over each pixel in the image
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      // Calculate the complex plane coordinates
      float x = x0 + j * dx / cols;
      float y = (dy + y0) - i * dy / rows;
      float complex z = x + y * I; // Initialize z to the complex plane coordinates
      int k = 0;

      // Iterate to determine if the point is in the Julia set
      for (k = 0; k < maxIterations; k++)
      {
        if (cabs(z) > 2.0)
          break;
        z = z * z + c;
      }

      // Determine the color
      FPixel color = getColor(k, maxIterations);

      image_setf(dst, i, j, color); // Set the pixel color in the image
    }
  }
}

// Permutation table for Perlin noise
static int perm[512];

// Gradient vectors for 3D Perlin noise
static const int grad3[12][3] = {
  {1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, 
  {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1}, 
  {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};

/*
 * Function: init_perlin
 * ---------------------
 * Initializes the permutation table for Perlin noise.
 *
 * seed: The seed for random number generation.
 *
 * Returns: void
 */
static void init_perlin(int seed)
{
  srand(seed); // Initialize random seed

  // Initialize the permutation table
  for (int i = 0; i < 256; i++)
    perm[i] = i;
  for (int i = 0; i < 256; i++)
  {
    int j = rand() % 256; // Generate a random index
    int swap = perm[i];   // Swap perm[i] and perm[j]
    perm[i] = perm[j];
    perm[j] = swap;
  }
  for (int i = 0; i < 256; i++)
    perm[256 + i] = perm[i]; // Extend the permutation table
}

/*
 * Function: dot
 * -------------
 * Computes the dot product of the gradient vector and the distance vector.
 *
 * g: The gradient vector.
 * x, y: The distance vector.
 *
 * Returns: The dot product.
 */
static float dot(const int *g, float x, float y)
{
  return g[0] * x + g[1] * y;
}

/*
 * Function: fade
 * --------------
 * Fade function to smooth the transition between grid points.
 *
 * t: The interpolation factor.
 *
 * Returns: The smoothed interpolation factor.
 */
static float fade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

/*
 * Function: lerp
 * --------------
 * Linear interpolation between two values.
 *
 * t: The interpolation factor.
 * a, b: The values to interpolate between.
 *
 * Returns: The interpolated value.
 */
static float lerp(float t, float a, float b)
{
  return a + t * (b - a);
}

/*
 * Function: noise
 * ---------------
 * Generates Perlin noise value for the given coordinates.
 *
 * x, y: The coordinates.
 *
 * Returns: The Perlin noise value.
 */
static float noise(float x, float y)
{
  // Calculate grid cell coordinates
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;

  // Calculate relative x, y coordinates in grid cell
  x -= floor(x);
  y -= floor(y);

  // Compute fade curves for x, y
  float u = fade(x);
  float v = fade(y);

  // Hash coordinates of the 4 square corners
  int A = perm[X] + Y, AA = perm[A], AB = perm[A + 1];
  int B = perm[X + 1] + Y, BA = perm[B], BB = perm[B + 1];

  // Add blended results from 4 corners of the square
  return lerp(v, lerp(u, dot(grad3[perm[AA] % 12], x, y), dot(grad3[perm[BA] % 12], x - 1, y)),
              lerp(u, dot(grad3[perm[AB] % 12], x, y - 1), dot(grad3[perm[BB] % 12], x - 1, y - 1)));
}

/*
 * Function: perlin_noise
 * ----------------------
 * Fills an image with Perlin noise values.
 *
 * img: The image to fill with noise.
 * seed: The seed for random number generation.
 * scale: The scale of the noise pattern.
 *
 * Returns: void
 */
void perlin_noise(Image *img, int seed, float scale)
{
  init_perlin(seed); // Initialize the permutation table with the seed

  // Loop over each pixel in the image
  for (int i = 0; i < img->rows; i++)
  {
    for (int j = 0; j < img->cols; j++)
    {
      float x = j / scale;          // Scale x coordinate
      float y = i / scale;          // Scale y coordinate
      float n = noise(x, y);        // Generate noise value
      FPixel color = {n, n, n};     // Create grayscale color from noise value
      image_setf(img, i, j, color); // Set the pixel color
    }
  }
}
