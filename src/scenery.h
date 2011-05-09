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
    Scenery(string type);
    virtual ~Scenery();
    void draw();
    void setIds();
    GLuint load_cube_map(QList<QFile *> files);
    void loadtextures();
    GLuint* m_texMap;
    void buildSkybox();
    void buildStreet();
    float fExtent;
    string m_type;

private:

    QHash<QString, GLuint>                      textures_; ///hashmap of all textures

};

#endif // SCENERY_H
