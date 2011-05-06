#ifndef SCOPEOPERATION_H
#define SCOPEOPERATION_H

#include "grammarnode.h"
#include "scope.h"
#include <stdlib.h>

// Scope Operation Types
#define SCALE      100
#define ROTATE     101
#define TRANSLATE  102
#define UNKNOWN   -1

// Dimensions
#define NODIM   -1
#define X        0
#define Y        1
#define Z        2

struct OpVal
{
    OpVal (double lowerBound = -1, double upperBound = -1)
    {
        lo = lowerBound;
        if (upperBound < 0) { hi = lowerBound; }
        else                { hi = upperBound; }

        relDim = NODIM;
    }

    OpVal (int dimension)
    {
        lo = -1;
        hi = -1;

        relDim = dimension;
    }

    double getValue(Scope scope)
    {
        // Definite value, or random value
        if (relDim == NODIM)
        {
            double rando = (double)rand();
            rando /= (double) RAND_MAX;

            return rando * (hi - lo) + lo;
        }

        // Relative value
        switch (relDim)
        {
            case X: // Fall through
            case Y: // Fall through
            case Z: // All valid dimensions evaluate here
                return scope.getScale().data[relDim];

            case NODIM:
                cerr << "ERROR: Relative scope op requested, but it doesn't have a dimension to pick." << endl;
                return 0;
        }
    }

    double lo;
    double hi;
    int    relDim; // If this is not NODIM, this is a relative value
};

class ScopeOperation
{
public:
    /// CTOR
    ScopeOperation(string arg = "");

    /// DTOR
    virtual ~ScopeOperation();

    /// Operate on the given feature's scope
    Scope evaluate(Scope &in);

    /// For debugging purposes only
    void printSelf();

private:
    /// Kind of operation
    int m_type;

    /// Data
    OpVal data[3];
};

#endif // SCOPEOPERATION_H
