#ifndef VIEWER_H
#define VIEWER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LINE_LENGTH 1000
#define VOBJLENGTH 3

void scaling_smaller(int count_of_vertexes, double **matrix, double parameter);
void scaling_bigger(int count_of_vertexes, double **matrix, double parameter);
void shift_left(int count_of_vertexes, double **matrix, double parameter);
void shift_up(int count_of_vertexes, double **matrix, double parameter);
void shift_forward(int count_of_vertexes, double **matrix, double parameter);
void rotation_by_x(int count_of_vertexes, double **matrix, double parameter);
void rotation_by_y(int count_of_vertexes, double **matrix, double parameter);
void rotation_by_z(int count_of_vertexes, double **matrix, double parameter);

void count_file_data(char *path, size_t *count_vertices, size_t *count_edges);
void load_model(double ***vertexes, size_t *size_vertexes, int ***edges,
                size_t *size_edges, char *path);
void clear_memory(double ***vertexes, size_t size_vertexes, int ***edges,
                  size_t size_edges);
void free_memory(double **arr, int rows);
double **allocateMemory(int rows, int cols);

#endif  // VIEWER_H
