#ifndef SCENERY_H
#define SCENERY_H
#include <QHash>
#include <QString>
#include <qgl.h>
#include "common.h"

class QGLContext;
class QGLShaderProgram;
class QFile;
class QGLFramebufferObject;
class QKeyEvent;

class Scenery
{
public:
    Scenery();
    void draw(float x, float y, float z);
    void setIds();
    QHash<QString, GLuint>                      textures_; ///hashmap of all textures
    GLuint load_cube_map(QList<QFile *> files);
    void loadtextures();

};

#endif // SCENERY_H
