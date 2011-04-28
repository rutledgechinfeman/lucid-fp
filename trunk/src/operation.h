#ifndef OPERATION_H
#define OPERATION_H

#include "grammarnode.h"

class Operation : public GrammarNode
{
public:
    Operation(string arg = "");
    virtual ~Operation();

    void evaluate(Feature* feat, Factory &fac);
};

#endif // OPERATION_H
