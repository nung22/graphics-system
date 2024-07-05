#ifndef GRAPHICS_H

#define GRAPHICS_H

#include "point.h"
#include "color.h"
#include "image.h"
#include "vector.h"
#include "line.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"
#include "drawstate.h"
#include "bezier.h"
#include "module.h"
#include <math.h>

// Common constants
#define PI 3.14159265358979323846
#define DEG_TO_RAD(deg) ((deg) * (PI / 180.0))
#define RAD_TO_DEG(rad) ((rad) * (180.0 / PI))

// Utility functions
double clamp(double value, double min, double max);

// Error handling function
void handle_error(const char *message);

#endif // GRAPHICS_H