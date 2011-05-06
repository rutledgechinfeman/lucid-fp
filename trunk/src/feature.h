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
    virtual void draw();

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


    GLuint loadTexture();

    /// Set the texture pointer
    void setTextureID(GLuint i);

    /// Set isActive
    void setActive(bool b);

    /// Set scope
    void setScope(Scope scope);

    /// Set whether to tile the texture
    void setTileTex(bool tileTex);

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

    /// set the parent feature of this feature
    void setParent(Feature* parent) { m_parent = parent; }
    Feature* getParent() { return m_parent; }

private:

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
};

#endif // FEATURE_H
