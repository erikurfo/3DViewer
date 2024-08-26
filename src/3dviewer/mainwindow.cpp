#include "mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMovie>
#include <QtMath>

#include "gif.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(size());

  ui->comboBox_facet_type->setCurrentIndex(ui->widget_model->facet_type);
  ui->doubleSpinBox_sizw_vertice->setValue(ui->widget_model->facet_width);
  ui->comboBox_vertice_type->setCurrentIndex(ui->widget_model->vertice_type);
  ui->doubleSpinBox_size_vertice->setValue(ui->widget_model->vertice_size);

  if (ui->comboBox_vertice_type->currentIndex() == 0)
    ui->doubleSpinBox_size_vertice->setEnabled(false);

  if (ui->widget_model->proj_type)
    ui->radioButton_Perspective->setChecked(true);
  else
    ui->radioButto_parallel->setChecked(true);
}

MainWindow::~MainWindow() { delete ui; }
GifWriter my_gif = {};

void MainWindow::on_pushButton_backgr_rgb_clicked() {
  QColor backgr_rgb =
      QColorDialog::getColor(Qt::white, this, "Pick background color");
  if (backgr_rgb.isValid()) {
    backgr_rgb.getRgbF(&(ui->widget_model->backgr_r),
                       &(ui->widget_model->backgr_g),
                       &(ui->widget_model->backgr_b));
    ui->widget_model->update();
  }
}

void MainWindow::on_pushButton_facet_rgb_clicked() {
  QColor facet_rgb =
      QColorDialog::getColor(Qt::black, this, "Pick facets color");
  if (facet_rgb.isValid()) {
    facet_rgb.getRgbF(&(ui->widget_model->facet_r),
                      &(ui->widget_model->facet_g),
                      &(ui->widget_model->facet_b));
    ui->widget_model->update();
  }
}

void MainWindow::on_pushButton_vertices_rgb_clicked() {
  QColor vertice_rgb =
      QColorDialog::getColor(Qt::red, this, "Pick vertices color");
  if (vertice_rgb.isValid()) {
    vertice_rgb.getRgbF(&(ui->widget_model->vertice_r),
                        &(ui->widget_model->vertice_g),
                        &(ui->widget_model->vertice_b));
    ui->widget_model->update();
  }
}

void MainWindow::on_pushButton_load_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Open Object File"), "../../", "Obj files (*.obj)");

  if (!(filename.isEmpty())) {
    char *c_str = new char[filename.length() + 1];

    for (int i = 0; i < filename.length(); i++) {
      c_str[i] = filename.toStdString().c_str()[i];
    }
    c_str[filename.length()] = '\0';

    data_t model_data = parser(c_str);

    ui->widget_model->paramsClearing();

    ui->widget_model->f_count = fs_size(model_data);
    ui->widget_model->v_count = model_data.count_of_vertexes;
    ui->widget_model->v = new double[ui->widget_model->v_count * 3];
    ui->widget_model->f = new unsigned int[ui->widget_model->f_count];
    vs_array(model_data, ui->widget_model->v);
    fs_array(model_data, ui->widget_model->f);

    double maximum = 0;
    for (unsigned int i = 0; i < ui->widget_model->v_count * 3; i++) {
      if (qFabs(ui->widget_model->v[i]) > maximum)
        maximum = qFabs(ui->widget_model->v[i]);
    }
    if (maximum > 2) {
      for (unsigned int i = 0; i < ui->widget_model->v_count * 3; i++) {
        ui->widget_model->v[i] = ui->widget_model->v[i] / maximum * 4;
      }
    }

    ui->textEdit_fileInfo->setText(
        "Model: " +
        filename.last(filename.length() - filename.lastIndexOf("/") - 1) +
        "\n" + "Vertices: " + QString::number(ui->widget_model->v_count) +
        "\n" + "Facets: " + QString::number(model_data.count_of_facets));

    for (unsigned int i = 0; i < model_data.count_of_facets; i++)
      free(model_data.polygons[i].vertexes);
    free(model_data.polygons);
    s21_remove_matrix(&model_data.matrix_3d);
  } else {
    if (!(ui->widget_model->v)) ui->textEdit_fileInfo->setText("No file");
  }
}

void MainWindow::on_doubleSpinBox_sizw_vertice_valueChanged(double arg1) {
  ui->widget_model->facet_width = arg1;
  ui->widget_model->update();
}

void MainWindow::on_comboBox_facet_type_currentIndexChanged(int index) {
  ui->widget_model->facet_type = index;
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_size_vertice_valueChanged(double arg1) {
  ui->widget_model->vertice_size = arg1;
  ui->widget_model->update();
}

void MainWindow::on_comboBox_vertice_type_currentIndexChanged(int index) {
  ui->widget_model->vertice_type = index;
  if (index) {
    ui->doubleSpinBox_size_vertice->setEnabled(true);
  } else {
    ui->doubleSpinBox_size_vertice->setEnabled(false);
  }
  ui->widget_model->update();
}

void MainWindow::on_horizontalSlider_moveY_sliderMoved(int position) {
  move_y(ui->widget_model->v, (position - ui->widget_model->yPos) / 100,
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->yPos = position;
}

void MainWindow::on_horizontalSlider_moveY_sliderReleased() {
  ui->widget_model->yPos = 0;
  ui->horizontalSlider_moveY->setValue(0);
}

void MainWindow::on_horizontalSlider_moveX_sliderMoved(int position) {
  move_x(ui->widget_model->v, ((double)position - ui->widget_model->xPos) / 100,
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->xPos = (double)position;
}

void MainWindow::on_horizontalSlider_moveX_sliderReleased() {
  ui->widget_model->xPos = 0;
  ui->horizontalSlider_moveX->setValue(0);
}

void MainWindow::on_horizontalSlider_moveZ_sliderMoved(int position) {
  move_z(ui->widget_model->v, ((double)position - ui->widget_model->zPos) / 100,
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->zPos = (double)position;
}

void MainWindow::on_horizontalSlider_moveZ_sliderReleased() {
  ui->widget_model->zPos = 0;
  ui->horizontalSlider_moveZ->setValue(0);
}

void MainWindow::on_horizontalSlider_scale_sliderMoved(int position) {
  if ((double)position - ui->widget_model->scale > 0)
    scale(ui->widget_model->v, 1.2, ui->widget_model->v_count * 3);
  else
    scale(ui->widget_model->v, 0.8, ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->scale = (double)position;
}

void MainWindow::on_horizontalSlider_scale_sliderReleased() {
  ui->widget_model->scale = 0;
  ui->horizontalSlider_scale->setValue(0);
}

void MainWindow::on_horizontalSlider_rotateX_sliderMoved(int position) {
  if ((double)position - ui->widget_model->xR > 0)
    rotation_by_ox(ui->widget_model->v, 10, ui->widget_model->v_count * 3);
  else
    rotation_by_ox(ui->widget_model->v, -10, ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->xR = (double)position;
}

void MainWindow::on_horizontalSlider_rotateX_sliderReleased() {
  ui->widget_model->xR = 0;
  ui->horizontalSlider_rotateX->setValue(0);
}

void MainWindow::on_horizontalSlider_rotateY_valueChanged(int value) {
  if ((double)value - ui->widget_model->yR > 0)
    rotation_by_oy(ui->widget_model->v, 10, ui->widget_model->v_count * 3);
  else
    rotation_by_oy(ui->widget_model->v, -10, ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->yR = (double)value;
}

void MainWindow::on_horizontalSlider_rotateY_sliderReleased() {
  ui->widget_model->yR = 0;
  ui->horizontalSlider_rotateY->setValue(0);
}

void MainWindow::on_horizontalSlider_rotateZ_sliderMoved(int position) {
  if ((double)position - ui->widget_model->zR > 0)
    rotation_by_oz(ui->widget_model->v, 10, ui->widget_model->v_count * 3);
  else
    rotation_by_oz(ui->widget_model->v, -10, ui->widget_model->v_count * 3);
  ui->widget_model->update();
  ui->widget_model->zR = (double)position;
}

void MainWindow::on_horizontalSlider_rotateZ_sliderReleased() {
  ui->widget_model->zR = 0;
  ui->horizontalSlider_rotateZ->setValue(0);
}

void MainWindow::on_radioButto_parallel_clicked() {
  ui->widget_model->proj_type = 0;
  ui->widget_model->update();
}

void MainWindow::on_radioButton_Perspective_clicked() {
  ui->widget_model->proj_type = 1;
  ui->widget_model->update();
}

void MainWindow::on_pushButton_clicked() {
  int count = 1;
  QImage *bmp = new QImage;
  QString filename = "screenshot1.bmp";

  while (QFile::exists(filename)) {
    filename.replace(QString::number(count), QString::number(count + 1));
    count++;
  }

  *bmp = ui->widget_model->grabFramebuffer();
  bmp->save(filename, "BMP");
  delete bmp;
}

void MainWindow::on_pushButton_2_clicked() {
  int count = 1;
  QImage *jpeg = new QImage;
  QString filename = "screenshot1.jpeg";

  while (QFile::exists(filename)) {
    filename.replace(QString::number(count), QString::number(count + 1));
    count++;
  }

  *jpeg = ui->widget_model->grabFramebuffer();
  jpeg->save(filename, "JPEG");
  delete jpeg;
}

void MainWindow::on_pushButton_3_clicked() {
  frame = new QImage;
  timer = new QTimer(this);
  int count = 1;
  frame_num = 0;
  QString filename = "screencast1.gif";

  while (QFile::exists(filename)) {
    filename.replace(QString::number(count), QString::number(count + 1));
    count++;
  }

  char *c_str = new char[filename.length() + 1];

  for (int i = 0; i < filename.length(); i++) {
    c_str[i] = filename.toStdString().c_str()[i];
  }
  c_str[filename.length()] = '\0';

  GifBegin(&my_gif, c_str, 640, 480, 10);

  connect(timer, SIGNAL(timeout()), this, SLOT(save_frame()));
  timer->start(100);

  delete[] c_str;
}

void MainWindow::save_frame() {
  frame_num++;

  *frame = ui->widget_model->grabFramebuffer();
  *frame =
      frame->scaled(640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  uchar temp;
  for (int i = 0; i < 1228800; i += 4) {
    temp = frame->bits()[i];
    frame->bits()[i] = frame->bits()[i + 2];
    frame->bits()[i + 2] = temp;
  }

  GifWriteFrame(&my_gif, frame->bits(), 640, 480, 10);

  if (frame_num == 50) {
    timer->stop();
    GifEnd(&my_gif);
    delete frame;
  }
}

void MainWindow::on_doubleSpinBox_x_editingFinished() {
  move_x(ui->widget_model->v, ui->doubleSpinBox_x->value(),
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_y_editingFinished() {
  move_y(ui->widget_model->v, ui->doubleSpinBox_y->value(),
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_z_editingFinished() {
  move_z(ui->widget_model->v, ui->doubleSpinBox_z->value(),
         ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_xR_editingFinished() {
  rotation_by_ox(ui->widget_model->v, ui->doubleSpinBox_xR->value(),
                 ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_yR_editingFinished() {
  rotation_by_oy(ui->widget_model->v, ui->doubleSpinBox_yR->value(),
                 ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_zR_editingFinished() {
  rotation_by_oz(ui->widget_model->v, ui->doubleSpinBox_zR->value(),
                 ui->widget_model->v_count * 3);
  ui->widget_model->update();
}

void MainWindow::on_doubleSpinBox_sc_editingFinished() {
  scale(ui->widget_model->v, ui->doubleSpinBox_sc->value(),
        ui->widget_model->v_count * 3);
  ui->widget_model->update();
}
