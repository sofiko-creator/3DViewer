#include <check.h>
#include <math.h>
#include <unistd.h>

#include "Viewer.h"

// loading tests
START_TEST(check_loading) {
  double** simple_vertexes = NULL;
  size_t vertexes_size = 0;
  int** simple_edges = NULL;
  size_t edges_size = 0;

  char currentDir[FILENAME_MAX];
  if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
    const char* relativePath = "Core/testing_objects/our_file.OBJ";
    char absolutePath[FILENAME_MAX];
    strncpy(absolutePath, currentDir, sizeof(absolutePath));
    strncat(absolutePath, "/", sizeof(absolutePath) - strlen(absolutePath) - 1);
    strncat(absolutePath, relativePath,
            sizeof(absolutePath) - strlen(absolutePath) - 1);

    load_model(&simple_vertexes, &vertexes_size, &simple_edges, &edges_size,
               absolutePath);
  } else {
    printf("\nDEBUG TEST: Не удалось получить текущий рабочий каталог\n");
  }
  ck_assert(vertexes_size == 476);
  ck_assert(edges_size == 492);
  double epsilon = 1e-6;
  ck_assert_double_eq_tol(simple_vertexes[1][0], 0, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[1][1], 11.420460, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[1][2], -0.137453, epsilon);

  ck_assert_double_eq_tol(simple_vertexes[2][0], -0.00000012196104, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[84][0], -6.80939960479736, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[133][0], -4.07009744644165, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[174][0], -11.79421997070312, epsilon);

  ck_assert_double_eq_tol(simple_vertexes[190][0], 11.79422569274902, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[190][1], -6.83394622802734, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[190][2], -6.80938482284546, epsilon);

  ck_assert_double_eq_tol(simple_vertexes[474][0], 3.1999249458313, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[474][1], 20.43998146057129, epsilon);
  ck_assert_double_eq_tol(simple_vertexes[474][2], 0.36479672789574, epsilon);

  ck_assert_int_eq(simple_edges[1][0], 3);
  ck_assert_int_eq(simple_edges[1][1], 1);
  ck_assert_int_eq(simple_edges[1][2], 2);
  ck_assert_int_eq(simple_edges[1][3], 3);

  ck_assert_int_eq(simple_edges[25][0], 4);
  ck_assert_int_eq(simple_edges[25][1], 2);
  ck_assert_int_eq(simple_edges[25][2], 26);
  ck_assert_int_eq(simple_edges[25][3], 27);
  ck_assert_int_eq(simple_edges[25][4], 3);

  ck_assert_int_eq(simple_edges[25][0], 4);
  ck_assert_int_eq(simple_edges[25][1], 2);
  ck_assert_int_eq(simple_edges[25][2], 26);
  ck_assert_int_eq(simple_edges[25][3], 27);
  ck_assert_int_eq(simple_edges[25][4], 3);

  ck_assert_int_eq(simple_edges[329][0], 3);
  ck_assert_int_eq(simple_edges[329][1], 307);
  ck_assert_int_eq(simple_edges[329][2], 308);
  ck_assert_int_eq(simple_edges[329][3], 315);

  ck_assert_int_eq(simple_edges[264][0], 4);
  ck_assert_int_eq(simple_edges[264][1], 241);
  ck_assert_int_eq(simple_edges[264][2], 265);
  ck_assert_int_eq(simple_edges[264][3], 242);
  ck_assert_int_eq(simple_edges[264][4], 218);

  clear_memory(&simple_vertexes, vertexes_size, &simple_edges, edges_size);
}
END_TEST

// affin transformation

START_TEST(test_shift_x_1) {
  double epsilon = 1e-6;
  int rows = 4;
  int cols = 3;
  double** vertixes = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);

  vertixes[0][0] = 4.01;
  vertixes[1][0] = -5;
  vertixes[2][0] = 6;
  vertixes[3][0] = 0.12;
  vertixes[0][1] = 7;
  vertixes[1][1] = -8;
  vertixes[2][1] = 9;
  vertixes[3][1] = 9;
  vertixes[0][2] = 0;
  vertixes[1][2] = 1.111;
  vertixes[2][2] = -2;
  vertixes[3][2] = -2;
  shift_left(rows - 1, vertixes, 2);
  result[0][0] = 2.01;
  result[1][0] = -7;
  result[2][0] = 4;
  result[3][0] = -1.88;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[0][2] = 0;
  result[1][2] = 1.111;
  result[2][2] = -2;
  result[3][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertixes[j][i], result[j][i], epsilon);
  }
  free_memory(vertixes, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_x_2) {
  double epsilon = 1e-6;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  shift_left(2, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_x_3) {
  double epsilon = 1e-6;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4.21;
  vertices[1][0] = -5.19;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[4][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[4][1] = -4;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  vertices[4][2] = 5.009;
  shift_left(4, vertices, -3);
  result[0][0] = 7.21;
  result[1][0] = -2.19;
  result[2][0] = 9;
  result[3][0] = 3;
  result[4][0] = 3;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[4][1] = -4;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  result[4][2] = 5.009;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_y_1) {
  double epsilon = 1e-6;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  shift_up(rows - 1, vertices, 2);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[0][1] = 9;
  result[1][1] = -6;
  result[2][1] = 11;
  result[3][1] = 11;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_y_2) {
  double epsilon = 1e-6;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[4][0] = 11;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[4][1] = 2;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  vertices[4][2] = 1;
  shift_up(rows - 1, vertices, -3);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[4][0] = 11;
  result[0][1] = 4;
  result[1][1] = -11;
  result[2][1] = 6;
  result[3][1] = 6;
  result[4][1] = -1;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  result[4][2] = 1;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_y_3) {
  double epsilon = 1e-6;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  shift_up(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_z_1) {
  double epsilon = 1e-6;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  shift_forward(rows - 1, vertices, 2);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[0][2] = 2;
  result[1][2] = 3;
  result[2][2] = 0;
  result[3][2] = 0;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_z_2) {
  double epsilon = 1e-6;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[4][0] = 11;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[4][1] = 6;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  vertices[4][2] = 3;
  shift_forward(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[4][0] = 11;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[4][1] = 6;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  result[4][2] = 3;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_shift_z_3) {
  double epsilon = 1e-6;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  shift_forward(rows - 1, vertices, -3);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[0][2] = -3;
  result[1][2] = -2;
  result[2][2] = -5;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_x_1) {
  double epsilon = 1e-5;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  rotation_by_x(rows - 1, vertices, 2);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[0][1] = -2.91303;
  result[1][1] = 4.23847;
  result[2][1] = -5.56392;
  result[3][1] = -5.56392;
  result[0][2] = -6.36508;
  result[1][2] = 6.85823;
  result[2][2] = -7.35138;
  result[3][2] = -7.35138;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_x_2) {
  double epsilon = 1e-5;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  rotation_by_x(rows - 1, vertices, -4);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 0;
  result[0][1] = -4.57551;
  result[1][1] = 5.98595;
  result[2][1] = -7.3964;
  result[0][2] = -5.29762;
  result[1][2] = 5.40078;
  result[2][2] = -5.50394;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_x_3) {
  double epsilon = 1e-5;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[4][0] = 2;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[4][1] = -4;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  vertices[4][2] = 7;
  rotation_by_x(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0;
  result[4][0] = 2;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[4][1] = -4;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  result[4][2] = 7;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_y_1) {
  double epsilon = 1e-5;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4.3;
  vertices[1][0] = -5;
  vertices[2][0] = 6.111;
  vertices[3][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2.656;
  rotation_by_y(rows - 1, vertices, 2);
  result[0][0] = -1.78943;
  result[1][0] = 2.99003;
  result[2][0] = -4.36167;
  result[3][0] = -2.41509;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9;
  result[3][1] = 9;
  result[0][2] = -3.90998;
  result[1][2] = 4.13034;
  result[2][2] = -4.72442;
  result[3][2] = 1.10529;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_y_2) {
  double epsilon = 1e-5;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0.222;
  vertices[4][0] = 2;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9.09;
  vertices[3][1] = 9;
  vertices[4][1] = -4;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2.55;
  vertices[3][2] = -2.123;
  vertices[4][2] = 7;
  rotation_by_y(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0.222;
  result[4][0] = 2;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9.09;
  result[3][1] = 9;
  result[4][1] = -4;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2.55;
  result[3][2] = -2.123;
  result[4][2] = 7;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_y_3) {
  double epsilon = 1e-5;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5.88;
  vertices[2][0] = 6;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = -3;
  vertices[0][2] = 0;
  vertices[1][2] = -6.7;
  vertices[2][2] = -2.66;
  rotation_by_y(rows - 1, vertices, -4);
  result[0][0] = -2.61457;
  result[1][0] = -1.22715;
  result[2][0] = -5.93496;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = -3;
  result[0][2] = -3.02721;
  result[1][2] = 8.82941;
  result[2][2] = -2.80212;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_z_1) {
  double epsilon = 1e-5;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 9;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  rotation_by_z(rows - 1, vertices, 2);
  result[0][0] = 4.70049;
  result[1][0] = -5.19365;
  result[2][0] = 5.6868;
  result[3][0] = 8.18368;
  result[0][1] = -6.55022;
  result[1][1] = 7.87566;
  result[2][1] = -9.20111;
  result[3][1] = -3.74532;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2;
  result[3][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_z_2) {
  double epsilon = 1e-5;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = 0.222;
  vertices[4][0] = 2;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9.09;
  vertices[3][1] = 9;
  vertices[4][1] = -4;
  vertices[0][2] = 0;
  vertices[1][2] = 1;
  vertices[2][2] = -2.55;
  vertices[3][2] = -2.123;
  vertices[4][2] = 7;
  rotation_by_z(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5;
  result[2][0] = 6;
  result[3][0] = 0.222;
  result[4][0] = 2;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9.09;
  result[3][1] = 9;
  result[4][1] = -4;
  result[0][2] = 0;
  result[1][2] = 1;
  result[2][2] = -2.55;
  result[3][2] = -2.123;
  result[4][2] = 7;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_rotation_by_z_3) {
  double epsilon = 1e-5;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5.88;
  vertices[2][0] = 6;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = -3;
  vertices[0][2] = 0;
  vertices[1][2] = -6.7;
  vertices[2][2] = -2.66;
  rotation_by_z(rows - 1, vertices, -3);
  result[0][0] = -4.94781;
  result[1][0] = 6.95012;
  result[2][0] = -5.51659;
  result[0][1] = -6.36547;
  result[1][1] = 7.09015;
  result[2][1] = 3.8167;
  result[0][2] = 0;
  result[1][2] = -6.7;
  result[2][2] = -2.66;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_smaller_1) {
  double epsilon = 1e-6;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 3;
  vertices[1][0] = 1;
  vertices[2][0] = -4;
  vertices[3][0] = 5;
  vertices[0][1] = -2;
  vertices[1][1] = 3;
  vertices[2][1] = 0;
  vertices[3][1] = 8;
  vertices[0][2] = -1;
  vertices[1][2] = -4;
  vertices[2][2] = 6;
  vertices[3][2] = 9;
  scaling_smaller(rows - 1, vertices, 2);
  result[0][0] = 1.5;
  result[1][0] = 0.5;
  result[2][0] = -2;
  result[3][0] = 2.5;
  result[0][1] = -1;
  result[1][1] = 1.5;
  result[2][1] = 0;
  result[3][1] = 4;
  result[0][2] = -0.5;
  result[1][2] = -2;
  result[2][2] = 3;
  result[3][2] = 4.5;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_smaller_2) {
  double epsilon = 1e-6;
  int rows = 2;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 3;
  vertices[1][0] = 6;
  vertices[0][1] = -3;
  vertices[1][1] = 3;
  vertices[0][2] = -1;
  vertices[1][2] = -4;
  scaling_smaller(rows - 1, vertices, -3);
  result[0][0] = 1;
  result[1][0] = 2;
  result[0][1] = -1;
  result[1][1] = 1;
  result[0][2] = -0.333333;
  result[1][2] = -1.333333;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_smaller_3) {
  double epsilon = 1e-6;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 3.55;
  vertices[1][0] = 6;
  vertices[2][0] = -4;
  vertices[3][0] = 5;
  vertices[4][0] = 7;
  vertices[0][1] = -3;
  vertices[1][1] = 3;
  vertices[2][1] = 0;
  vertices[3][1] = 8.004;
  vertices[4][1] = -2;
  vertices[0][2] = -1;
  vertices[1][2] = -4;
  vertices[2][2] = 6.6;
  vertices[3][2] = 9;
  vertices[4][2] = 2;
  scaling_smaller(rows - 1, vertices, 0);
  result[0][0] = 3.55;
  result[1][0] = 6;
  result[2][0] = -4;
  result[3][0] = 5;
  result[4][0] = 7;
  result[0][1] = -3;
  result[1][1] = 3;
  result[2][1] = 0;
  result[3][1] = 8.004;
  result[4][1] = -2;
  result[0][2] = -1;
  result[1][2] = -4;
  result[2][2] = 6.6;
  result[3][2] = 9;
  result[4][2] = 2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_bigger_1) {
  double epsilon = 1e-6;
  int rows = 4;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4.42;
  vertices[1][0] = -5;
  vertices[2][0] = 6;
  vertices[3][0] = -3;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9.11;
  vertices[3][1] = 0;
  vertices[0][2] = 4;
  vertices[1][2] = 1;
  vertices[2][2] = -2.31;
  vertices[3][2] = -2;
  scaling_bigger(rows - 1, vertices, 2);
  result[0][0] = 8.84;
  result[1][0] = -10;
  result[2][0] = 12;
  result[3][0] = -6;
  result[0][1] = 14;
  result[1][1] = -16;
  result[2][1] = 18.22;
  result[3][1] = 0;
  result[0][2] = 8;
  result[1][2] = 2;
  result[2][2] = -4.62;
  result[3][2] = -4;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_bigger_2) {
  double epsilon = 1e-6;
  int rows = 5;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5;
  vertices[2][0] = 6.12;
  vertices[3][0] = -3;
  vertices[4][0] = -3;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9;
  vertices[3][1] = 0;
  vertices[4][1] = -5;
  vertices[0][2] = 4;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  vertices[3][2] = -2;
  vertices[4][2] = 0.33;
  scaling_bigger(rows - 1, vertices, -3);
  result[0][0] = 12;
  result[1][0] = -15;
  result[2][0] = 18.36;
  result[3][0] = -9;
  result[4][0] = -9;
  result[0][1] = 21;
  result[1][1] = -24;
  result[2][1] = 27;
  result[3][1] = 0;
  result[4][1] = -15;
  result[0][2] = 12;
  result[1][2] = 3;
  result[2][2] = -6;
  result[3][2] = -6;
  result[4][2] = 0.99;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

START_TEST(test_scale_bigger_3) {
  double epsilon = 1e-6;
  int rows = 3;
  int cols = 3;
  double** vertices = allocateMemory(rows, cols);
  double** result = allocateMemory(rows, cols);
  vertices[0][0] = 4;
  vertices[1][0] = -5.12;
  vertices[2][0] = -3.5555;
  vertices[0][1] = 7;
  vertices[1][1] = -8;
  vertices[2][1] = 9.0001;
  vertices[0][2] = 4;
  vertices[1][2] = 1;
  vertices[2][2] = -2;
  scaling_bigger(rows - 1, vertices, 0);
  result[0][0] = 4;
  result[1][0] = -5.12;
  result[2][0] = -3.5555;
  result[0][1] = 7;
  result[1][1] = -8;
  result[2][1] = 9.0001;
  result[0][2] = 4;
  result[1][2] = 1;
  result[2][2] = -2;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++)
      ck_assert_double_eq_tol(vertices[j][i], result[j][i], epsilon);
  }
  free_memory(vertices, rows);
  free_memory(result, rows);
}
END_TEST

Suite* s21_3D_Viewer_tests_create() {
  Suite* s;
  TCase* tc_core;
  s = suite_create("s21_3D_Viewer");
  tc_core = tcase_create("tests");
  tcase_add_test(tc_core, check_loading);
  tcase_add_test(tc_core, test_shift_x_1);
  tcase_add_test(tc_core, test_shift_x_2);
  tcase_add_test(tc_core, test_shift_x_3);
  tcase_add_test(tc_core, test_shift_y_1);
  tcase_add_test(tc_core, test_shift_y_2);
  tcase_add_test(tc_core, test_shift_y_3);
  tcase_add_test(tc_core, test_shift_z_1);
  tcase_add_test(tc_core, test_shift_z_2);
  tcase_add_test(tc_core, test_shift_z_3);
  tcase_add_test(tc_core, test_rotation_by_x_1);
  tcase_add_test(tc_core, test_rotation_by_x_2);
  tcase_add_test(tc_core, test_rotation_by_x_3);
  tcase_add_test(tc_core, test_rotation_by_y_1);
  tcase_add_test(tc_core, test_rotation_by_y_2);
  tcase_add_test(tc_core, test_rotation_by_y_3);
  tcase_add_test(tc_core, test_rotation_by_z_1);
  tcase_add_test(tc_core, test_rotation_by_z_2);
  tcase_add_test(tc_core, test_rotation_by_z_3);
  tcase_add_test(tc_core, test_scale_smaller_1);
  tcase_add_test(tc_core, test_scale_smaller_2);
  tcase_add_test(tc_core, test_scale_smaller_3);
  tcase_add_test(tc_core, test_scale_bigger_1);
  tcase_add_test(tc_core, test_scale_bigger_2);
  tcase_add_test(tc_core, test_scale_bigger_3);
  suite_add_tcase(s, tc_core);
  return s;
}
int main() {
  Suite* s = s21_3D_Viewer_tests_create();
  SRunner* s21_3D_Viewer_runner = srunner_create(s);
  int number_failed;
  srunner_run_all(s21_3D_Viewer_runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(s21_3D_Viewer_runner);
  srunner_free(s21_3D_Viewer_runner);

  return number_failed == 0 ? 0 : 1;
}
