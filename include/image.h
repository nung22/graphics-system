#ifndef IMAGE_H

#define IMAGE_H

// Structure to represent a floating-point pixel with RGB components
typedef struct
{
  float rgb[3];
} FPixel;

// Structure to represent an image with its cols, rows, and pixel data
typedef struct
{
  int cols;     // columns
  int rows;     // rows
  float **a;     // 2D array of alpha values
  float **z;     // 2D array of depth values
  FPixel **data; // 2D array of floating-point pixels
} Image;

/* Function prototypes for image operations */
Image *image_create(int rows, int cols);
void image_free(Image *img);
void image_alloc(Image *img, int rows, int cols);
void image_dealloc(Image *img);
void image_reset(Image *img);
FPixel image_getf(Image *img, int row, int col);
float image_getc(Image *img, int row, int col);
float image_geta(Image *img, int row, int col);
float image_getz(Image *img, int row, int col);
void image_setf(Image *img, int row, int col, FPixel value);
void image_setc(Image *img, int row, int col, int ch, float value);
void image_seta(Image *img, int row, int col, float val);
void image_setz(Image *img, int row, int col, float val);
void image_fill(Image *img, FPixel value);
void image_fillrgb(Image *img, float r, float g, float b);
void image_filla(Image *img, float a);
void image_fillz(Image *img, float z);
Image *image_read(char *filename);
int image_write(Image *img, char *filename);

#endif
