#include "contractor.h"

Contractor::Contractor() { }
Contractor::~Contractor() { }

Feature* Contractor::build(Grammar* grammar, Factory* factory)
{
    // Set global class variables for recursion
    m_grammar = grammar;
    m_factory = factory;

    // Look up the universal AXIOM rule
    Feature* root = factory->instanceOf(AXIOM_SYMBOL);
    expandFeature(root);

    return root;
}

void Contractor::expandFeature(Feature* current)
{
    // Base case
    if(current == NULL || !current->getActive()) { return; }

    Rule* r = m_grammar->lookupRule(current);
    if (r) { r->apply(current, *m_factory, current->getScope()); }
    else   { cerr << "WARNING: Skipping evaluation of feature because no rules exist: " << current->getSymbol() << endl; return; }

    for(int i = 0; i < current->getNumChildren(); ++ i)
    {
        expandFeature(current->getChild(i));
    }
}
