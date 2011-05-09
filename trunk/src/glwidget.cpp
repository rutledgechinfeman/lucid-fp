#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include <iostream>
#include <CS123Algebra.h>
#include "mainwindow.h"
#include <string>
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

    m_planner = NULL;

}

GLWidget::~GLWidget()
{
    delete scenery;
    delete m_timer;
  // Delete anything you have created
}

void GLWidget::setRoot(Feature* root, string s)
{
    m_root = root; // TODO, delete the old one?
    int f = s.find("second");
    if(f < s.length()){
        scenery = new Scenery("night");
    }
    else{
        scenery = new Scenery("day");
    }
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

void GLWidget::parallelCamera(int width, int height) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,static_cast<float>(width),static_cast<float>(height),0.f,-1.f,1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    shader = new QGLShaderProgram();
    shader->addShaderFromSourceFile(QGLShader::Vertex, "../data/blinnphong.vert");
    shader->addShaderFromSourceFile(QGLShader::Fragment, "../data/blinnphong.frag");
    int status = shader->link();
    if (!status) { cerr << "ERROR: Shader didn't link!" << endl; }
}

extern "C" void APIENTRY glActiveTexture(unsigned int);


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (m_planner == NULL) {

        this->perspectiveCamera(this->width(), this->height());

        if(true){

           scenery->draw();
        }

        if( m_root) { m_root->draw(shader); }

    }
    else {

        this->parallelCamera(1, 1);

        m_planner->drawSelf();

    }

}


void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
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
       double theta = dy / 200.0f;
       double phi = dx / 200.0f;

       Vector4 lookVec = Vector4(look.x, look.y, look.z, 1);
       Matrix4x4 rotY;
       rotY = getInvRotYMat(phi);
       Matrix4x4 rotZ;
       Matrix4x4 rotX;
       if(m_camera.center.x > m_camera.eye.x){
            rotZ = getInvRotZMat(theta);
       }
       else{
            rotZ = getRotZMat(theta);
       }
       if(m_camera.center.z > m_camera.eye.z){
            rotX = getRotXMat(theta);
       }
       else{
            rotX = getInvRotXMat(theta);
       }


       lookVec = rotY * lookVec;
       lookVec = rotZ * rotX * lookVec;
       m_camera.center = m_camera.eye - double3(lookVec.x, lookVec.y, lookVec.z);

       this->perspectiveCamera(this->width(), this->height());
    } else if (m_middleMouseDown) {
    } else if (m_leftMouseDown && !m_rightMouseDown && !m_middleMouseDown) {
            grapeVine(event->x() - m_prevMousePos.x, event->y() - m_prevMousePos.y);

    }
    if(m_camera.eye.y < .01){
        m_camera.eye.y = .01;
    }

    m_prevMousePos.x = event->x(); m_prevMousePos.y = event->y();
}

void GLWidget::grapeVine(int dx, int dy){
    double3 l = m_camera.center - m_camera.eye;
    Vector3 look = Vector3(l.x, l.y, l.z);
    look.normalize();
    Vector3 myup = Vector3(m_camera.up.x, m_camera.up.y, m_camera.up.z);
    myup.normalize();

    Vector3 myx = myup.cross(look);
    m_camera.eye.y += myup.y*(dy / 50.f);
    m_camera.center.y += myup.y*(dy / 50.f);
    m_camera.eye.x += myup.x*(dy / 50.f);
    m_camera.center.x += myup.x*(dy / 50.f);
    m_camera.eye.z += myup.z*(dy / 50.f);
    m_camera.center.z += myup.z*(dy / 50.f);
    m_camera.eye.x += myx.x*(dx / 50.f);
    m_camera.center.x += myx.x*(dx / 50.f);
    m_camera.eye.y += myx.y*(dx / 50.f);
    m_camera.center.y += myx.y*(dx / 50.f);
    m_camera.eye.z += myx.z*(dx / 50.f);
    m_camera.center.z += myx.z*(dx / 50.f);
    this->perspectiveCamera(this->width(), this->height());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    scrollMovement(event->delta());


}

void GLWidget::scrollMovement(int dx){
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

void GLWidget::keyPressEvent(QKeyEvent *event) {
    ((MainWindow*)parent())->keyPressEvent(event);
    if(event->key() == Qt::Key_W) scrollMovement(120);
    else if(event->key() == Qt::Key_S) scrollMovement(-120);
    else if(event->key() == Qt::Key_D) grapeVine(-120, 0);
    else if(event->key() == Qt::Key_A) grapeVine(120, 0);

}
