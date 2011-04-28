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
    Feature();
    Feature(string symbol, string geom, bool isActive = true, Scope scope = Scope(), Feature* parent = NULL);

    virtual ~Feature();

    /// Draw self with OpenGL
    virtual void draw();

    void addChild(Feature* f);
    void setMesh(Mesh* m);
    void setTexture(QImage* i);
    void setMedia(void* data);

    void setActive(bool b);
    bool getActive();
    Scope getScope();
    string getSymbol();
    int getNumChildren();
    Feature* getChild(int index);


    Vector4 getPoint();

    Vector4 getScale();

    Matrix4x4 getBasis();


private:

    void setType(string geom);

    /// Unique identifier
    string m_id; // TODO: we probably will not use this

    /// Grammatical symbol representing this feature
    string m_symbol;

    int m_geom_type;

    /// Whether this feature has children or is in active use
    bool m_active;

    /// Children of this node, provided it is inactive
    vector<Feature*> m_children;

    Mesh* m_mesh;
    QImage* m_texture;

    Feature* m_parent;

    Scope m_scope;

};

#endif // FEATURE_H
