#include "axiom.h"
#include <stdlib.h>

Axiom::Axiom()
{
    // TODO: We need to think of a way to populate axioms
}

Feature Axiom::getStartAxiom(int index) const
{
    if( index >= 0 )
        return m_footprints[ index ].f;

    double sample = (double) rand() / (double) RAND_MAX;
    double total = 0;
    for( vector<FeatureWithProbability>::const_iterator ii = m_footprints.begin() ; ii != m_footprints.end() ; ++ ii )
    {
        total += ii->p;
        if( total >= sample )
        {
            return ii->f;
        }
    }

    return Feature();
}

void Axiom::addFeature(Feature f, double probability)
{
    m_footprints.push_back(FeatureWithProbability(f, probability));
}
