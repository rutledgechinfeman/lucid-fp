#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

GLWidget::GLWidget(QWidget *parent)
  : QGLWidget(parent)
{
    m_prevMousePos.x = 0.0; m_prevMousePos.y = 0.0;
    m_leftMouseDown = m_rightMouseDown = m_middleMouseDown = false;
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
}

GLWidget::~GLWidget()
{
  // Delete anything you have created
}

void GLWidget::setRoot(Feature* root){
    m_root = root;
}

QSize GLWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
  return QSize(400, 400);
}

void GLWidget::perspectiveCamera(int width, int height) {
    float ratio = width / static_cast<float>(height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy,ratio,m_camera.near,m_camera.far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera.eye.x,m_camera.eye.y,m_camera.eye.z,
              m_camera.center.x,m_camera.center.y,m_camera.center.z,
              m_camera.up.x,m_camera.up.y,m_camera.up.z);
}

void GLWidget::initializeGL()
{
  // Set the way we want things to look
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL()
{
    cout << m_camera.center << endl;
    /*this->perspectiveCamera(this->width(), this->height());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //Render the triangle ID Map.  If the mesh has really small triangles...they wont render to a pixel (darn)
    //So we need to zoom in, using the brush mask as our viewport before rendering the id map.
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;  GLdouble wx, wy, wz;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
    realy = viewport[3] - (GLint) m_prevMousePos.y - 1;
    gluUnProject ((GLdouble) m_prevMousePos.x, (GLdouble) realy, 1.0,
                  mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    double3 centerOld = m_camera.center;
    m_camera.center.x = wx, m_camera.center.y = wy, m_camera.center.z = wz;
    double fovOld = m_camera.fovy;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,this->width(),this->height());
    m_camera.center = centerOld;  m_camera.fovy = fovOld;
    glBindTexture(GL_TEXTURE_2D, 0);*/



    if(m_root) m_root->draw();

    /*this->perspectiveCamera(this->width(), this->height());
    glFlush();
    this->swapBuffers();*/

}


void GLWidget::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.5, +1.5, +1.5, -1.5, 4.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}



void GLWidget::normalizeAngle(int *angle)
{
  while (*angle < 0)
    *angle += 360 * 16;
  while (*angle > 360 * 16)
    *angle -= 360 * 16;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    if(m_rightMouseDown && !m_leftMouseDown) {
       float dx = event->x() - m_prevMousePos.x;
       float dy = event->y() - m_prevMousePos.y;
       double x = m_camera.eye.x, y = m_camera.eye.y, z = m_camera.eye.z;
       double r = m_camera.eye.magnitude();
       double theta = acos(y / r) - dy / 200.0f;
       double phi = atan2(z, x) + dx / 200.0f;
       if (theta > M_PI-.1) theta =M_PI-.1;
       if (theta < .1) theta = .1;
       m_camera.eye.x = r * sin(theta) * cos(phi);
       m_camera.eye.z = r * sin(theta) * sin(phi);
       m_camera.eye.y = r * cos(theta);
       this->perspectiveCamera(this->width(), this->height());
    } else if (m_middleMouseDown) {
        float dy = event->y() - m_prevMousePos.y;
        m_camera.eye.y += dy / 500.f;
        m_camera.center.y += dy / 500.f;
        this->perspectiveCamera(this->width(), this->height());
    } else if (m_leftMouseDown && !m_rightMouseDown && !m_middleMouseDown) {
        GLint viewport[4];
        GLdouble mvmatrix[16], projmatrix[16];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

        /*if(m_texturererer) m_texturererer->paint(event->x(), event->y(), m_meshBrush, m_idImage,
                                                 viewport, projmatrix, mvmatrix);*/
    }

    m_prevMousePos.x = event->x(); m_prevMousePos.y = event->y();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {

    if(event->buttons() & Qt::LeftButton)  {
        GLint viewport[4];
        GLdouble mvmatrix[16], projmatrix[16];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

        /*if(m_texturererer) m_texturererer->paint(event->x(), event->y(), m_meshBrush, m_idImage,
                                               viewport, projmatrix, mvmatrix);*/
        m_leftMouseDown = true;
    }
    if(event->buttons() & Qt::RightButton) m_rightMouseDown = true;
    if(event->buttons() & Qt::MidButton) m_middleMouseDown = true;
}
