#include "Viewer.h"

void scaling_smaller(int count_of_vertexes, double **matrix, double parameter) {
  if (parameter != 0) {
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < count_of_vertexes + 1; i++)
        matrix[i][j] /= fabs(parameter);
    }
  }
}

void scaling_bigger(int count_of_vertexes, double **matrix, double parameter) {
  if (parameter != 0) {
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < count_of_vertexes + 1; i++)
        matrix[i][j] *= fabs(parameter);
    }
  }
}

void shift_left(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) matrix[i][0] -= parameter;
}

void shift_up(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) matrix[i][1] += parameter;
}

void shift_forward(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) matrix[i][2] += parameter;
}

void rotation_by_x(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) {
    double y = matrix[i][1];
    double z = matrix[i][2];
    matrix[i][1] = cos(parameter) * y + sin(parameter) * z;
    matrix[i][2] = -sin(parameter) * y + cos(parameter) * z;
  }
}

void rotation_by_y(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) {
    double x = matrix[i][0];
    double z = matrix[i][2];
    matrix[i][0] = cos(parameter) * x + sin(parameter) * z;
    matrix[i][2] = -sin(parameter) * x + cos(parameter) * z;
  }
}

void rotation_by_z(int count_of_vertexes, double **matrix, double parameter) {
  for (int i = 0; i < count_of_vertexes + 1; i++) {
    double x = matrix[i][0];
    double y = matrix[i][1];
    matrix[i][0] = cos(parameter) * x + sin(parameter) * y;
    matrix[i][1] = -sin(parameter) * x + cos(parameter) * y;
  }
}

void count_file_data(char *path, size_t *count_vertices, size_t *count_edges) {
  *count_vertices = *count_edges = 0;
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("Sonya error: file open!");
    return;
  }
  char line[FILE_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    if (strlen(line) == 0) {
      continue;
    }
    if (line[0] == 'v' && line[1] == ' ') {
      (*count_vertices)++;
    }
    if (line[0] == 'f' && line[1] == ' ') {
      (*count_edges)++;
    }
  }

  fclose(file);
}

void load_model(double ***vertexes, size_t *size_vertexes, int ***edges,
                size_t *size_edges, char *path) {
  size_t count_vertices = 0;
  size_t count_edges = 0;

  count_file_data(path, &count_vertices, &count_edges);

  *size_vertexes = count_vertices;
  *size_edges = count_edges;

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("Rodion error: file open!");
    return;
  }
  char line[FILE_LINE_LENGTH];

  (*vertexes) = (double **)calloc(count_vertices + 1, sizeof(double *));
  for (size_t i = 0; i < count_vertices + 1; i++) {
    (*vertexes)[i] = (double *)calloc(3, sizeof(double));
  }

  (*edges) = (int **)calloc(count_edges + 1, sizeof(int *));

  int row_vertixes = 1, row_edges = 1;
  (*edges)[0] = (int *)calloc(1, sizeof(int));
  (*edges)[0][0] = 0;
  while (fgets(line, sizeof(line), file)) {
    if (strlen(line) < 3) {
      continue;
    }
    if (line[0] == 'v' && line[1] == ' ') {
      char work_line[FILE_LINE_LENGTH];
      strcpy(work_line, line + 1);

      char *ptr = strtok(work_line, " ");
      int j = 0;

      while (NULL != ptr && j < 3) {
        (*vertexes)[row_vertixes][j] = atof(ptr);
        ptr = strtok(NULL, " ");
        j++;
      }
      row_vertixes++;
    }
    if (line[0] == 'f' && line[1] == ' ') {
      char work_line[FILE_LINE_LENGTH];
      strcpy(work_line, line + 1);
      char *ptr = strtok(work_line, " ");

      int amount = 0;

      while (NULL != ptr) {
        ptr = strtok(NULL, " ");
        amount++;
      }
      strcpy(work_line, line + 1);
      ptr = strtok(work_line, " ");
      (*edges)[row_edges] = (int *)calloc(amount + 1, sizeof(int));
      (*edges)[row_edges][0] = amount;

      int j = 1;

      while (NULL != ptr) {
        int value = atoi(ptr);
        if (value == 0) break;
        if (value >= 0) {
          (*edges)[row_edges][j] = value;
        } else {
          (*edges)[row_edges][j] = count_vertices + 1 + value;
        }
        ptr = strtok(NULL, " ");
        j++;
      }
      row_edges++;
    }
  }

  fclose(file);
}

void clear_memory(double ***vertexes, size_t size_vertexes, int ***edges,
                  size_t size_edges) {
  for (size_t i = 0; i < size_vertexes + 1; i++) {
    free((*vertexes)[i]);
  }
  free(*vertexes);

  for (size_t i = 0; i < size_edges + 1; i++) {
    free((*edges)[i]);
  }
  free(*edges);
}

double **allocateMemory(int rows, int cols) {
  double **arr = (double **)malloc(rows * sizeof(double *));
  if (arr == NULL) {
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < rows; ++i) {
    arr[i] = (double *)malloc(cols * sizeof(double));
    if (arr[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free(arr[j]);
      }
      free(arr);

      exit(EXIT_FAILURE);
    }
  }

  return arr;
}

void free_memory(double **arr, int rows) {
  for (int i = 0; i < rows; ++i) {
    free(arr[i]);
  }
  free(arr);
}