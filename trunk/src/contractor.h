#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "grammar.h"
#include "factory.h"

#define AXIOM_SYMBOL "A"

class Contractor
{
public:
    /// CTOR
    Contractor();

    /// DTOR
    virtual ~Contractor();

    /// Main loop; construct a feature tree from a grammar using the factory
    Feature* build(Grammar* grammar, Factory* factory);

private:
    /// Recursive helper method to build out the tree
    void expandFeature(Feature* current);

    /// Class global convenience var (for recursion)
    Grammar* m_grammar;

    /// Class global convenience var (for recursion)
    Factory* m_factory;
};

#endif // CONTRACTOR_H
