#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>
#include "mesh.h"
#include "QImage"
#include "scope.h"
#include <QFile>
#include <qgl.h>
#include <QGLFramebufferObject>
#include "massmodel.h"
#include <QGLShaderProgram>

using std::string;
using std::vector;

//Geometry type
#define UNKNOWN -1
#define PLANE 100
#define MESH 101

class Feature
{
public:
    /// CTOR
    Feature();

    /// Convenience CTOR
    Feature(string symbol, string geom, bool isActive = true, Scope scope = Scope(), Feature* parent = NULL);

    /// DTOR
    virtual ~Feature();

    /// Draw self with OpenGL
    virtual void draw(QGLShaderProgram *shader);

    /// Push back a child for this feature (used by grammarnodes)
    void addChild(Feature* f);

    /// Helper method to paint the texture in a tiled fashion
    void drawTiledSelf();

    /// Helper method to stretch the texture across faces
    void drawStretchedSelf();

    /**
     * Setters
     */

    /// Set the mesh pointer
    void setMesh(Mesh* m);

    /// Set up a mass model
    void setMassModel(int numfaces, string type);

    /// Set the texture pointer
    void setTextureID(GLuint i);

    /// Set isActive
    void setActive(bool b);

    /// Set scope
    void setScope(Scope scope);

    /// Set whether to tile the texture
    void setTileTex(bool tileTex);

    /// Set the parent feature of this feature
    void setParent(Feature* parent) { m_parent = parent; }

    /// Set whether this draws as a triangle
    void setTriangle(bool newState) { m_isTriangle = newState; }

    /**
     * Getters
     */

    /// Get isActive
    bool getActive();

    /// Get identifying symbol
    string getSymbol();

    /// Get number of children (to prevent access to actual children vector)
    int getNumChildren();

    /// Get specific child (to prevent access to actual children vector)
    Feature* getChild(int index);

    /// Get current scope copy
    Scope getScope();

    /// Get Scope point
    Vector4 getPoint();

    /// Get Scope scale
    Vector4 getScale();

    /// Get Scope basis
    Matrix4x4 getBasis();

    /// Get the parent of this feature
    Feature* getParent() { return m_parent; }

    /// Get the mass model
    MassModel* getMassModel() { return m_mass; }

private:

    QGLShaderProgram* m_shader;

    /// Convenience parser
    void setType(string geom);

    /// Grammatical symbol representing this feature
    string m_symbol;

    /// Enum, plane or mesh
    int m_geom_type;

    /// Whether this feature has children or is in active use
    bool m_active;

    /// Children of this node, provided it is inactive
    vector<Feature*> m_children;

    /// Mesh media pointer
    Mesh* m_mesh;

    /// Texture media pointer
    GLuint m_textureId;

    /// Parent in feature tree
    Feature* m_parent;

    /// Scope location
    Scope m_scope;

    /// Whether to tile the texture
    bool m_tileTex;

    /// For solid-mass modeling. This can be a roof, or just a block, or whatever. The point is that it knows how to break itself up.
    MassModel* m_mass;

    /// Whether to draw as a rectangle or a triangle
    bool m_isTriangle;
};

#endif // FEATURE_H
