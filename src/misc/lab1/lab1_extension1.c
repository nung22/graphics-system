/*
  This program reads an input image, flips it horizontally, 
  and writes the output image.

  Nicholas K. Ung updated 2024-05-16
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  Pixel *image, *output;
  int rows, cols, colors;
  long imagesize;
  long i, j;

  if (argc < 3)
  {
    printf("Usage: lab1_extension1 <input file> <output file>\n");
    exit(-1);
  }

  // Read in the image
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if (!image)
  {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  // Calculate the image size
  imagesize = (long)rows * (long)cols;

  // Allocate memory for the output
  output = (Pixel *)malloc(imagesize * sizeof(Pixel));
  if (!output)
  {
    fprintf(stderr, "Unable to allocate memory for output\n");
    free(image);
    exit(-1);
  }


  // Create the output
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      output[i * cols + j] = image[i * cols + (cols - 1 - j)];
    }
  }

  // Write out the resulting output
  writePPM(output, rows, cols, 255, argv[2]);

  // Free the image memory
#if USECPP
  delete[] image;
  delete[] output;
#else
  free(image);
  free(output);
#endif

  return (0);
}