// These functions provide methods for reading and writing images.
// Written by Nicholas Ung 2024-05-20

#include "image.h"
#include "ppmIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to allocate image data
int image_allocate_data(Image *src, int rows, int cols)
{
  // Allocate memory for 2D floating-point pixel data
  src->data = (FPixel **)malloc(rows * sizeof(FPixel *));
  if (!src->data)
    return 1;

  FPixel *dataBlock = (FPixel *)malloc(rows * cols * sizeof(FPixel));
  if (!dataBlock)
  {
    free(src->data);
    return 1;
  }

  for (int i = 0; i < rows; i++)
  {
    src->data[i] = dataBlock + i * cols;
  }

  // Allocate memory for 2D alpha values
  src->a = (float **)malloc(rows * sizeof(float *));
  if (!src->a)
  {
    free(dataBlock);
    free(src->data);
    return 1;
  }

  float *aBlock = (float *)malloc(rows * cols * sizeof(float));
  if (!aBlock)
  {
    free(src->a);
    free(dataBlock);
    free(src->data);
    return 1;
  }

  for (int i = 0; i < rows; i++)
  {
    src->a[i] = aBlock + i * cols;
  }

  // Allocate memory for 2D depth values
  src->z = (float **)malloc(rows * sizeof(float *));
  if (!src->z)
  {
    free(aBlock);
    free(src->a);
    free(dataBlock);
    free(src->data);
    return 1;
  }

  float *zBlock = (float *)malloc(rows * cols * sizeof(float));
  if (!zBlock)
  {
    free(src->z);
    free(aBlock);
    free(src->a);
    free(dataBlock);
    free(src->data);
    return 1;
  }

  for (int i = 0; i < rows; i++)
  {
    src->z[i] = zBlock + i * cols;
  }

  // Initialize image data to appropriate values
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      src->data[i][j].rgb[0] = 0.0f; // Initialize R to 0.0
      src->data[i][j].rgb[1] = 0.0f; // Initialize G to 0.0
      src->data[i][j].rgb[2] = 0.0f; // Initialize B to 0.0
      src->a[i][j] = 1.0f;           // Initialize alpha to 1.0
      src->z[i][j] = 1.0f;           // Initialize depth to 1.0
    }
  }

  return 0;
}

// Create an image with the specified cols and rows
Image *image_create(int rows, int cols)
{
  Image *src = (Image *)malloc(sizeof(Image));
  if (src)
  {
    if (rows == 0 || cols == 0)
    {
      src->cols = 0;
      src->rows = 0;
      src->data = NULL;
      src->a = NULL;
      src->z = NULL;
    }
    else
    {
      src->cols = cols;
      src->rows = rows;
      if (image_allocate_data(src, rows, cols) != 0)
      {
        return NULL;
      }
    }
  }
  return src;
}

// Free the memory allocated for an image
void image_free(Image *src)
{
  if (src) // Check if the image pointer is not NULL
  {
    free(src->data[0]); // Free the pixel data block
    free(src->data);    // Free the array of row pointers
    free(src->a[0]);    // Free the alpha data block
    free(src->a);       // Free the array of alpha row pointers
    free(src->z[0]);    // Free the depth data block
    free(src->z);       // Free the array of depth row pointers
    free(src);          // Free the image structure
  }
}

// Initialize an image
void image_init(Image *src)
{
  if (src) // Check if the image pointer is not NULL
  {
    src->cols = 0; // Set the image cols to zero
    src->rows = 0; // Set the image rows to zero
    src->data = NULL;
    src->a = NULL;
    src->z = NULL;
  }
}

// Allocate memory for an image with the specified cols and rows
int image_alloc(Image *src, int rows, int cols)
{
  if (src) // Check if the image pointer is not NULL
  {
    src->cols = cols;
    src->rows = rows;

    // Free existing memory if any
    if (src->data)
    {
      free(src->data[0]);
      free(src->data);
    }
    if (src->a)
    {
      free(src->a[0]);
      free(src->a);
    }
    if (src->z)
    {
      free(src->z[0]);
      free(src->z);
    }

    return image_allocate_data(src, rows, cols);
  }

  return 1;
}

// Deallocate image data
void image_dealloc(Image *src)
{
  if (src)
  {                     // Check if the image pointer is not NULL
    free(src->data[0]); // Free the pixel data block
    free(src->data);    // Free the array of row pointers
    free(src->a[0]);    // Free the alpha data block
    free(src->a);       // Free the array of alpha row pointers
    free(src->z[0]);    // Free the depth data block
    free(src->z);       // Free the array of depth row pointers

    // Reset the Image structure fields
    image_init(src);
  }
}

// Resets every pixel to a default value (e.g. Black, alpha value of 1.0, z value of 1.0)
void image_reset(Image *src)
{
  if (src) // Check if the image pointer is not NULL
  {
    memset(src->data[0], 0, src->cols * src->rows * sizeof(FPixel)); // Set all pixel values to zero
    for (int i = 0; i < src->rows; i++)
    {
      for (int j = 0; j < src->cols; j++)
      {
        src->a[i][j] = 1.0; // Set all alpha values to 1.0
        src->z[i][j] = 1.0; // Set all depth values to 1.0
      }
    }
  }
}

// Returns the FPixel at (r, c).
FPixel image_getf(Image *src, int row, int col)
{
  FPixel pixel = {0}; // Initialize a pixel with all zeros
  if (src && row < src->rows && col < src->cols)
  {                              // Check if the coordinates are within bounds
    pixel = src->data[row][col]; // Get the pixel value
  }
  return pixel; // Return the pixel
}

// Returns the value of band b at pixel (r, c).
float image_getc(Image *src, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (src && row < src->rows && col < src->cols)
  {                                     // Check if the coordinates are within bounds
    value = src->data[row][col].rgb[0]; // Get the red channel value
  }
  return value; // Return the channel value
}

// Returns the alpha value at pixel (r, c).
float image_geta(Image *src, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (src && row < src->rows && col < src->cols)
  {                           // Check if the coordinates are within bounds
    value = src->a[row][col]; // Get the alpha value
  }
  return value; // Return the alpha value
}

// Returns the depth value at pixel (r, c).
float image_getz(Image *src, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (src && row < src->rows && col < src->cols)
  {                           // Check if the coordinates are within bounds
    value = src->z[row][col]; // Get the depth value
  }
  return value; // Return the depth value
}

// Sets the alpha value at pixel (r, c) to val.
void image_seta(Image *src, int row, int col, float value)
{
  if (src && row < src->rows && col < src->cols)
  {                           // Check if the coordinates are within bounds
    src->a[row][col] = value; // Set the alpha value for the pixel
  }
}

// Sets the depth value at pixel (r, c) to val.
void image_setz(Image *src, int row, int col, float value)
{
  if (src && row < src->rows && col < src->cols)
  {                           // Check if the coordinates are within bounds
    src->z[row][col] = value; // Set the depth value for the pixel
  }
}

// Sets the value of pixel (r, c) band b to val.
void image_setc(Image *src, int row, int col, int ch, float value)
{
  if (src && row < src->rows && col < src->cols && ch >= 0 && ch < 3)
  {                                      // Check if coordinates and channel are within bounds
    src->data[row][col].rgb[ch] = value; // Set the channel value for the pixel
  }
}

// Sets the values of pixel (r, c) to the FPixel val.
void image_setf(Image *src, int row, int col, FPixel value)
{
  if (src && row < src->rows && col < src->cols)
  {                              // Check if the coordinates are within bounds
    src->data[row][col] = value; // Set the pixel value
  }
}

// Fills the image with the FPixel value.
void image_fill(Image *src, FPixel value)
{
  if (src)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < src->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < src->cols; j++)
      {                               // Loop over the columns
        image_setf(src, i, j, value); // Set the pixel value
      }
    }
  }
}

// Sets the (r, g, b) values of each pixel to the given color.
void image_fillrgb(Image *src, float r, float g, float b)
{
  FPixel value = {r, g, b}; // Create an FPixel with the specified color
  image_fill(src, value);   // Fill the image with the color
}

// Sets the alpha value of each pixel to the given value.
void image_filla(Image *src, float a)
{
  if (src)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < src->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < src->cols; j++)
      {                           // Loop over the columns
        image_seta(src, i, j, a); // Set the alpha value
      }
    }
  }
}

// Sets the depth value of each pixel to the given value.
void image_fillz(Image *src, float z)
{
  if (src)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < src->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < src->cols; j++)
      {                           // Loop over the columns
        image_setz(src, i, j, z); // Set the depth value
      }
    }
  }
}

// Read a PPM P6 image from the given filename. Initializes the alpha channel to 1.0 and the z channel to 1.0. Returns a NULL pointer if the operation fails.
Image *image_read(char *filename)
{
  int rows, cols, colors;
  Pixel *ppmData = readPPM(&rows, &cols, &colors, filename);

  if (!ppmData)
  {
    fprintf(stderr, "Unable to read PPM file %s\n", filename);
    return NULL; // Return NULL if the PPM read fails
  }

  Image *src = image_create(rows, cols);
  if (!src)
  {
    free(ppmData);
    return NULL; // Return NULL if the image could not be created
  }

  // Convert PPM data to Image format
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      Pixel p = ppmData[i * cols + j];
      FPixel pixel = {p.r / 255.0, p.g / 255.0, p.b / 255.0};
      src->data[i][j] = pixel; // Set the pixel value in the image
    }
  }

  free(ppmData);
  return src;
}

// Write the image to a file in PPM P6 format
int image_write(Image *src, char *filename)
{
  int rows = src->rows;
  int cols = src->cols;
  Pixel *ppmData = (Pixel *)malloc(rows * cols * sizeof(Pixel));

  if (!ppmData)
  {
    fprintf(stderr, "Unable to allocate memory for PPM data\n");
    return 0; // Return 0 if memory allocation fails
  }

  // Convert Image format to PPM data
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      FPixel p = src->data[i][j];
      // Bounds check the pixel values
      p.rgb[0] = p.rgb[0] < 0 ? 0 : p.rgb[0] > 1 ? 1 : p.rgb[0];
      p.rgb[1] = p.rgb[1] < 0 ? 0 : p.rgb[1] > 1 ? 1 : p.rgb[1];
      p.rgb[2] = p.rgb[2] < 0 ? 0 : p.rgb[2] > 1 ? 1 : p.rgb[2];
      Pixel pixel = {(unsigned char)(p.rgb[0] * 255), (unsigned char)(p.rgb[1] * 255), (unsigned char)(p.rgb[2] * 255)};
      ppmData[i * cols + j] = pixel;
    }
  }

  writePPM(ppmData, rows, cols, 255, filename);

  // free(ppmData);
  return 1; // Return 1 to indicate success
}