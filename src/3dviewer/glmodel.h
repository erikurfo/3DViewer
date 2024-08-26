#ifndef GLMODEL_H
#define GLMODEL_H

#include <QFile>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QSettings>
#include <QTimer>

class glModel : public QOpenGLWidget {
 public:
  double* v;
  unsigned int* f;
  unsigned int v_count, f_count;

  int proj_type;
  int facet_type;
  int vertice_type;
  float facet_r, facet_g, facet_b, facet_width;
  float vertice_r, vertice_g, vertice_b, vertice_size;
  float backgr_r, backgr_g, backgr_b;

  float xRot, yRot, zRot, xcopyRot, ycopyRot;
  double xPos, yPos, zPos, scale, xR, yR, zR;

  explicit glModel(QWidget* parent = 0);
  void paramsClearing();

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  ~glModel();

 private:
  QPoint mousePos;
  void mousePressEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  QSettings* settings;
};

#endif  // GLMODEL_H
