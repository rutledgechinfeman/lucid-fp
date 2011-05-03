#include "feature.h"
#include <qgl.h>
#include <glwidget.h>
#include <iostream>

using namespace std;


Feature::Feature()
{
    m_active = false;
    m_mesh = NULL;
    m_texture = NULL;
}

Feature::Feature(string symbol, string geom, bool isActive, Scope scope, Feature* parent)
{
    /// I think this is for identification. We may add id later
    m_symbol = symbol;

    /// Whether this feature has children or is in active use
    m_active = isActive;


    /// If this is the root then parent is NULL
    m_parent = parent;


    /// Set mesh and texture to null. SET THESE LATER IF YOU WANT TO USE THE MESH OR TEXTURE
    m_mesh = NULL;
    m_texture = NULL;

    m_scope = scope;

    setType(geom);


}

Scope Feature::getScope(){
    return m_scope;
}

Vector4 Feature::getPoint(){
    return m_scope.getPoint();
}

Vector4 Feature::getScale(){
    return m_scope.getScale();
}

Matrix4x4 Feature::getBasis(){
    return m_scope.getBasis();
}

void Feature::setType(string geom)
{
    if(geom == "plane") { m_geom_type = PLANE; }
    else if(geom == "mesh") { m_geom_type = MESH; }
    else { m_geom_type = UNKNOWN; }
}

void Feature::setScope(Scope scope)
{
    m_scope = scope;
}

//This is here so that we don't have to include mesh and image in the constructor.
//Convenient because feature may not have a mesh or a texture
void Feature::setMesh(Mesh* mesh)
{

    m_mesh = mesh;

}

void Feature::setTexture(QImage* image)
{
    m_texture = image;
}

void Feature::setMedia(void* data)
{
    switch (m_geom_type)
    {
        case PLANE:
            m_image = (QImage*) data;
            m_id = loadTexture(m_image);
            break;

        case MESH:
            m_mesh = (Mesh*) data;
            break;
    }
}

void Feature::setActive(bool b)
{
    m_active = b;
}

bool Feature::getActive()
{
    return m_active;
}

string Feature::getSymbol()
{
    return m_symbol;
}

int Feature::getNumChildren()
{
    return m_children.size();
}

Feature* Feature::getChild(int index)
{
    return m_children.at(index);
}

//Adds a feature child to the list of children
//This is no longer a terminal if it ever was in the past
void Feature::addChild(Feature* f){
    m_children.push_back(f);

}


Feature::~Feature()
{

}

extern "C"{
    extern void APIENTRY glActiveTexture (GLenum);
}
GLuint Feature::loadTexture(QImage* img) {

    QImage texture;
    GLuint toReturn = -1;
    texture = QGLWidget::convertToGLFormat(*img);

    glGenTextures(1, &toReturn);
    m_texture = new QImage(texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_texture->width(), m_texture->height(), GL_RGBA, GL_UNSIGNED_BYTE, m_texture->bits());

    return toReturn;
}


void Feature::draw()
{
    if(m_children.size()==0){

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        Matrix4x4 mat = getTransMat(m_scope.getPoint()) * m_scope.getBasis() * getScaleMat(m_scope.getScale());
        REAL* matrix = new REAL[16];
        mat.getTranspose().fillArray(matrix);
        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH); //Enable smoothing

        glEnable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0+m_id );
        glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0+m_id);


        glMultMatrixd(matrix);
        switch (m_geom_type)
        {
            case PLANE:
                glBegin(GL_QUADS);
                //xy
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0, 0.0, 0.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, 0.0, 0.0);

                //xz

                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0, 0.0, 0.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, 0.0, 0.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 0.0, 1.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.0, 0.0, 1.0);

                //yz

                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0, 0.0, 0.0);

                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.0, 0.0, 1.0);

                glTexCoord2f(1.0, 1.0);
                glVertex3f(0.0, 1.0, 1.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0, 1.0, 0.0);

                //xy

                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0, 0.0, 1.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, 0.0, 1.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 1.0, 1.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.0, 1.0, 1.0);

                //xz
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0, 1.0, 0.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.0, 1.0, 1.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 1.0, 1.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, 1.0, 0.0);

                //yz

                glTexCoord2f(0.0, 0.0);
                glVertex3f(1.0, 0.0, 0.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 1.0, 1.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(1.0, 0.0, 1.0);



                glEnd();
                break;

            case MESH:
                if(m_mesh) { m_mesh->drawGL(); }
                break;
        }
        glPopMatrix();

    }
    else{
        for(unsigned int i=0; i<m_children.size(); i++){
            m_children.at(i)->draw();
        }
    }
    glFlush();

}
