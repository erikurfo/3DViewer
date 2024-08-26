#include "parser.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = 0;
  if (result) {
    result->matrix = NULL;
    result->columns = 0;
    result->rows = 0;
  }
  if (rows < 1 || columns < 1) {
    error = 1;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double *));
    if (result->matrix != NULL) {
      for (int i = 0; i < result->rows; i++) {
        result->matrix[i] = calloc(columns, sizeof(double));
      }
      error = 0;
    } else {
      error = 1;
    }
  }
  return error;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
  A->columns = 0;
  A->rows = 0;
}

void Count_f_and_v(FILE *file, data_t *file_data) {
  file_data->count_of_facets = 0;
  file_data->count_of_vertexes = 0;
  char *line = calloc(500, sizeof(char));
  while (fgets(line, 500, file)) {
    if (line[0] == 'f' && line[1] == ' ') file_data->count_of_facets++;
    if (line[0] == 'v' && line[1] == ' ') file_data->count_of_vertexes++;
  }
  file_data->polygons = calloc(file_data->count_of_facets, sizeof(polygon_t));
  free(line);
}

void Second_Reading(FILE *file, data_t *file_data) {
  int row = 1, vert_row = 0;
  for (int i = 0; i < 3; i++) file_data->matrix_3d.matrix[0][i] = 0;
  char *line = calloc(500, sizeof(char));
  while (fgets(line, 500, file)) {
    if (line[0] == 'v' && line[1] == ' ') {
      V_parsing(line, &file_data->matrix_3d, row);
      row++;
    }
    if (line[0] == 'f' && line[1] == ' ') {
      F_parsing(line, file_data->polygons, vert_row,
                file_data->count_of_vertexes);
      vert_row++;
    }
  }
  free(line);
}

void V_parsing(char *line, matrix_t *vertexes_matrix, int row) {
  char *token;
  const char delimeters[] = " \r\n\0";
  int i = 0;
  token = strtok(line, delimeters);
  while (token) {
    token = strtok(NULL, delimeters);
    if (token) vertexes_matrix->matrix[row][i] = atof(token), i++;
  }
}

void F_parsing(char *line, polygon_t *polygons, int vert_row,
               int count_of_vertex) {
  int i = 0;
  const char delimeters[] = " \r\n\0";
  polygons[vert_row].numbers_of_vertexes_in_facets = Vert_Counter_In_F(line);
  polygons[vert_row].vertexes = calloc(
      (polygons[vert_row].numbers_of_vertexes_in_facets + 1), sizeof(int));
  char *token;
  token = strtok(line, delimeters);
  while (token) {
    token = strtok(NULL, delimeters);
    if (token) {
      polygons[vert_row].vertexes[i] =
          (atof(token) < 0) ? (count_of_vertex + atof(token) + 1) : atof(token);
      i++;
    }
  }
}

int Vert_Counter_In_F(char *line) {
  char numbers[] = "0123456789-";
  int vert_counter = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == ' ' && strchr(numbers, line[i + 1])) vert_counter++;
  }
  return vert_counter;
}

data_t parser(char *name_of_file) {
  FILE *file = fopen(name_of_file, "r");
  data_t file_data;
  Count_f_and_v(file, &file_data);
  fclose(file);
  file = fopen(name_of_file, "r");
  s21_create_matrix(file_data.count_of_vertexes + 1, 3, &file_data.matrix_3d);
  Second_Reading(file, &file_data);
  fclose(file);
  return file_data;
}

unsigned int fs_size(data_t file_data) {
  unsigned int array_size = 0;
  for (unsigned int i = 0; i < file_data.count_of_facets; i++) {
    array_size += file_data.polygons[i].numbers_of_vertexes_in_facets;
  }
  return (array_size * 2);
}

unsigned int vs_array(data_t file_data, double *array) {
  unsigned int index = 0;
  for (int i = 1; i < file_data.matrix_3d.rows; i++) {
    for (int j = 0; j < file_data.matrix_3d.columns; j++) {
      array[index] = file_data.matrix_3d.matrix[i][j], index++;
    }
  }
  return index;
}

unsigned int *fs_array(data_t file_data, unsigned int *array) {
  unsigned int index = 0;
  for (unsigned int i = 0; i < file_data.count_of_facets; i++) {
    int temp = file_data.polygons[i].numbers_of_vertexes_in_facets;
    for (int j = 0; j < file_data.polygons[i].numbers_of_vertexes_in_facets;
         j++) {
      if (j == 0) {
        array[index] = file_data.polygons[i].vertexes[j] - 1;
        index++;
      } else {
        array[index] = file_data.polygons[i].vertexes[j] - 1, index++;
        array[index] = file_data.polygons[i].vertexes[j] - 1, index++;
        if (j == (temp - 1))
          array[index] = file_data.polygons[i].vertexes[0] - 1, index++;
      }
    }
  }
  return array;
}
