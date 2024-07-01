// These functions provide methods for setting 2D and 3D View matrices.
// Written by Nicholas Ung 2024-06-27

#include "view.h"
#include <math.h>

// Set the view2D data structure
void view2D_set(View2D *view, Point *vrp, double dx, Vector *x, int screenx, int screeny)
{
  point_copy(&view->vrp, vrp);
  view->dx = dx;
  vector_copy(&view->x, x);
  view->screenx = screenx;
  view->screeny = screeny;
}

// Set the view2D data structure
void matrix_setView2D(Matrix *vtm, View2D *view)
{
  double dy = view->dx * view->screeny / view->screenx;

  // Clear the matrix
  matrix_identity(vtm);
  matrix_print(vtm, stdout); // Identity matrix

  // Translate the view reference point (vrp) to the origin
  matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
  matrix_print(vtm, stdout); // Translated to origin

  // Apply scaling
  Matrix scale;
  matrix_identity(&scale);
  scale.m[0][0] = view->screenx / view->dx;
  scale.m[1][1] = -view->screeny / dy; // Flipping y-axis to match image coordinates
  matrix_multiply(&scale, vtm, vtm);
  matrix_print(vtm, stdout); // Scaled to screen size

  // Translate to match image space
  Matrix translate;
  matrix_identity(&translate);
  translate.m[0][3] = view->screenx / 2.0;
  translate.m[1][3] = view->screeny / 2.0;
  matrix_multiply(&translate, vtm, vtm);
  matrix_print(vtm, stdout); // Translated to image space
}

// Set the view3D data structure
void matrix_setView3D(Matrix *vtm, View3D *view) {
  // Initialize the view transformation matrix as an identity matrix
  matrix_identity(vtm);

  // Step 1: Translate VRP to the origin
  Matrix translation;
  matrix_identity(&translation);
  matrix_translate(&translation, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
  matrix_multiply(&translation, vtm, vtm);
  printf("After VRP translation:\n");
  matrix_print(vtm, stdout);

  // Step 2: Calculate U vector (U = VUP x VPN)
  Vector u, vup, vpn;
  vector_normalize(&view->vpn); // Ensure VPN is normalized
  vector_cross(&view->vup, &view->vpn, &u);
  vector_normalize(&u);

  // Step 3: Recalculate VUP so the view reference coordinates are orthogonal (VUP = VPN x U)
  vector_cross(&view->vpn, &u, &vup);
  vector_normalize(&vup);

  vpn = view->vpn; // Use the normalized VPN
  vector_normalize(&vpn);

  printf("View reference axes\n");
  printf("[ %.3f %.3f %.3f %.3f ]\n", u.val[0], u.val[1], u.val[2], u.val[3]);
  printf("[ %.3f %.3f %.3f %.3f ]\n", vup.val[0], vup.val[1], vup.val[2], vup.val[3]);
  printf("[ %.3f %.3f %.3f %.3f ]\n\n", vpn.val[0], vpn.val[1], vpn.val[2], vpn.val[3]);

  // Step 4: Rotate to align axes using Rxyz rotation method
  Matrix rotation;
  matrix_identity(&rotation);
  matrix_set(&rotation, 0, 0, u.val[0]);
  matrix_set(&rotation, 0, 1, u.val[1]);
  matrix_set(&rotation, 0, 2, u.val[2]);
  matrix_set(&rotation, 1, 0, vup.val[0]);
  matrix_set(&rotation, 1, 1, vup.val[1]);
  matrix_set(&rotation, 1, 2, vup.val[2]);
  matrix_set(&rotation, 2, 0, vpn.val[0]);
  matrix_set(&rotation, 2, 1, vpn.val[1]);
  matrix_set(&rotation, 2, 2, vpn.val[2]);
  matrix_multiply(&rotation, vtm, vtm);
  printf("After Rxyz\n");
  matrix_print(vtm, stdout);

  // Step 5: Translate the center of projection (COP) to the origin
  matrix_identity(&translation);
  matrix_translate(&translation, 0, 0, view->d);
  matrix_multiply(&translation, vtm, vtm);
  printf("After translating COP to origin:\n");
  matrix_print(vtm, stdout);

  // Step 6: Update VRP' after transformations
  Point vrp_prime;
  matrix_xformPoint(vtm, &view->vrp, &vrp_prime);

  // Step 7: Calculate B'
  double B_prime = view->d + view->b;

  // Step 8: Scale to canonical view volume (CVV)
  Matrix scale;
  matrix_identity(&scale);
  double scaleX = 2.0 * view->d / (view->du * B_prime);
  double scaleY = 2.0 * view->d / (view->dv * B_prime);
  double scaleZ = 1.0 / B_prime;
  matrix_set(&scale, 0, 0, scaleX);
  matrix_set(&scale, 1, 1, scaleY);
  matrix_set(&scale, 2, 2, scaleZ);
  matrix_multiply(&scale, vtm, vtm);
  printf("After scaling to CVV:\n");
  matrix_print(vtm, stdout);

  // Step 9: Apply perspective projection
  double d_prime = view->d / B_prime;
  if (view->d != 0) {
    Matrix perspective;
    matrix_identity(&perspective);
    perspective.m[2][2] = 1.0;
    perspective.m[2][3] = 0.0;
    perspective.m[3][2] = 1.0 / d_prime;
    perspective.m[3][3] = 0.0;
    matrix_multiply(&perspective, vtm, vtm);
    printf("After perspective:\n");
    matrix_print(vtm, stdout);
  }

  // Step 10: Scale to image coordinates
  Matrix imageScale;
  matrix_identity(&imageScale);
  matrix_scale(&imageScale, -view->screenx / 2.0 / d_prime, -view->screeny / 2.0 / d_prime, 1);
  matrix_multiply(&imageScale, vtm, vtm);
  printf("After scale to image coords:\n");
  matrix_print(vtm, stdout);

  // Step 11: Translate to image coordinates
  matrix_translate(vtm, view->screenx / 2.0, view->screeny / 2.0, 0);
  printf("After final translation to image coords:\n");
  matrix_print(vtm, stdout);
}
