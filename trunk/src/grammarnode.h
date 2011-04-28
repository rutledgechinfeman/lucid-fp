#ifndef GRAMMARNODE_H
#define GRAMMARNODE_H

#include <string>
#include <vector>

#include "factory.h"
#include "feature.h"

using std::string;
using std::vector;

class GrammarNode
{
public:
    GrammarNode(string line = "");
    virtual ~GrammarNode();

    virtual void evaluate(Feature* feat, Factory &fac) = 0;

protected:
    vector< GrammarNode* > m_children;

};

#endif // GRAMMARNODE_H
