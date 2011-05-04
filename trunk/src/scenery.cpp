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
    //_scenery = new GLuint[6];
    //setIds();
}

void Scenery::setIds(){
   // GLuint temp = 0;
    //m_texMap[dataPath] = temp;
    //glGenTextures(6, _scenery);
}

void Scenery::loadtextures(){
    QList<QFile *> fileList;
    fileList.append(new QFile("textures/astra/posx.jpg"));
    fileList.append(new QFile("textures/astra/negx.jpg"));
    fileList.append(new QFile("textures/astra/posy.jpg"));
    fileList.append(new QFile("textures/astra/negy.jpg"));
    fileList.append(new QFile("textures/astra/posz.jpg"));
    fileList.append(new QFile("textures/astra/negz.jpg"));
    textures_["cube_map_1"] = load_cube_map(fileList);
}


/**
  @paragraph Loads the cube map into video memory.

  @param files: a list of files containing the cube map images (should be length
  six) in order.
  @return The assigned OpenGL id to the cube map.
**/
GLuint Scenery::load_cube_map(QList<QFile *> files) {
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
        cout << "\t \033[32m" << files[i]->fileName().toStdString() << "\033[0m" << endl;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    return id;
}


void Scenery::draw(float x, float y, float z){
/*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glPushMatrix();

    glLoadIdentity();

    gluLookAt(0,0,0, x, y, z, 0, 1, 0);

    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    glColor4f(1,1,1,1);

    glBindTexture(GL_TEXTURE_2D, _skybox[0]);


    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();

    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, _skybox[1]);

   glBegin(GL_QUADS);

       glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );

      glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );

     glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );

       glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );

   glEnd();



   // Render the back quad

   glBindTexture(GL_TEXTURE_2D, _skybox[2]);

   glBegin(GL_QUADS);

       glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );

       glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );

       glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );

       glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );



   glEnd();



   // Render the right quad

   glBindTexture(GL_TEXTURE_2D, _skybox[3]);

   glBegin(GL_QUADS);

       glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );

       glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );

       glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );

       glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );

   glEnd();



   // Render the top quad

   glBindTexture(GL_TEXTURE_2D, _skybox[4]);

   glBegin(GL_QUADS);

       glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );

       glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );

       glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );

       glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );

   glEnd();



   // Render the bottom quad

   glBindTexture(GL_TEXTURE_2D, _skybox[5]);

   glBegin(GL_QUADS);

       glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );

       glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );

       glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );

       glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );

   glEnd();



   // Restore enable bits and matrix

   glPopAttrib();

    glPopMatrix();
*/
}
