#ifndef PROBABILITYSUCCESSOR_H
#define PROBABILITYSUCCESSOR_H

#include "grammarnode.h"

class ProbabilityNode : public GrammarNode
{
public:
    ProbabilityNode();
    virtual ~ProbabilityNode();

private:
    double m_probability;
};

#endif // PROBABILITYSUCCESSOR_H
