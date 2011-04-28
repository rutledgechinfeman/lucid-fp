#include "contractor.h"

Contractor::Contractor() { }
Contractor::~Contractor() { }

bool Contractor::build(Grammar &grammar, Factory &fac)
{
    // Look up the universal AXIOM rule
    fac.instanceOf(AXIOM_SYMBOL);
    Rule first = grammar.lookupRule(AXIOM_SYMBOL);




    (void) grammar; // TODO
    (void) fac; // TODO

    return true;
}
