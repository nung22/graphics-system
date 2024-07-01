// These functions include headers and functions needed for a graphics system.
// Written by Nicholas Ung 2024-05-26

#include <stdlib.h>
#include <math.h>
#include "graphics.h"

// Clamps a value between a minimum and maximum
double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Error handling function
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}
