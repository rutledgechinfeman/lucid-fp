#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
#include <CS123Algebra.h>

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
    m_root = NULL;
    this->setDefaultCamera();
}

GLWidget::~GLWidget()
{
  // Delete anything you have created
}

void GLWidget::setRoot(Feature* root)
{
    m_root = root; // TODO, delete the old one?

    scenery = new Scenery();
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
    this->setDefaultCamera();
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_DIFFUSE ) ;
    glEnable(GL_COLOR_MATERIAL);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    m_timer->start(1000.0f / 30.0f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->perspectiveCamera(this->width(), this->height());
    //TODO: make some scenery variable

    if(true){

       scenery->draw();
    }

    if( m_root) { m_root->draw(); }

}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
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
       double3 look = m_camera.eye - m_camera.center;
       //look.normalize();
       double theta = dy / 200.0f;
       //double phi = atan2(look.z, look.x) + dx / 200.0f;
       double phi = dx / 200.0f;
       //cout << phi << endl;
       /*
       m_camera.center.x = r * sin(theta) * cos(phi);
       m_camera.center.z = r * sin(theta) * sin(phi);
       m_camera.center.y = r * cos(theta);*/
       Vector4 lookVec = Vector4(look.x, look.y, look.z, 1);
       Matrix4x4 rotY;
       rotY = getInvRotYMat(phi);
       Matrix4x4 rotX;
       if(m_camera.center.x > m_camera.eye.x){
            rotX = getInvRotZMat(theta);
       }
       else{
           rotX = getRotZMat(theta);
       }
       lookVec = rotY * lookVec;
       lookVec = rotX * lookVec;
       m_camera.center = m_camera.eye - double3(lookVec.x, lookVec.y, lookVec.z);

       this->perspectiveCamera(this->width(), this->height());
    } else if (m_middleMouseDown) {
    } else if (m_leftMouseDown && !m_rightMouseDown && !m_middleMouseDown) {
        double3 l = m_camera.center - m_camera.eye;
        Vector3 look = Vector3(l.x, l.y, l.z);
        look.normalize();
        Vector3 myup = Vector3(m_camera.up.x, m_camera.up.y, m_camera.up.z);
        myup.normalize();

        Vector3 myx = myup.cross(look);
        float dy = event->y() - m_prevMousePos.y;
        m_camera.eye.y += myup.y*(dy / 50.f);
        m_camera.center.y += myup.y*(dy / 50.f);
        m_camera.eye.x += myup.x*(dy / 50.f);
        m_camera.center.x += myup.x*(dy / 50.f);
        m_camera.eye.z += myup.z*(dy / 50.f);
        m_camera.center.z += myup.z*(dy / 50.f);
        float dx = event->x() - m_prevMousePos.x;
        m_camera.eye.x += myx.x*(dx / 50.f);
        m_camera.center.x += myx.x*(dx / 50.f);
        m_camera.eye.y += myx.y*(dx / 50.f);
        m_camera.center.y += myx.y*(dx / 50.f);
        m_camera.eye.z += myx.z*(dx / 50.f);
        m_camera.center.z += myx.z*(dx / 50.f);
        this->perspectiveCamera(this->width(), this->height());
    }
    if(m_camera.eye.y < .01){
        m_camera.eye.y = .01;
    }

    m_prevMousePos.x = event->x(); m_prevMousePos.y = event->y();
}

void GLWidget::wheelEvent(QWheelEvent *event) {

    int dx = event->delta();
    double3 look = (m_camera.center - m_camera.eye).getNormalized();
    m_camera.eye += look * 0.01 * dx;
    m_camera.center += look * 0.01 * dx;
    this->perspectiveCamera(this->width(), this->height());


}

void GLWidget::setDefaultCamera() {
    m_camera.center.x = 0.0,m_camera.center.y = 0.0,m_camera.center.z = 0.0;
    m_camera.eye.x = -14.61,m_camera.eye.y = 12.81,m_camera.eye.z = -10.74;
    m_camera.up.x = 0.0,m_camera.up.y = 1.0,m_camera.up.z = 0.0;
    m_camera.near = 0.001f,m_camera.far = 200.0;
    m_camera.fovy = 60.0;
    this->perspectiveCamera(this->width(), this->height());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(!(event->buttons() & Qt::LeftButton)) m_leftMouseDown = false;
    if(!(event->buttons() & Qt::RightButton)) m_rightMouseDown = false;
    if(!(event->buttons() & Qt::MidButton)) m_middleMouseDown = false;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton)  {
        GLint viewport[4];
        GLdouble mvmatrix[16], projmatrix[16];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
        m_leftMouseDown = true;
    }
    if(event->buttons() & Qt::RightButton) m_rightMouseDown = true;
    if(event->buttons() & Qt::MidButton) m_middleMouseDown = true;
}
