#include "feature.h"
#include <qgl.h>
#include <glwidget.h>
#include <iostream>
#include <QGLFramebufferObject>

using namespace std;


Feature::Feature()
{
    m_active = false;
    m_mesh = NULL;
    m_mass = NULL;
    m_isTriangle = false;
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

    m_scope = scope;

    m_tileTex = false;

    setType(geom);
    m_mass = NULL;
    m_isTriangle = false;
}

Feature::~Feature()
{
    for (vector<Feature*>::iterator i = m_children.begin(); i != m_children.end(); ++ i)
    {
        delete *i;
    }

    delete m_mass;
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

void Feature::setTileTex(bool tileTex)
{
    m_tileTex = tileTex;
}


void Feature::setTextureID(GLuint i)
{
    m_textureId = i;
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
void Feature::addChild(Feature* f)
{
    f->setParent(this);
    m_children.push_back(f);
}

void Feature::draw(QGLShaderProgram* shader)
{

    if(m_children.size()==0){

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        if(m_geom_type == MESH){
            Vector4 scope = this->getScope().getScale();
            this->setScope(this->getScope().setScale(Vector4(scope.x, scope.y, .2, scope.w)));
        }

        Matrix4x4 mat = getTransMat(m_scope.getPoint())* m_scope.getBasis() *getScaleMat(m_scope.getScale());
        REAL* matrix = new REAL[16];
        mat.getTranspose().fillArray(matrix);

        glMultMatrixd(matrix);


        switch (m_geom_type)
        {
            case PLANE:
                glEnable(GL_LIGHTING);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, m_textureId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                if (m_tileTex) { drawTiledSelf();     }
                else           { drawStretchedSelf(); }

                glDisable(GL_LIGHTING);
                glBindTexture(GL_TEXTURE_2D, 0);

                break;

            case MESH:
                if(m_mesh) {
                    shader->bind();
                    m_mesh->drawGL();
                    shader->release();
                }
                break;
        }
        glPopMatrix();

        delete [] matrix;

    }
    else{
        for(unsigned int i=0; i<m_children.size(); i++){
            m_children.at(i)->draw(shader);
        }
    }
    glFlush();
    glDisable(GL_LIGHTING);


}

void Feature::setMassModel(int numfaces, string type)
{
    delete m_mass;
    m_mass = new MassModel(numfaces, type);
}

void Feature::drawTiledSelf()
{
    double x=0;
    double y=0;
    if(abs(this->getScale().x) < .1){
        x = this->getScale().y;
        y = this->getScale().z;
    }
    if(abs(this->getScale().y) < .1){
        x = this->getScale().x;
        y = this->getScale().z;
    }
    if(abs(this->getScale().z) < .1){
        x = this->getScale().x;
        y = this->getScale().y;
    }


    if (m_isTriangle)
    {
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1.0);


        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(x, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glTexCoord2f(.5*x, y);
        glVertex3f(0.5, 1.0, 0.0);


        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
        glTexCoord2f(x, 0.0);
        glVertex3f(1.0, 0.0, 1.0);
        glTexCoord2f(.5*x, y);
        glVertex3f(0.5, 1.0, 1.0);

        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
        glTexCoord2f(x, .5*y);
        glVertex3f(0.0, 1.0, 0.5);
        glTexCoord2f(0.0, y);
        glVertex3f(0.0, 0.0, 0.0);

        glTexCoord2f(0.0, y);
        glVertex3f(1.0, 0.0, 1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glTexCoord2f(x, .5 * y);
        glVertex3f(1.0, 1.0, 0.5);

        glEnd();
        return;
    }

    // Not triangle:
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    //xy
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, y);
    glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(x, y);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    //xz
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(x, y);
    glVertex3f(1.0, 0.0, 1.0);
    glTexCoord2f(0.0, y);
    glVertex3f(0.0, 0.0, 1.0);

    //yz
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, y);
    glVertex3f(0.0, 0.0, 1.0);
    glTexCoord2f(x, y);
    glVertex3f(0.0, 1.0, 1.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    //xy
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(1.0, 0.0, 1.0);
    glTexCoord2f(x, y);
    glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, y);
    glVertex3f(0.0, 1.0, 1.0);

    //xz
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, y);
    glVertex3f(0.0, 1.0, 1.0);
    glTexCoord2f(x, y);
    glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(1.0, 1.0, 0.0);

    //yz
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(x, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(x, y);
    glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, y);
    glVertex3f(1.0, 0.0, 1.0);
    glEnd();
}

void Feature::drawStretchedSelf()
{


    if (m_isTriangle)
    {
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1.0);


        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glTexCoord2f(.5, 1.0);
        glVertex3f(0.5, 1.0, 0.0);


        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(1.0, 0.0, 1.0);
        glTexCoord2f(.5, 1.0);
        glVertex3f(0.5, 1.0, 1.0);

        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
        glTexCoord2f(1.0, .5);
        glVertex3f(0.0, 1.0, 0.5);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);

        glTexCoord2f(0.0, 1.0);
        glVertex3f(1.0, 0.0, 1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glTexCoord2f(1.0, .5);
        glVertex3f(1.0, 1.0, 0.5);

        glEnd();
        return;
    }
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
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
}
