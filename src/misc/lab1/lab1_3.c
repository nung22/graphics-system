/*
  This program takes in two source images (the green/blue screen image and a background image)
  and a mask image and merges the two images using the mask as an alpha mask.
  It also takes in two offsets, dx and dy, to position the green/blue screen image on the background.

  Nicholas K. Ung updated 2024-05-16
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  Pixel *image1, *image2, *mask, *output;
  int dx, dy;
  int rows1, cols1, colors1;
  int rows2, cols2, colors2;
  int rows_mask, cols_mask, colors_mask;

  if (argc < 7)
  {
    printf("Usage: lab1_3 <green/blue screen file> <background file> <mask file> <output file> <dx> <dy>\n");
    return -1;
  }

  // Parse the offset values
  dx = atoi(argv[5]);
  dy = atoi(argv[6]);

  // Read in the green/blue screen image
  image1 = readPPM(&rows1, &cols1, &colors1, argv[1]);
  if (!image1)
  {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    return -1;
  }

  // Read in the background image
  image2 = readPPM(&rows2, &cols2, &colors2, argv[2]);
  if (!image2)
  {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    free(image1);
    return -1;
  }

  // Read in the mask
  mask = readPPM(&rows_mask, &cols_mask, &colors_mask, argv[3]);
  if (!mask)
  {
    fprintf(stderr, "Unable to read %s\n", argv[3]);
    free(image1);
    free(image2);
    return -1;
  }

  // Ensure mask and green/blue screen image have same dimensions
  if (rows1 != rows_mask || cols1 != cols_mask)
  {
    fprintf(stderr, "The dimensions of the green/blue screen image and the mask must match.\n");
    free(image1);
    free(image2);
    free(mask);
    return -1;
  }

  // Create the output image
  output = (Pixel *)malloc(rows2 * cols2 * sizeof(Pixel));
  if (!output)
  {
    fprintf(stderr, "Unable to allocate memory for the output image\n");
    free(image1);
    free(image2);
    free(mask);
    return -1;
  }

  // Initialize the output image to the background image
  for (int i = 0; i < rows2 * cols2; i++)
  {
    output[i] = image2[i];
  }

  // Apply the alpha blending with the offset
  for (int y = 0; y < rows1; y++)
  {
    for (int x = 0; x < cols1; x++)
    {
      int out_x = x + dx;
      int out_y = y + dy;

      // Ensure the pixel is within the bounds of the output image
      if (out_x >= 0 && out_x < cols2 && out_y >= 0 && out_y < rows2)
      {
        int idx1 = y * cols1 + x;
        int idx2 = out_y * cols2 + out_x;
        // Interpolate the mask value from 0-255 to 0.0-1.0
        double alpha = mask[idx1].r / 255.0;

        output[idx2].r = (unsigned char)(alpha * image1[idx1].r + (1 - alpha) * image2[idx2].r);
        output[idx2].g = (unsigned char)(alpha * image1[idx1].g + (1 - alpha) * image2[idx2].g);
        output[idx2].b = (unsigned char)(alpha * image1[idx1].b + (1 - alpha) * image2[idx2].b);
      }
    }
  }

  // Write the output image
  writePPM(output, rows2, cols2, 255, argv[4]);

// Free the image memory
#if USECPP
  delete[] image1;
  delete[] image2;
  delete[] mask;
  delete[] output;

#else
  free(image1);
  free(image2);
  free(mask);
  free(output);

#endif
  return 0;
}
