#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygons;
} data_t;

// Функции из матриц
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

// Функции для парсинга
void V_parsing(char *line, matrix_t *vertexes_matrix, int row);
void F_parsing(char *line, polygon_t *polygons, int vert_row,
               int count_of_vertex);
void Count_f_and_v(FILE *file, data_t *file_data);
void Second_Reading(FILE *file, data_t *file_data);
int Vert_Counter_In_F(char *line);
data_t parser(char *name_of_file);

unsigned int fs_size(data_t file_data);
unsigned int *fs_array(data_t file_data, unsigned int *array);
unsigned int vs_array(data_t file_data, double *array);

// Функции для афинных преобразований
void move_x(double *data, double number_x, int count);
void move_y(double *data, double number_y, int count);
void move_z(double *data, double number_z, int count);
void rotation_by_ox(double *data, double angle, int count);
void rotation_by_oy(double *data, double angle, int count);
void rotation_by_oz(double *data, double angle, int count);
void scale(double *data, double coeff, int count);
