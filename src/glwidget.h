// Only include once
#ifndef GLWIDGET_H
#define GLWIDGET_H
// base class for the GLWidget
#include <QGLWidget>
#include "common.h"
#include "feature.h"
#include "scenery.h"
#include <QGLShaderProgram>
#include "floorplanner.h"
class QTimer;


class GLWidget : public QGLWidget
{
   // Magic stuff here
   Q_OBJECT
public:
   // Constructor and destructor
   GLWidget(QWidget *parent = 0);
   ~GLWidget();

   void setRoot(Feature* root, string s);
   REAL m_r;
   REAL m_phi;
   REAL m_theta;
   //Basic setup
   QSize minimumSizeHint() const;
   QSize sizeHint() const;

   QTimer *m_timer;

   struct double2 {
       double x,y;
   } m_prevMousePos;

   void wheelEvent(QWheelEvent *event);

   /// Loads a perspective projection into OpenGL.
   void perspectiveCamera(int width, int height);
   struct Camera {
       double fovy, near, far;
       double3 eye, center, up;
   } m_camera;
   Feature* m_root;

   void setDefaultCamera();
   void mouseReleaseEvent(QMouseEvent *event);
   Scenery* scenery;
   QGLShaderProgram* shader;

   FloorPlanner* m_planner;

   void setPlanner(FloorPlanner* planner) { m_planner = planner; }

   void parallelCamera(int width, int height);

// Not really C++ but these are the handler functions
// for the events

// And these are the messages that connect to the previous
// handlers
signals:
   void xRotationChanged(int angle);
   void yRotationChanged(int angle);
   void zRotationChanged(int angle);
   protected:
   void initializeGL();
   void paintGL();
   void resizeGL(int width, int height);
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void normalizeAngle(int *angle);
   void scrollMovement(int d);
   GLuint object;
   void keyPressEvent(QKeyEvent *event);
   void grapeVine(int x, int y);
   bool m_rightMouseDown, m_leftMouseDown, m_middleMouseDown;

   QPoint lastPos;
};
#endif
