#ifndef OPERATION_H
#define OPERATION_H

#include "grammarnode.h"

class Operation : public GrammarNode
{
public:
    Operation(string arg = "");
    virtual ~Operation();
};

#endif // OPERATION_H
