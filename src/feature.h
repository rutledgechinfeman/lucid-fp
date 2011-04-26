#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Feature
{
public:
    Feature();
    virtual ~Feature();

    /// Draw self with OpenGL
    virtual void draw();

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
    vector<Feature> children;
};

#endif // FEATURE_H
