/*
  This program reads an input image, sets pixels of the
  blue/green background to black & those of foreground 
  to white, and writes the output mask image.

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
  Pixel *image, *mask;
  int rows, cols, colors;
  long imagesize;
  long i, j;
  char background[10];

  if (argc < 3)
  {
    printf("Usage: lab1_2 <background color> <input file> <output mask file>\n");
    exit(-1);
  }

  // Read in the image
  image = readPPM(&rows, &cols, &colors, argv[2]);
  if (!image)
  {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    exit(-1);
  }

  // Calculate the image size
  imagesize = (long)rows * (long)cols;

  // Allocate memory for the mask
  mask = (Pixel *)malloc(imagesize * sizeof(Pixel));
  if (!mask)
  {
    fprintf(stderr, "Unable to allocate memory for mask\n");
    free(image);
    exit(-1);
  }

  // Standardize background color argument
  strcpy(background, argv[1]);
  for (int i = 0; i < strlen(background); i++)
  {
    background[i] = tolower(background[i]);
  }
  // Close program if background color is not green or blue
  if (strcmp("green", background) != 0 && strcmp("blue", background) != 0)
  {
    fprintf(stderr, "Invalid background color\n");
    free(image);
    free(mask);
    exit(-1);
  }

  // Create the mask
  for (i = 0; i < imagesize; i++)
  {

    // Threshold for very green/blue pixels depending on background color
    if (strcmp("green", background) == 0)
    {
      j = (int)image[i].g - ((int)image[i].r + (int)image[i].b) / 2;
    }
    else
    {
      j = (int)image[i].b - ((int)image[i].r + (int)image[i].g) / 2;
    }

    if (j > 40)
    {
      mask[i].r = mask[i].g = mask[i].b = 0; // background (black)
    }
    else
    {
      mask[i].r = mask[i].g = mask[i].b = 255; // foreground (white)
    }
  }

  // Write out the resulting mask
  writePPM(mask, rows, cols, 255, argv[3]);

  // Free the image memory
#if USECPP
  delete[] image;
  delete[] mask;
#else
  free(image);
  free(mask);
#endif

  return (0);
}