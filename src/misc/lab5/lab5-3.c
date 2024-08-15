#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

void setBrown(Image *src);
void setBrown(Image *src) {
    int i, j;
    Color Brown;
    color_set(&Brown, 0.647, 0.165, 0.165);

    for (i = 0; i < src->rows; i++) {
        for (j = 0; j < src->cols; j++) {
            image_setColor(src, i, j, Brown);
        }
    }
}

void drawHouse(Line *house, Image *src, Color color);
void drawHouse(Line *house, Image *src, Color color) {
    for (int i = 0; i < 9; i++) {
        line_draw(&house[i], src, color);
    }
}

int main( int argc, char *argv[] ) {
    const int rows = 360;
    const int cols = 640;
    const int nFrames = 10;
    const float maxShake = 0.05;
    char filename[256];
    Image *src;
    Color White;
    Line house[9];
    float shakeX, shakeY;

    // Set color to black
    color_set(&White, 1.0, 1.0, 1.0);

    // Initialize image
    src = image_create(rows, cols);

    // Define the house using lines
    line_set2D(&house[0], 200, 300, 440, 300); // base
    line_set2D(&house[1], 200, 300, 200, 180); // left wall
    line_set2D(&house[2], 440, 300, 440, 180); // right wall
    line_set2D(&house[3], 200, 180, 320, 100); // left roof
    line_set2D(&house[4], 440, 180, 320, 100); // right roof
    line_set2D(&house[5], 200, 180, 440, 180); // roof bottom
    line_set2D(&house[6], 320, 300, 320, 220); // door left
    line_set2D(&house[7], 360, 300, 360, 220); // door right
    line_set2D(&house[8], 320, 220, 360, 220); // door top  

    // Loop to generate frames
    for (int t = 0; t < nFrames; t++) {
        setBrown(src);

        // Apply shaking by randomly shifting the house
        shakeX = (float)(rand() % 1000) / 1000.0 * maxShake * cols - (maxShake * cols / 2);
        shakeY = (float)(rand() % 1000) / 1000.0 * maxShake * rows - (maxShake * rows / 2);

        Line shakenHouse[9];
        for (int i = 0; i < 9; i++) {
            shakenHouse[i] = house[i];
            shakenHouse[i].a.val[0] += shakeX;
            shakenHouse[i].a.val[1] += shakeY;
            shakenHouse[i].b.val[0] += shakeX;
            shakenHouse[i].b.val[1] += shakeY;
        }

        // Draw the shaken house
        drawHouse(shakenHouse, src, White);

        // Write out the image
        sprintf(filename, "house_frame-%02d.ppm", t);
        image_write(src, filename);
    }

    image_free(src);
    return 0;
}
