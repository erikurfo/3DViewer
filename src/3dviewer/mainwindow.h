#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "glmodel.h"

extern "C" {
#include "../parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_pushButton_backgr_rgb_clicked();

  void on_pushButton_facet_rgb_clicked();

  void on_pushButton_vertices_rgb_clicked();

  void on_pushButton_load_clicked();

  void on_doubleSpinBox_sizw_vertice_valueChanged(double arg1);

  void on_comboBox_facet_type_currentIndexChanged(int index);

  void on_doubleSpinBox_size_vertice_valueChanged(double arg1);

  void on_comboBox_vertice_type_currentIndexChanged(int index);

  void on_horizontalSlider_moveY_sliderMoved(int position);

  void on_horizontalSlider_moveY_sliderReleased();

  void on_horizontalSlider_moveX_sliderMoved(int position);

  void on_horizontalSlider_moveX_sliderReleased();

  void on_horizontalSlider_moveZ_sliderMoved(int position);

  void on_horizontalSlider_moveZ_sliderReleased();

  void on_horizontalSlider_scale_sliderMoved(int position);

  void on_horizontalSlider_scale_sliderReleased();

  void on_horizontalSlider_rotateX_sliderMoved(int position);

  void on_horizontalSlider_rotateX_sliderReleased();

  void on_horizontalSlider_rotateY_valueChanged(int value);

  void on_horizontalSlider_rotateY_sliderReleased();

  void on_horizontalSlider_rotateZ_sliderMoved(int position);

  void on_horizontalSlider_rotateZ_sliderReleased();

  void on_radioButto_parallel_clicked();

  void on_radioButton_Perspective_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_doubleSpinBox_x_editingFinished();

  void on_doubleSpinBox_y_editingFinished();

  void on_doubleSpinBox_z_editingFinished();

  void on_doubleSpinBox_xR_editingFinished();

  void on_doubleSpinBox_yR_editingFinished();

  void on_doubleSpinBox_zR_editingFinished();

  void on_doubleSpinBox_sc_editingFinished();

 private:
  Ui::MainWindow *ui;
  QImage *frame;
  QTimer *timer;
  int frame_num;

 private slots:
  void save_frame();
};
#endif  // MAINWINDOW_H
