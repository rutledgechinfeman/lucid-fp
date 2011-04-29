#ifndef SCOPEOPERATION_H
#define SCOPEOPERATION_H

#include "grammarnode.h"

#define SCALE     100
#define ROTATE    101
#define TRANSLATE 102
#define UNKNOWN   -1

class ScopeOperation
{
public:
    /// CTOR
    ScopeOperation(string arg = "");

    /// DTOR
    virtual ~ScopeOperation();

    /// Operate on the given feature's scope
    void evaluate(Feature* feat, Factory &fac);

    /// For debugging purposes only
    void printSelf();

private:
    /// Kind of operation
    int m_type;

    /// Parameters of the operation (ie, xyz in S(x,y,z))
    Vector4 m_params;
};

#endif // SCOPEOPERATION_H
