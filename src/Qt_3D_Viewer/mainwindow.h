#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QColorDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <cstring>
#include <sstream>
#include <vector>

#include "customqopenglwidget.h"
#include "qgifimage.h"
#define VOBJLENGTH 3

#define VERTEXNONE 15
#define VERTEXCIRCLE 25
#define VERTEXSQUARE 35

extern "C" {
// #include "../Core/Viewer.h"
#include "../Core/Viewer.h"
}

QT_BEGIN_NAMESPACE namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  CustomQOpenGLWidget* glWidget = nullptr;
  // void read_file_count_values(char* path);
  void read_file_build_matrices(char* path);
  QString selectedFile = nullptr;
  size_t count_vertices = 0, count_edges = 0;
  void allocate_memory(double*** simple_vertixes);
  void update_and_clear_memory(double*** simple_vertixes);
  double modelWidth = 0;
  double modelHeight = 0;
  int current_combobox_ind = 0;

  std::vector<std::vector<double> > vertixes = {{0}};
  std::vector<std::vector<int> > edges = {{0}};

  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_pushButton_clicked();
  void on_scaling_smaller_button_clicked();
  void on_scaling_bigger_button_clicked();
  void on_shift_left_button_clicked();
  void on_shift_right_clicked();
  void on_pushButton_up_clicked();
  void on_pushButton_down_clicked();
  void on_shift_forward_button_clicked();
  void on_shift_backward_button_clicked();
  void on_rotate_up_button_clicked();
  void on_rotate_down_button_clicked();
  void on_rotate_left_button_left_clicked();
  void on_rotate_right_button_right_clicked();
  void on_rotate_clockwise_button_clicked();
  void on_rotate_counterclockwise_button_clicked();

  void on_pushButton_perspective_clicked();

  void on_pushButton_parallel_clicked();

  void on_pushButton_Select_clicked();

  void on_dashed_button_clicked();

  void on_horizontalSlider_width_valueChanged(int value);

  void on_pushButton_Select_line_clicked();

  void on_horizontalSlider_vertex_width_valueChanged(int value);

  void on_comboBox_currentIndexChanged(int index);

  void on_pushButton_Select_vertex_color_clicked();

  void on_screenshot_btn_clicked();

  void on_record_btn_clicked();

  void set_gif();

 private:
  QString gif_name;
  QGifImage* gif_frame;
  int frames_counter = 0;
  QTimer* timer;
  Ui::MainWindow* ui;
};
#endif  // MAINWINDOW_H
