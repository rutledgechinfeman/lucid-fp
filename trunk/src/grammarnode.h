#ifndef GRAMMARNODE_H
#define GRAMMARNODE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class GrammarNode
{
public:
    GrammarNode(string line = "");
    virtual ~GrammarNode();

protected:
    vector< GrammarNode* > m_children;

};

#endif // GRAMMARNODE_H
