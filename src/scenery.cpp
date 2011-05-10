#include "scenery.h"
#include <qgl.h>
#include <QKeyEvent>
#include <QGLContext>
#include <QHash>
#include <QGLShaderProgram>
#include <QQuaternion>
#include <QVector3D>
#include <QString>
#include <GL/glu.h>
#include <iostream>
#include <QFile>
#include <QGLFramebufferObject>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>


using std::cout;
using std::endl;

extern "C"{
    extern void APIENTRY glActiveTexture (GLenum);
}


Scenery::Scenery(string type)
{
    m_type = type;

    fExtent = 500.f;
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    loadtextures();
}


Scenery::~Scenery() { }

void Scenery::loadtextures(){

    // TODO: this needs to be cleaned up (don't need to new all these files, etc)

    QList<QFile *> fileList;
    if(m_type == "night"){
        fileList.append(new QFile("../data/skybox/city/city_right.jpg"));
        fileList.append(new QFile("../data/skybox/city/city_left.jpg"));
        fileList.append(new QFile("../data/skybox/city/city_top.jpg"));
        fileList.append(new QFile("../data/skybox/city/city_bottom.jpg"));
        fileList.append(new QFile("../data/skybox/city/city_front.jpg"));
        fileList.append(new QFile("../data/skybox/city/city_back.jpg"));
    }

    else{
        fileList.append(new QFile("../data/skybox/brightday1_positive_x.png"));
        fileList.append(new QFile("../data/skybox/brightday1_negative_x.png"));
        fileList.append(new QFile("../data/skybox/brightday1_positive_y.png"));
        fileList.append(new QFile("../data/skybox/brightday1_negative_y.png"));
        fileList.append(new QFile("../data/skybox/brightday1_positive_z.png"));
        fileList.append(new QFile("../data/skybox/brightday1_negative_z.png"));
    }

    textures_["cube_map_1"] = load_cube_map(fileList);

    QFile* a;
    foreach(a, fileList)
    {
        delete a;
    }

    GLuint streetTex;
    glGenTextures(1, &streetTex);
    glBindTexture(GL_TEXTURE_2D, streetTex);
    QImage texture;
    texture.load("../data/street.png");
    texture = QGLWidget::convertToGLFormat(texture);

    glTexImage2D(GL_TEXTURE_2D,3,3,texture.width(),texture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,texture.bits());
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    glBindTexture(GL_TEXTURE_2D, streetTex);
    textures_["street_texture"] = streetTex;



}


/**
  @paragraph Loads the cube map into video memory.

  @param files: a list of files containing the cube map images (should be length
  six) in order.
  @return The assigned OpenGL id to the cube map.
**/
GLuint Scenery::load_cube_map(QList<QFile *> files) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_CUBE_MAP,id);
    QImage texture;
    for(unsigned i = 0; i < 6; ++i) {

        texture.load(files[i]->fileName());
        texture = texture.mirrored(false,true);
        texture = QGLWidget::convertToGLFormat(texture);
        texture = texture.scaledToWidth(1024,Qt::SmoothTransformation);

        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    return id;

}


void Scenery::draw(){


    buildSkybox();
    buildStreet();


}

void Scenery::buildSkybox(){
    double offset = 480;
    glPushMatrix();
    glTranslatef(0, fExtent-offset, 0);

    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textures_["cube_map_1"]);
    glBegin(GL_QUADS);


    glTexCoord3f(1.0f,-1.0f,-1.0f); glVertex3f(fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,-1.0f);glVertex3f(-fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f); glVertex3f(fExtent,fExtent,-fExtent);

    glTexCoord3f(1.0f,-1.0f,1.0f);glVertex3f(fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,-1.0f); glVertex3f(fExtent,-fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f);  glVertex3f(fExtent,fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f); glVertex3f(fExtent,fExtent,fExtent);

    glTexCoord3f(-1.0f,-1.0f,1.0f);  glVertex3f(-fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f); glVertex3f(fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f);  glVertex3f(fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f); glVertex3f(-fExtent,fExtent,fExtent);

    glTexCoord3f(-1.0f,-1.0f,-1.0f); glVertex3f(-fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);glVertex3f(-fExtent,-fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f); glVertex3f(-fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);

    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f);glVertex3f(-fExtent,fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f);glVertex3f(fExtent,fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f);glVertex3f(fExtent,fExtent,-fExtent);

    glTexCoord3f(-1.0f,-1.0f,-1.0f);glVertex3f(-fExtent, -fExtent + offset, -fExtent);
    glTexCoord3f(1.0f,-1.0f,-1.0f);glVertex3f(fExtent, -fExtent + offset, -fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f); glVertex3f(fExtent, -fExtent + offset, fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);glVertex3f(-fExtent, -fExtent + offset, fExtent);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

void Scenery::buildStreet(){
    double offset = 480;
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,textures_["street_texture"]);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexCoord2f(0, 0);
    glVertex3f(-fExtent, -fExtent + offset, -15);
    glTexCoord2f(0, fExtent - 421);
    glVertex3f(fExtent, -fExtent + offset, -15);
    glTexCoord2f(1, fExtent - 421);
    glVertex3f(fExtent, -fExtent + offset, -2);
    glTexCoord2f(1, 0);
    glVertex3f(-fExtent, -fExtent + offset, -2);

    glEnd();
    glEnable(GL_DEPTH_TEST);


    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
