#ifndef GRAMMARNODE_H
#define GRAMMARNODE_H

#include "factory.h"
#include "feature.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class GrammarNode
{
public:
    /// CTOR
    GrammarNode(string line = "");

    /// DTOR
    virtual ~GrammarNode();

    /// Pure virtual method to edit a parent feature and give it children
    virtual void evaluate(Feature* feat, Factory &fac, Scope scope) = 0;

    /// For debugging purposes only
    virtual void printSelf() = 0;

protected:
    /// The rest of this subtree
    vector< GrammarNode* > m_children;
};

#endif // GRAMMARNODE_H
