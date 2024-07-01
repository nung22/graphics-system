// These functions provide methods for reading and writing images.
// Written by Nicholas Ung 2024-05-20

#include "image.h"
#include "ppmIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create an image with the specified cols and rows
Image *image_create(int rows, int cols)
{
  Image *img = (Image *)malloc(sizeof(Image)); // Allocate memory for the image structure
  if (img)
  {                   // Check if the image pointer is not NULL
    img->cols = cols; // Set the image cols
    img->rows = rows; // Set the image rows

    // Allocate memory for 2D floating-point pixel data
    img->data = (FPixel **)malloc(rows * sizeof(FPixel *));
    FPixel *dataBlock = (FPixel *)malloc(rows * cols * sizeof(FPixel));
    for (int i = 0; i < rows; i++)
    {
      img->data[i] = dataBlock + i * cols;
    }

    // Allocate memory for 2D alpha values
    img->a = (float **)malloc(rows * sizeof(float *));
    float *aBlock = (float *)malloc(rows * cols * sizeof(float));
    for (int i = 0; i < rows; i++)
    {
      img->a[i] = aBlock + i * cols;
    }

    // Allocate memory for 2D depth values
    img->z = (float **)malloc(rows * sizeof(float *));
    float *zBlock = (float *)malloc(rows * cols * sizeof(float));
    for (int i = 0; i < rows; i++)
    {
      img->z[i] = zBlock + i * cols;
    }
  }
  return img; // Return the image pointer
}

// Free the memory allocated for an image
void image_free(Image *img)
{
  if (img)
  {                     // Check if the image pointer is not NULL
    free(img->data[0]); // Free the pixel data block
    free(img->data);    // Free the array of row pointers
    free(img->a[0]);    // Free the alpha data block
    free(img->a);       // Free the array of alpha row pointers
    free(img->z[0]);    // Free the depth data block
    free(img->z);       // Free the array of depth row pointers
    free(img);          // Free the image structure
  }
}

// Allocate memory for an image with the specified cols and rows
void image_alloc(Image *img, int rows, int cols)
{
  if (img)
  {                   // Check if the image pointer is not NULL
    img->cols = cols; // Set the image cols
    img->rows = rows; // Set the image rows

    // Allocate memory for 2D floating-point pixel data
    img->data = (FPixel **)malloc(rows * sizeof(FPixel *));
    FPixel *dataBlock = (FPixel *)malloc(rows * cols * sizeof(FPixel));
    for (int i = 0; i < rows; i++)
    {
      img->data[i] = dataBlock + i * cols;
    }
  }
}

// Deallocate memory for an image
void image_dealloc(Image *img)
{
  if (img)
  {                     // Check if the image pointer is not NULL
    free(img->data[0]); // Free the pixel data block
    free(img->data);    // Free the array of row pointers
    free(img->a[0]);    // Free the alpha data block
    free(img->a);       // Free the array of alpha row pointers
    free(img->z[0]);    // Free the depth data block
    free(img->z);       // Free the array of depth row pointers
  }
}

// Resets every pixel to a default value (e.g. Black, alpha value of 1.0, z value of 1.0)
void image_reset(Image *img)
{
  if (img)
  {                                                                  // Check if the image pointer is not NULL
    memset(img->data[0], 0, img->cols * img->rows * sizeof(FPixel)); // Set all pixel values to zero
    for (int i = 0; i < img->rows; i++)
    {
      for (int j = 0; j < img->cols; j++)
      {
        img->a[i][j] = 1.0; // Set all alpha values to 1.0
        img->z[i][j] = 1.0; // Set all depth values to 1.0
      }
    }
  }
}

// Returns the FPixel at (r, c).
FPixel image_getf(Image *img, int row, int col)
{
  FPixel pixel = {0}; // Initialize a pixel with all zeros
  if (img && row < img->rows && col < img->cols)
  {                              // Check if the coordinates are within bounds
    pixel = img->data[row][col]; // Get the pixel value
  }
  return pixel; // Return the pixel
}

// Returns the value of band b at pixel (r, c).
float image_getc(Image *img, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (img && row < img->rows && col < img->cols)
  {                                     // Check if the coordinates are within bounds
    value = img->data[row][col].rgb[0]; // Get the red channel value
  }
  return value; // Return the channel value
}

// Returns the alpha value at pixel (r, c).
float image_geta(Image *img, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (img && row < img->rows && col < img->cols)
  {                           // Check if the coordinates are within bounds
    value = img->a[row][col]; // Get the alpha value
  }
  return value; // Return the alpha value
}

// Returns the depth value at pixel (r, c).
float image_getz(Image *img, int row, int col)
{
  float value = 0; // Initialize the value to zero
  if (img && row < img->rows && col < img->cols)
  {                           // Check if the coordinates are within bounds
    value = img->z[row][col]; // Get the depth value
  }
  return value; // Return the depth value
}

// Sets the alpha value at pixel (r, c) to val.
void image_seta(Image *img, int row, int col, float value)
{
  if (img && row < img->rows && col < img->cols)
  {                           // Check if the coordinates are within bounds
    img->a[row][col] = value; // Set the alpha value for the pixel
  }
}

// Sets the depth value at pixel (r, c) to val.
void image_setz(Image *img, int row, int col, float value)
{
  if (img && row < img->rows && col < img->cols)
  {                           // Check if the coordinates are within bounds
    img->z[row][col] = value; // Set the depth value for the pixel
  }
}

// Sets the value of pixel (r, c) band b to val.
void image_setc(Image *img, int row, int col, int ch, float value)
{
  if (img && row < img->rows && col < img->cols && ch >= 0 && ch < 3)
  {                                      // Check if coordinates and channel are within bounds
    img->data[row][col].rgb[ch] = value; // Set the channel value for the pixel
  }
}

// Sets the values of pixel (r, c) to the FPixel val.
void image_setf(Image *img, int row, int col, FPixel value)
{
  if (img && row < img->rows && col < img->cols)
  {                              // Check if the coordinates are within bounds
    img->data[row][col] = value; // Set the pixel value
  }
}

// Fills the image with the FPixel value.
void image_fill(Image *img, FPixel value)
{
  if (img)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < img->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < img->cols; j++)
      {                               // Loop over the columns
        image_setf(img, i, j, value); // Set the pixel value
      }
    }
  }
}

// Sets the (r, g, b) values of each pixel to the given color.
void image_fillrgb(Image *img, float r, float g, float b)
{
  FPixel value = {r, g, b}; // Create an FPixel with the specified color
  image_fill(img, value);   // Fill the image with the color
}

// Sets the alpha value of each pixel to the given value.
void image_filla(Image *img, float a)
{
  if (img)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < img->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < img->cols; j++)
      {                           // Loop over the columns
        image_seta(img, i, j, a); // Set the alpha value
      }
    }
  }
}

// Sets the depth value of each pixel to the given value.
void image_fillz(Image *img, float z)
{
  if (img)
  { // Check if the image pointer is not NULL
    for (int i = 0; i < img->rows; i++)
    { // Loop over the rows
      for (int j = 0; j < img->cols; j++)
      {                           // Loop over the columns
        image_setz(img, i, j, z); // Set the depth value
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

  Image *img = image_create(rows, cols);
  if (!img)
  {
    free(ppmData);
    return NULL; // Return NULL if the image could not be created
  }

  // Initialize alpha and z buffers
  memset(img->a, 1.0, rows * cols * sizeof(float));
  memset(img->z, 1.0, rows * cols * sizeof(float));

  // Convert PPM data to Image format
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      Pixel p = ppmData[i * cols + j];
      FPixel pixel = {p.r / 255.0, p.g / 255.0, p.b / 255.0};
      img->data[i][j] = pixel; // Set the pixel value in the image
    }
  }

  free(ppmData);
  return img;
}

// Write the image to a file in PPM P6 format
int image_write(Image *img, char *filename)
{
  int rows = img->rows;
  int cols = img->cols;
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
      FPixel p = img->data[i][j];
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