#include "mainwindow.h"

#include <QtWidgets/QFileDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  glWidget = ui->OurCustomWidget;

  QSettings settings("pacani_anime", "3DViewer");
  ui->horizontalSlider_width->setValue(
      settings.value("key_line_width").toFloat());
  ui->horizontalSlider_vertex_width->setValue(
      settings.value("key_vertex_size").toFloat());

  if (glWidget->vertex_type == VERTEXNONE) {
    ui->comboBox->setCurrentIndex(0);
  } else if (glWidget->vertex_type == VERTEXSQUARE) {
    ui->comboBox->setCurrentIndex(1);
  } else if (glWidget->vertex_type == VERTEXCIRCLE) {
    ui->comboBox->setCurrentIndex(2);
  }
  timer = new QTimer(0);
  connect(timer, SIGNAL(timeout()), this, SLOT(set_gif()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::read_file_build_matrices(char* path) {
  double** simple_vertixes_load = nullptr;
  int** simple_edges_load = nullptr;
  load_model(&simple_vertixes_load, &count_vertices, &simple_edges_load,
             &count_edges, path);
  vertixes = {{0}};
  edges = {{0}};

  vertixes.resize(count_vertices + 1, std::vector<double>(0));
  edges.resize(count_edges + 1, std::vector<int>(0));

  for (size_t i = 0; i < count_vertices + 1; i++) {
    for (int j = 0; j < 3; j++) {
      vertixes[i].push_back(simple_vertixes_load[i][j]);
    }
  }
  for (size_t i = 0; i < count_edges + 1; i++) {
    for (int j = 0; j < simple_edges_load[i][0] + 1; j++) {
      edges[i].push_back(simple_edges_load[i][j]);
    }
  }
  clear_memory(&simple_vertixes_load, count_vertices, &simple_edges_load,
               count_edges);
}

double min(double val1, double val2) { return val1 ? val1 < val2 : val2; }
double max(double val1, double val2) { return val1 ? val1 > val2 : val2; }

void MainWindow::on_pushButton_clicked() {
  selectedFile = QFileDialog::getOpenFileName(nullptr, "Open File");
  if (selectedFile == "") return;
  QByteArray arr_8bit = selectedFile.toLocal8Bit();
  char* str = arr_8bit.data();
  char* last_slash = nullptr;
  last_slash = strrchr(str, '/');
  char* filename = last_slash + 1;
  ui->label_filename->setText(filename);

  count_vertices = 0;
  count_edges = 0;

  count_file_data(str, &count_vertices, &count_edges);

  std::string str_count_vertices, str_count_edges;
  std::stringstream ss;
  ss << count_vertices;
  str_count_vertices = ss.str();
  ss.str(std::string());

  ss << count_edges;
  str_count_edges = ss.str();
  ui->label_vertices->setText(str_count_vertices.c_str());

  ui->label_edges->setText(str_count_edges.c_str());

  read_file_build_matrices(str);

  // Получение размеров модели
  double minX = std::numeric_limits<double>::max();
  double minY = std::numeric_limits<double>::max();
  double minZ = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::min();
  double maxY = std::numeric_limits<double>::min();
  double maxZ = std::numeric_limits<double>::min();

  for (const auto& vertex : vertixes) {
    minX = std::min(minX, vertex[0]);
    minY = std::min(minY, vertex[1]);
    minZ = std::min(minZ, vertex[2]);
    maxX = std::max(maxX, vertex[0]);
    maxY = std::max(maxY, vertex[1]);
    maxZ = std::max(maxZ, vertex[2]);
  }

  // Вычисление размеров модели
  modelWidth = maxX - minX;
  modelHeight = maxY - minY;
  double modelDepth = maxZ - minZ;

  // Определение масштабного коэффициента на основе размеров модели
  double maxModelSize = std::max(std::max(modelWidth, modelHeight), modelDepth);
  double modelScale = 1.0f / maxModelSize;

  for (size_t i = 0; i < count_vertices + 1; i++) {
    for (int j = 0; j < 3; j++) {
      vertixes[i][j] *= modelScale;
    }
  }

  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::allocate_memory(double*** simple_vertixes) {
  (*simple_vertixes) = (double**)calloc(count_vertices + 1, sizeof(double*));

  for (size_t i = 0; i < count_vertices + 1; i++) {
    (*simple_vertixes)[i] = (double*)calloc(VOBJLENGTH, sizeof(double));
    (*simple_vertixes)[i][0] = vertixes[i][0];
    (*simple_vertixes)[i][1] = vertixes[i][1];
    (*simple_vertixes)[i][2] = vertixes[i][2];
  }
}

void MainWindow::update_and_clear_memory(double*** simple_vertixes) {
  for (size_t i = 0; i < count_vertices + 1; i++) {
    for (int j = 0; j < 3; j++) {
      vertixes[i][j] = (*simple_vertixes)[i][j];
    }
  }

  for (size_t i = 0; i < count_vertices + 1; i++) {
    free((*simple_vertixes)[i]);
  }
  free(*simple_vertixes);
}

void MainWindow::on_scaling_smaller_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_scale->value();

  scaling_smaller(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_scaling_bigger_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_scale->value();

  scaling_bigger(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_shift_left_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_spinbox->value();

  shift_left(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_shift_right_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_spinbox->value();

  shift_left(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_pushButton_up_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_up_down_spinbox->value();

  shift_up(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_pushButton_down_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_up_down_spinbox->value();

  shift_up(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_shift_forward_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_z_spinbox->value();

  shift_forward(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_shift_backward_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->shift_z_spinbox->value();

  shift_forward(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_up_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_up_down->value();

  rotation_by_x(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_down_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_up_down->value();

  rotation_by_x(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_left_button_left_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_left_right->value();

  rotation_by_y(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_right_button_right_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_left_right->value();

  rotation_by_y(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_clockwise_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_wise->value();

  rotation_by_z(count_vertices, simple_vertixes, arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_rotate_counterclockwise_button_clicked() {
  double** simple_vertixes = nullptr;
  allocate_memory(&simple_vertixes);
  double arg = ui->doubleSpinBox_rotate_wise->value();

  rotation_by_z(count_vertices, simple_vertixes, -arg);

  update_and_clear_memory(&simple_vertixes);
  glWidget->set_vectors(vertixes, edges);
  glWidget->update();
}

void MainWindow::on_pushButton_perspective_clicked() {
  glWidget->setPerspectiveProjection(modelWidth, modelHeight);
  glWidget->update();
}

void MainWindow::on_pushButton_parallel_clicked() {
  glWidget->setParallelProjection(modelWidth, modelHeight);
  glWidget->update();
}

void MainWindow::on_pushButton_Select_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor();
  if (color.isValid()) {
    float red = color.red() / 255.0f;
    float green = color.green() / 255.0f;
    float blue = color.blue() / 255.0f;
    float alpha = color.alpha() / 255.0f;
    glWidget->setColor(red, green, blue, alpha);
    glWidget->update();
  }
}

void MainWindow::on_dashed_button_clicked() {
  glWidget->line_stripple =
      glWidget->line_stripple == LINESOLID ? LINESTRIPPLED : LINESOLID;
  glWidget->update();
}

void MainWindow::on_horizontalSlider_width_valueChanged(int value) {
  glWidget->lineWidth = value;
  glWidget->update();
}

void MainWindow::on_pushButton_Select_line_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor();
  if (color.isValid()) {
    float red = color.red() / 255.0f;
    float green = color.green() / 255.0f;
    float blue = color.blue() / 255.0f;
    float alpha = color.alpha() / 255.0f;
    glWidget->setColorLine(red, green, blue, alpha);
    glWidget->update();
  }
}

void MainWindow::on_horizontalSlider_vertex_width_valueChanged(int value) {
  glWidget->vertex_size = value;
  glWidget->update();
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
  if (index == 0) {
    glWidget->vertex_type = VERTEXNONE;
  } else if (index == 1) {
    glWidget->vertex_type = VERTEXSQUARE;
  } else if (index == 2) {
    glWidget->vertex_type = VERTEXCIRCLE;
  }
  glWidget->update();
}

void MainWindow::on_pushButton_Select_vertex_color_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor();
  if (color.isValid()) {
    float red = color.red() / 255.0f;
    float green = color.green() / 255.0f;
    float blue = color.blue() / 255.0f;
    float alpha = color.alpha() / 255.0f;
    glWidget->setVertexColor(red, green, blue, alpha);
    glWidget->update();
  }
}

void MainWindow::on_screenshot_btn_clicked() {
  QImage screenshot = glWidget->takeScreenshot();
  QString fileFilter =
      "JPEG Files (*.jpg *.jpeg);;BMP Files (*.bmp);;All Files (*)";
  QString file_name =
      QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", fileFilter);

  if (!file_name.isEmpty()) {
    QFileInfo fileInfo(file_name);
    QString file_extension = fileInfo.suffix().toLower();

    if (file_extension.isEmpty()) {
      file_name += ".jpg";
    } else if (!(file_extension == "jpg" || file_extension == "jpeg" ||
                 file_extension == "bmp")) {
      QMessageBox::warning(
          this, tr("Unsupported Format"),
          tr("Unsupported format selected. Please choose JPG or BMP."));
    }

    screenshot.save(file_name);
  }
}

void MainWindow::on_record_btn_clicked() {
  QString fileFilter = "GIF Files (*.gif);;All Files (*)";
  gif_name = QFileDialog::getSaveFileName(this, tr("Save a gif animation"), "",
                                          fileFilter);
  if (!gif_name.isEmpty()) {
    QFileInfo fileInfo(gif_name);
    QString file_extension = fileInfo.suffix().toLower();

    if (file_extension != "gif") {
      QMessageBox::warning(
          this, tr("Unsupported Format"),
          tr("Unsupported format selected. Please choose Gif."));
    } else {
      ui->record_btn->setDisabled(true);
      gif_frame = new QGifImage;
      gif_frame->setDefaultDelay(10);
      timer->setInterval(100);
      timer->start();
    }
  }
}

void MainWindow::set_gif() {
  QImage image = glWidget->grabFramebuffer();
  QSize image_size(640, 480);
  QImage scaled_image = image.scaled(image_size);
  gif_frame->addFrame(scaled_image);
  if (frames_counter == 50) {
    timer->stop();
    gif_frame->save(gif_name);
    frames_counter = 0;
    QMessageBox messageBoxGif;
    messageBoxGif.information(0, "", "Gif animation saved");
    delete gif_frame;
    ui->record_btn->setText("Start recording");
    ui->record_btn->setEnabled(true);
  }
  frames_counter++;
  if (!ui->record_btn->isEnabled()) {
    ui->record_btn->setText(QString::number(frames_counter / 10) + "s");
  }
}
