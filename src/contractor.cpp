#include "contractor.h"

Contractor::Contractor() { }
Contractor::~Contractor() { }

bool Contractor::build(Grammar &grammar, Factory &factory)
{
    // Set global class variables for recursion
    m_grammar = grammar;
    m_factory = factory;

    // Look up the universal AXIOM rule
    Feature* root = factory.instanceOf(AXIOM_SYMBOL);
    expandFeature(root);

    return true;
}

void Contractor::expandFeature(Feature* current)
{
    if(current == NULL || !current->getActive()) { return; }


}
