#ifndef GRAMMARNODE_H
#define GRAMMARNODE_H

#include <string>

using std::string;

class GrammarNode
{
public:
    GrammarNode(string line = "");
    virtual ~GrammarNode();
};

#endif // GRAMMARNODE_H
