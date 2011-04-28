#ifndef PROBABILITYNODE_H
#define PROBABILITYNODE_H

#include "grammarnode.h"


class ProbabilityNode : public GrammarNode
{
public:
    /// CTOR
    ProbabilityNode(string line = "");

    /// DTOR
    virtual ~ProbabilityNode();

    /// Apply to feature
    void evaluate(Feature* feat, Factory &fac);

    /// For debugging purposes
    virtual void printSelf();

private:
    /// Match probabilities to each child node, so we can pick children with certain probability
    vector<double> m_probabilities;
};

#endif // PROBABILITYNODE_H
