/*
  This program reads and writes an image using the ppmIO.c
  routines. It outputs an image that highlights very green
  pixels of the original image and turns the rest greyscale.

  Nicholas K. Ung updated 2024-05-16
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i, j;
  int min;

  if (argc < 3)
  {
    printf("Usage: lab1_1 <input file> <output file>\n");
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

  // Modify the image
  for (i = 0; i < imagesize; i++)
  {

    // This segment thresholds out very green pixels so the resulting 
    // image will be very green where there is green and greyscale elsewhere
    j = (int)image[i].g - ((int)image[i].r + (int)image[i].b) / 2;
    min = image[i].r < image[i].b ? image[i].r : image[i].b;
    min = image[i].g < min ? image[i].g : min;
    min = min < 128 ? min : min / 2;
    if (j > 10)
    {
      image[i].g = image[i].g < 128 ? image[i].g * 2 : image[i].g;
    }
    else
    {
      image[i].g = min;
    }
    image[i].r = image[i].b = min;
  }

  // Write out the resulting image
  writePPM(image, rows, cols, colors, argv[2]);

  // Free the image memory
  #if USECPP
    delete[] image;
  #else
    free(image);
  #endif

  return (0);
}
