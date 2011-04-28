#ifndef PROBABILITYNODE_H
#define PROBABILITYNODE_H

#include "grammarnode.h"

class ProbabilityNode : public GrammarNode
{
public:
    ProbabilityNode(string line = "");
    virtual ~ProbabilityNode();
    void evaluate(Feature* feat, Factory &fac);

private:
    vector<double> m_probabilities;
};

#endif // PROBABILITYNODE_H
