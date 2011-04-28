#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>
#include "mesh.h"
#include "QImage"

using std::string;
using std::vector;
#define UNKNOWN -1

//Condition type
#define PLANE 100;
#define MESH 101;


class Feature
{
public:
    Feature();
    Feature(string symbol, string geom, bool isActive = true, Feature* parent = NULL);

    virtual ~Feature();

    /// Draw self with OpenGL
    virtual void draw();
    void addChild(Feature* f);
    void setMesh(Mesh* m);
    void setTexture(QImage* i);
    void setActive(bool b);

    void setType(string geom);

private:
    /// Unique identifier
    string m_id;

    /// Grammatical symbol representing this feature
    string m_symbol;

    int m_type;

    /// Whether this feature has children or is in active use
    bool m_active;

    /// Children of this node, provided it is inactive
    vector<Feature*> m_children;

    Mesh* m_mesh;
    QImage* m_texture;

    Feature* m_parent;

};

#endif // FEATURE_H
