#ifndef OPERATION_H
#define OPERATION_H

#include "grammarnode.h"

class Operation
{
public:
    Operation(string arg = "");
    virtual ~Operation();

    void evaluate(Feature* feat, Factory &fac);
    void printSelf();
};

#endif // OPERATION_H
