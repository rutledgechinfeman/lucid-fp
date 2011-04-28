#ifndef OPERATION_H
#define OPERATION_H

#include "grammarnode.h"

#define SCALE     100
#define ROTATE    101
#define TRANSLATE 102
#define UNKNOWN   -1

class Operation
{
public:
    /// CTOR
    Operation(string arg = "");

    /// DTOR
    virtual ~Operation();

    /// Operate on the given feature's scope
    void evaluate(Feature* feat, Factory &fac);

    /// For debugging purposes only
    void printSelf();

private:
    /// Kind of operation
    int m_type;

    /// Parameters of the operation (ie, xyz in S(x,y,z))
    double m_params[3];
};

#endif // OPERATION_H
