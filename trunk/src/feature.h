#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>
#include "mesh.h"
#include "QImage"

using std::string;
using std::vector;

class Feature
{
public:
    Feature();
    Feature(string symbol, bool isTerminal, bool isActive);

    virtual ~Feature();

    /// Draw self with OpenGL
    virtual void draw();
    bool addChild(Feature f);
    bool setMesh(Mesh* m);
    bool setTexture(QImage* i);

private:
    /// Unique identifier
    string m_id;

    /// Grammatical symbol representing this feature
    string m_symbol;

    /// Whether this feature has children or is in active use
    bool   m_active;

    /// Whether this symbol is a terminal or a nonterminal
    bool   m_terminal;

    /// Children of this node, provided it is inactive
    vector<Feature> m_children;

    Mesh* m_mesh;
    QImage* m_texture;

};

#endif // FEATURE_H
