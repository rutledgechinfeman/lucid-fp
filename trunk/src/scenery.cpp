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


Scenery::Scenery()
{

    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    loadtextures();
}


void Scenery::loadtextures(){
    QList<QFile *> fileList;
    fileList.append(new QFile("../data/skybox/brightday1_positive_x.png"));
    fileList.append(new QFile("../data/skybox/brightday1_negative_x.png"));
    fileList.append(new QFile("../data/skybox/brightday1_positive_y.png"));
    fileList.append(new QFile("../data/skybox/brightday1_negative_y.png"));
    fileList.append(new QFile("../data/skybox/brightday1_positive_z.png"));
    fileList.append(new QFile("../data/skybox/brightday1_negative_z.png"));

    textures_["cube_map_1"] = load_cube_map(fileList);
}


/**
  @paragraph Loads the cube map into video memory.

  @param files: a list of files containing the cube map images (should be length
  six) in order.
  @return The assigned OpenGL id to the cube map.
**/
GLuint Scenery::load_cube_map(QList<QFile *> files) {
    /*m_texMap = new GLuint[6];

    for(unsigned int i=0; i<6; i++){
        QImage* img = new QImage(files.at(i)->fileName());
        GLuint temp = 0;

        glGenTextures(1, &temp);

        m_texMap[i]= temp;

        glBindTexture(GL_TEXTURE_2D, temp);
        cout << "here" << endl;
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img->width(), img->height(), GL_BGRA, GL_UNSIGNED_BYTE, img->bits());
        cout << "here1" << endl;
        delete img;
    }
    return m_texMap[0];*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_CUBE_MAP,id);
    for(unsigned i = 0; i < 6; ++i) {
        QImage image,texture;
        image.load(files[i]->fileName());
        image = image.mirrored(false,true);
        texture = QGLWidget::convertToGLFormat(image);
        texture = texture.scaledToWidth(1024,Qt::SmoothTransformation);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,3,3,texture.width(),texture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,texture.bits());
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    return id;

}


void Scenery::draw(){
    glPushMatrix();
    float fExtent = 50.f;
    glTranslatef(0, fExtent-42, 0);

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

    glTexCoord3f(-1.0f,-1.0f,-1.0f);glVertex3f(-fExtent, -fExtent + 42, -fExtent);
    glTexCoord3f(1.0f,-1.0f,-1.0f);glVertex3f(fExtent, -fExtent + 42, -fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f); glVertex3f(fExtent, -fExtent + 42, fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);glVertex3f(-fExtent, -fExtent + 42, fExtent);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);

    glBegin(GL_QUADS);
    glColor3f(.5, .5, .5);

    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-fExtent, -fExtent + 42.2, -15);
    glVertex3f(fExtent, -fExtent + 42.2, -15);
    glVertex3f(fExtent, -fExtent + 42.2, -2);
    glVertex3f(-fExtent, -fExtent + 42.2, -2);

    glColor3f(1.0, 1.0, 0.0);
    float begin = -fExtent;
    float end = begin + 3.0;
    while (begin < fExtent)
    {
        glVertex3f(begin, -fExtent + 42.3, -8.5);
        glVertex3f(end, -fExtent + 42.3, -8.5);
        glVertex3f(end, -fExtent + 42.3, -7.5);
        glVertex3f(begin, -fExtent + 42.3, -7.5);
        begin = end + 2.0;
        end += 5.0;
    }

    glColor3f(1.0, 1.0, 1.0); //TODO: why does everything get gray when we don't use this? someone's not calling this when they should

    glEnd();


    glPopMatrix();

}
