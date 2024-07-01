/*
  This program reads an input image and an overlay image (e.g., fog or explosion),
  applies a random transparency mask with values between 30 and 50, and writes
  the output image.

  Nicholas K. Ung updated 2024-05-16
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  Pixel *input, *overlay, *output;
  int rows1, cols1, colors1;
  int rows2, cols2, colors2;

  if (argc < 4)
  {
    printf("Usage: lab1_extension2 <input file> <overlay file> <output file>\n");
    return -1;
  }

  srand(time(NULL)); // Seed the random number generator

  // Read the images
  input = readPPM(&rows1, &cols1, &colors1, argv[1]);
  if (!input)
  {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    return -1;
  }

  overlay = readPPM(&rows2, &cols2, &colors2, argv[2]);
  if (!overlay)
  {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    free(input);
    return -1;
  }

  if (rows1 != rows2 || cols1 != cols2)
  {
    fprintf(stderr, "The dimensions of the input file and the overlay file must match.\n");
    free(input);
    free(overlay);
    return -1;
  }

  // Create the output image
  output = (Pixel *)malloc(rows1 * cols1 * sizeof(Pixel));
  if (!output)
  {
    fprintf(stderr, "Unable to allocate memory for the output image\n");
    free(input);
    free(overlay);
    return -1;
  }

  // Apply the random transparency mask and blend the images
  for (int i = 0; i < rows1; i++)
  {
    for (int j = 0; j < cols1; j++)
    {
      int idx = i * cols1 + j;
      // Generate a random mask value between 30 and 50
      unsigned char random_mask_value = (unsigned char)(rand() % 21 + 30);
      // Interpolate mask value from 0-255 to 0.0-1.0
      double alpha = random_mask_value / 255.0;

      output[idx].r = (unsigned char)(alpha * overlay[idx].r + (1 - alpha) * input[idx].r);
      output[idx].g = (unsigned char)(alpha * overlay[idx].g + (1 - alpha) * input[idx].g);
      output[idx].b = (unsigned char)(alpha * overlay[idx].b + (1 - alpha) * input[idx].b);
    }
  }

  // Write the output image
  writePPM(output, rows1, cols1, 255, argv[3]);

  // Free the image memory
#if USECPP
  delete[] input;
  delete[] overlay;
  delete[] output;
#else
  free(input);
  free(overlay);
  free(output);
#endif

  return 0;
}
