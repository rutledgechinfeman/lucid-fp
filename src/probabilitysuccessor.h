#ifndef PROBABILITYSUCCESSOR_H
#define PROBABILITYSUCCESSOR_H

#include "grammarnode.h"

class ProbabilitySuccessor : public GrammarNode
{
public:
    ProbabilitySuccessor();
    virtual ~ProbabilitySuccessor();

private:
    double m_probability;
};

#endif // PROBABILITYSUCCESSOR_H
