#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "grammar.h"
#include "factory.h"

class Contractor
{
public:
    Contractor();
    virtual ~Contractor();

    bool build(Grammar &grammar, Factory &fac);
};

#endif // CONTRACTOR_H
