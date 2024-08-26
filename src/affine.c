#include "parser.h"

#if defined(__linux__)
#define M_PI 3.14159265358979323846
#endif

void move_x(double* data, double number_x, int count) {
  for (int i = 0; i < count; i += 3) data[i] += number_x;
}

void move_y(double* data, double number_y, int count) {
  for (int i = 1; i < count; i += 3) data[i] += number_y;
}

void move_z(double* data, double number_z, int count) {
  for (int i = 2; i < count; i += 3) data[i] += number_z;
}

void rotation_by_ox(double* data, double angle, int count) {
  angle = (M_PI / 180) * angle;
  for (int i = 0; i < count; i += 3) {
    double temp_y = data[i + 1];
    double temp_z = data[i + 2];
    data[i + 1] = cos(angle) * temp_y - sin(angle) * temp_z;
    data[i + 2] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_oy(double* data, double angle, int count) {
  angle = (M_PI / 180) * angle;
  for (int i = 1; i < count; i += 3) {
    double temp_x = data[i - 1];
    double temp_z = data[i + 1];
    data[i - 1] = cos(angle) * temp_x + sin(angle) * temp_z;
    data[i + 1] = cos(angle) * temp_z - sin(angle) * temp_x;
  }
}

void rotation_by_oz(double* data, double angle, int count) {
  angle = (M_PI / 180) * angle;
  for (int i = 2; i < count; i += 3) {
    double temp_x = data[i - 2];
    double temp_y = data[i - 1];
    data[i - 2] = cos(angle) * temp_x - sin(angle) * temp_y;
    data[i - 1] = sin(angle) * temp_x + cos(angle) * temp_y;
  }
}

void scale(double* data, double coeff, int count) {
  for (int i = 0; i < count; i++) {
    data[i] *= coeff;
  }
}
