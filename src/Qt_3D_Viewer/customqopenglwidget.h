#ifndef CUSTOMQOPENGLWIDGET_H
#define CUSTOMQOPENGLWIDGET_H

#include <GL/glu.h>
#include <math.h>

#include <QImage>
#include <QOpenGLWidget>
#include <QSettings>
#include <limits>

#define FRUSTUM 2
#define PERSPECTIVE 1

#define LINESTRIPPLED 3
#define LINESOLID 4

#define VERTEXNONE 15
#define VERTEXCIRCLE 25
#define VERTEXSQUARE 35

class CustomQOpenGLWidget : public QOpenGLWidget {
 public:
  explicit CustomQOpenGLWidget(QWidget *parent);
  std::vector<std::vector<double> > custom_vers = {{0}};
  std::vector<std::vector<int> > custom_edges = {{0}};
  void set_vectors(std::vector<std::vector<double> > given_vertixes,
                   std::vector<std::vector<int> > given_edges);
  void setParallelProjection(double getmodelWidth, double getmodelHeight);
  void setPerspectiveProjection(double getmodelWidth, double getmodelHeight);
  void setColor(float get_red, float get_green, float get_blue,
                float get_alpha);
  void setColorLine(float get_red, float get_green, float get_blue,
                    float get_alpha);
  void setVertexColor(float get_red, float get_green, float get_blue,
                      float get_alpha);
  double custom_modelWidth = 0;
  double custom_modelHeight = 0;
  float red = 1.0f;
  float green = 1.0f;
  float blue = 1.0f;
  float alpha = 1.0f;

  float red_edge = 1.0f;
  float green_edge = 1.0f;
  float blue_edge = 1.0f;
  float alpha_edge = 1.0f;

  float red_ver = 1.0f;
  float green_ver = 1.0f;
  float blue_ver = 1.0f;
  float alpha_ver = 1.0f;

  int projection_type = FRUSTUM;
  int line_stripple = LINESOLID;
  void save_settings();
  void load_settings();
  int lineWidth = 1;

  int vertex_type = VERTEXNONE;
  int vertex_size = 1;
  QImage takeScreenshot();

  ~CustomQOpenGLWidget();

 protected:
  void paintGL() override;
  void initializeGL() override;
};

#endif  // CUSTOMQOPENGLWIDGET_H
