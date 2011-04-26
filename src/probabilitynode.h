#ifndef PROBABILITYNODE_H
#define PROBABILITYNODE_H

#include "grammarnode.h"

class ProbabilityNode : public GrammarNode
{
public:
    ProbabilityNode();
    ProbabilityNode(string line);
    virtual ~ProbabilityNode();

private:
    double m_probability;
};

#endif // PROBABILITYNODE_H
