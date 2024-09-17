#include "customqopenglwidget.h"

CustomQOpenGLWidget::CustomQOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
  load_settings();
}

CustomQOpenGLWidget::~CustomQOpenGLWidget() { save_settings(); }

void CustomQOpenGLWidget::save_settings() {
  QSettings settings("pacani_anime", "3DViewer");

  settings.setValue("key_color_red", red);
  settings.setValue("key_color_green", green);
  settings.setValue("key_color_blue", blue);
  settings.setValue("key_color_alpha", alpha);

  settings.setValue("key_color_red_edge", red_edge);
  settings.setValue("key_color_green_edge", green_edge);
  settings.setValue("key_color_blue_edge", blue_edge);
  settings.setValue("key_color_alpha_edge", alpha_edge);

  settings.setValue("key_line_width", lineWidth);
  settings.setValue("key_vertex_type", vertex_type);
  settings.setValue("key_vertex_size", vertex_size);

  settings.setValue("key_color_red_ver", red_ver);
  settings.setValue("key_color_green_ver", green_ver);
  settings.setValue("key_color_blue_ver", blue_ver);
  settings.setValue("key_color_alpha_ver", alpha_ver);
}

void CustomQOpenGLWidget::load_settings() {
  QSettings settings("pacani_anime", "3DViewer");

  red = settings.value("key_color_red").toFloat();
  green = settings.value("key_color_green").toFloat();
  blue = settings.value("key_color_blue").toFloat();
  alpha = settings.value("key_color_alpha").toFloat();

  lineWidth = settings.value("key_line_width").toFloat();

  red_edge = settings.value("key_color_red_edge").toFloat();
  green_edge = settings.value("key_color_green_edge").toFloat();
  blue_edge = settings.value("key_color_blue_edge").toFloat();
  alpha_edge = settings.value("key_color_alpha_edge").toFloat();

  vertex_type = settings.value("key_vertex_type").toInt();
  vertex_size = settings.value("key_vertex_size").toInt();

  red_ver = settings.value("key_color_red_ver").toFloat();
  green_ver = settings.value("key_color_green_ver").toFloat();
  blue_ver = settings.value("key_color_blue_ver").toFloat();
  alpha_ver = settings.value("key_color_alpha_ver").toFloat();
}

void CustomQOpenGLWidget::set_vectors(
    std::vector<std::vector<double>> given_vertixes,
    std::vector<std::vector<int>> given_edges) {
  custom_vers.resize(given_vertixes.size() - 1, std::vector<double>());
  custom_edges.resize(given_edges.size() - 1, std::vector<int>());

  for (size_t i = 1; i < given_vertixes.size(); i++) {
    std::vector<double> pushing_vec = {
        given_vertixes[i][0], given_vertixes[i][1], given_vertixes[i][2]};
    custom_vers[i - 1] = pushing_vec;
  }

  for (size_t i = 1; i < given_edges.size(); i++) {
    custom_edges[i - 1].clear();
    for (size_t j = 1; j < given_edges[i].size(); j++) {
      custom_edges[i - 1].push_back(given_edges[i][j]);
    }
  }
}

void CustomQOpenGLWidget::initializeGL() {
  glClearColor(red, green, blue, alpha);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glRotatef(30, 1.0f, 0.0f, 0.0f);
  glRotatef(-30, 0.0f, 1.0f, 0.0f);
}

void CustomQOpenGLWidget::paintGL() {
  if (line_stripple == LINESOLID) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 30);
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double max_model_size = custom_modelHeight;
  if (custom_modelWidth > max_model_size) max_model_size = custom_modelWidth;

  if (projection_type == PERSPECTIVE) {
    // Установка центральной проекции
    gluPerspective(100, (custom_modelWidth / (double)custom_modelHeight), 0, 2);
  } else if (projection_type == FRUSTUM) {
    glFrustum(-1, 1, -1, 1, 1, 1);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(30, 1.0f, 0.0f, 0.0f);
  glRotatef(-30, 0.0f, 1.0f, 0.0f);

  glClearColor(red, green, blue, alpha);  // фон

  // Очистка экрана и установка цвета фона
  glClear(GL_COLOR_BUFFER_BIT);
  // Черный цвет

  // Установка ширины линии
  glLineWidth(lineWidth / 15);

  // Установка цвета вершин
  glColor4f(red_ver, green_ver, blue_ver, alpha_ver);

  // Перебор вершин с отрисовкой
  if (vertex_type == VERTEXSQUARE) {
    glPointSize(0.5 * vertex_size);
    for (size_t i = 1; i < custom_vers.size(); i++) {
      std::vector<double> ver = custom_vers[i];
      glBegin(GL_POINTS);
      glVertex3d(ver[0], ver[1], ver[2]);
      glEnd();
    }
  }

  else if (vertex_type == VERTEXCIRCLE) {
    int numSegments = 20;  // Number of segments to approximate the circle
    float radius = 0.0009 * vertex_size;  // Radius of the circle

    for (size_t i = 1; i < custom_vers.size(); i++) {
      std::vector<double> ver = custom_vers[i];

      glBegin(GL_POLYGON);  // Start drawing a polygon
      for (int j = 0; j < numSegments; j++) {
        float angle = 2.0f * M_PI * float(j) /
                      float(numSegments);  // 360 degrees in radians
        float x = radius * cosf(angle);    // Calculate the x component
        float y = radius * sinf(angle);    // Calculate the y component

        glVertex3f(ver[0] + x, ver[1] + y, ver[2]);  // Output vertex
      }
      glEnd();  // Done drawing the polygon (circle)
    }
  }

  // Установка цвета линии
  glColor4f(red_edge, green_edge, blue_edge, alpha_edge);

  // Перебор всех рёбер и отрисовка линий
  for (const auto& edge : custom_edges) {
    if (edge.size() < 2) continue;  // Нужно минимум две вершины для ребра

    glBegin(GL_LINE_LOOP);  // Начало отрисовки линий
    for (size_t i = 1; i < edge.size(); ++i) {
      // Получение индексов вершин
      size_t startIdx = edge[i - 1] - 1;  // Индексы в OBJ начинаются с 1
      size_t endIdx = edge[i] - 1;

      // Отрисовка линии между вершинами
      if ((startIdx < custom_vers.size()) && (endIdx < custom_vers.size())) {
        glVertex3dv(custom_vers[startIdx].data());
        glVertex3dv(custom_vers[endIdx].data());
      }
    }
    glEnd();
  }
}

void CustomQOpenGLWidget::setParallelProjection(double getmodelWidth,
                                                double getmodelHeight) {
  custom_modelWidth = getmodelWidth;
  custom_modelHeight = getmodelHeight;
  projection_type = FRUSTUM;
}
void CustomQOpenGLWidget::setPerspectiveProjection(double getmodelWidth,
                                                   double getmodelHeight) {
  custom_modelWidth = getmodelWidth;
  custom_modelHeight = getmodelHeight;
  projection_type = PERSPECTIVE;
}

void CustomQOpenGLWidget::setColor(float get_red, float get_green,
                                   float get_blue, float get_alpha) {
  red = get_red;
  green = get_green;
  blue = get_blue;
  alpha = get_alpha;
  update();
}

void CustomQOpenGLWidget::setColorLine(float get_red, float get_green,
                                       float get_blue, float get_alpha) {
  red_edge = get_red;
  green_edge = get_green;
  blue_edge = get_blue;
  alpha_edge = get_alpha;
  update();
}

void CustomQOpenGLWidget::setVertexColor(float get_red, float get_green,
                                         float get_blue, float get_alpha) {
  red_ver = get_red;
  green_ver = get_green;
  blue_ver = get_blue;
  alpha_ver = get_alpha;
  update();
}

QImage CustomQOpenGLWidget::takeScreenshot() { return grabFramebuffer(); }
