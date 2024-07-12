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
void image_free(Image *src);
void image_init(Image *src); 
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);
void image_reset(Image *src);
FPixel image_getf(Image *src, int row, int col);
float image_getc(Image *src, int row, int col);
float image_geta(Image *src, int row, int col);
float image_getz(Image *src, int row, int col);
void image_setf(Image *src, int row, int col, FPixel value);
void image_setc(Image *src, int row, int col, int ch, float value);
void image_seta(Image *src, int row, int col, float val);
void image_setz(Image *src, int row, int col, float val);
void image_fill(Image *src, FPixel value);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void image_fillz(Image *src, float z);
Image *image_read(char *filename);
int image_write(Image *src, char *filename);

#endif
