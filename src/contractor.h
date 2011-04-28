#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "grammar.h"
#include "factory.h"

#define AXIOM_SYMBOL "A"

class Contractor
{
public:
    Contractor();
    virtual ~Contractor();

    bool build(Grammar &grammar, Factory &factory);

private:
    void expandFeature(Feature* current);
    Grammar m_grammar;
    Factory m_factory;
};

#endif // CONTRACTOR_H
