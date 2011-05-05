#ifndef SCENERY_H
#define SCENERY_H
#include <QHash>
#include <QString>
#include <qgl.h>
#include "common.h"

#include <string>
#include <map>
#include <QImage>
#include "mesh.h"
#include "feature.h"
#include "scope.h"
using std::string;
using std::map;

class Scenery
{
public:
    Scenery();
    virtual ~Scenery();
    void draw();
    void setIds();
    QHash<QString, GLuint>                      textures_; ///hashmap of all textures
    GLuint load_cube_map(QList<QFile *> files);
    void loadtextures();
    GLuint* m_texMap;

};

#endif // SCENERY_H
