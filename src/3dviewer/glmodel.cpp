#include "glmodel.h"

glModel::glModel(QWidget* parent) : QOpenGLWidget(parent) {
  v = NULL;
  f = NULL;

  paramsClearing();

  if (!(QFile::exists("settings.ini"))) {
    settings = new QSettings("settings.ini", QSettings::IniFormat, this);

    settings->beginGroup("Settings");

    settings->setValue("proj_type", 0);
    settings->setValue("facet_type", 1);
    settings->setValue("vertice_type", 1);
    settings->setValue("facet_r", 1.0);
    settings->setValue("facet_g", 1.0);
    settings->setValue("facet_b", 1.0);
    settings->setValue("facet_width", 2.0);
    settings->setValue("vertice_r", 0.0);
    settings->setValue("vertice_g", 0.9);
    settings->setValue("vertice_b", 0.0);
    settings->setValue("vertice_size", 8.0);
    settings->setValue("backgr_r", 0);
    settings->setValue("backgr_g", 0);
    settings->setValue("backgr_b", 0);

    settings->endGroup();
  }

  settings = new QSettings("settings.ini", QSettings::IniFormat, this);
  settings->beginGroup("Settings");

  proj_type = settings->value("proj_type").toInt();
  facet_type = settings->value("facet_type").toInt();
  vertice_type = settings->value("vertice_type").toInt();

  facet_r = settings->value("facet_r").toFloat();
  facet_g = settings->value("facet_g").toFloat();
  facet_b = settings->value("facet_b").toFloat();
  facet_width = settings->value("facet_width").toFloat();

  backgr_r = settings->value("backgr_r").toFloat();
  backgr_g = settings->value("backgr_g").toFloat();
  backgr_b = settings->value("backgr_b").toFloat();

  vertice_r = settings->value("vertice_r").toFloat();
  vertice_g = settings->value("vertice_g").toFloat();
  vertice_b = settings->value("vertice_b").toFloat();
  vertice_size = settings->value("vertice_size").toFloat();

  settings->endGroup();
}

glModel::~glModel() {
  settings->beginGroup("Settings");

  settings->setValue("proj_type", proj_type);
  settings->setValue("facet_type", facet_type);
  settings->setValue("vertice_type", vertice_type);
  settings->setValue("facet_r", facet_r);
  settings->setValue("facet_g", facet_g);
  settings->setValue("facet_b", facet_b);
  settings->setValue("facet_width", facet_width);
  settings->setValue("vertice_r", vertice_r);
  settings->setValue("vertice_g", vertice_g);
  settings->setValue("vertice_b", vertice_b);
  settings->setValue("vertice_size", vertice_size);
  settings->setValue("backgr_r", backgr_r);
  settings->setValue("backgr_g", backgr_g);
  settings->setValue("backgr_b", backgr_b);

  settings->endGroup();

  if (v) delete[] v;
  if (f) delete[] f;
}

void glModel::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glLineStipple(1, 0x0F0F);
}

void glModel::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void glModel::paintGL() {
  glClearColor(backgr_r, backgr_g, backgr_b, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (this->v) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (proj_type) {
      glFrustum(-1, 1, -1, 1, 1, 40);
      glTranslated(0, 0, -3);
    } else {
      glOrtho(-4, 4, -4, 4, 4, 40);
      glTranslated(0, 0, -12);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0, 0, 0);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glRotatef(zRot, 0, 0, 1);
    // glScaled(0.003, 0.003, 0.003);
    glVertexPointer(3, GL_DOUBLE, 0, this->v);
    glEnableClientState(GL_VERTEX_ARRAY);  // ----------------------

    glColor3d(vertice_r, vertice_g, vertice_b);  // points
    glPointSize(vertice_size);
    if (vertice_type == 1) glEnable(GL_POINT_SMOOTH);
    if (vertice_type) glDrawArrays(GL_POINTS, 0, v_count);
    if (vertice_type == 1) glDisable(GL_POINT_SMOOTH);

    glColor3d(facet_r, facet_g, facet_b);  // edges
    glLineWidth(facet_width);

    if (facet_type) glEnable(GL_LINE_STIPPLE);
    if (this->f)
      glDrawElements(GL_LINES, this->f_count, GL_UNSIGNED_INT, this->f);
    if (facet_type) glDisable(GL_LINE_STIPPLE);

    glDisableClientState(GL_VERTEX_ARRAY);  // ----------------------
  }
}

void glModel::mousePressEvent(QMouseEvent* m) { mousePos = m->pos(); }

void glModel::mouseMoveEvent(QMouseEvent* m) {
  xRot = 1 / M_PI * (m->pos().y() - mousePos.y()) + xcopyRot;
  yRot = 1 / M_PI * (m->pos().x() - mousePos.x()) + ycopyRot;
  update();
}

void glModel::mouseReleaseEvent(QMouseEvent* m) {
  xcopyRot = (float)m->pos().x();
  xcopyRot = xRot;
  ycopyRot = yRot;
}

void glModel::paramsClearing() {
  xcopyRot = ycopyRot = 0;
  xRot = yRot = zRot = 0;
  xPos = yPos = zPos = scale = xR = yR = zR = 0;
}
