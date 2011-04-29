#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>
#include "mesh.h"
#include "QImage"
#include "scope.h"

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

    /**
     * Setters
     */

    /// Set the mesh pointer
    void setMesh(Mesh* m);

    /// Set the texture pointer
    void setTexture(QImage* i);

    /// Set the mesh/texture pointer depending on geometry type
    void setMedia(void* data);

    /// Set isActive
    void setActive(bool b);

    /// Set scope
    void setScope(Scope scope);

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

private:

    /// Convenience parser
    void setType(string geom);

    /// Unique identifier
    string m_id; // TODO: we probably will not use this

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
    QImage* m_texture;

    /// Parent in feature tree
    Feature* m_parent;

    /// Scope location
    Scope m_scope;
};

#endif // FEATURE_H
