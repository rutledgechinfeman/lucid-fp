#ifndef PROBABILITYNODE_H
#define PROBABILITYNODE_H

#include "grammarnode.h"

struct NodeWithProbability
{
    NodeWithProbability(GrammarNode gn = GrammarNode() , double p = 1.0)
    {
        node = gn;
        prob = p;
    }

    GrammarNode node;
    double      prob;
};

class ProbabilityNode : public GrammarNode
{
public:
    ProbabilityNode(string line = "");
    virtual ~ProbabilityNode();

private:
    vector<double> m_probabilities;
};

#endif // PROBABILITYNODE_H
