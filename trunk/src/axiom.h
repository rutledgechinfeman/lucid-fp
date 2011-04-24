#ifndef AXIOM_H
#define AXIOM_H

#include "feature.h"
#include <vector>
using std::vector;

struct FeatureWithProbability
{
    FeatureWithProbability( Feature ftr, double prob)
    {
        f = ftr;
        p = prob;
    }

    Feature f;
    double p;
};

class Axiom
{
public:
    Axiom();

    /// Returns a footprint to seed the grammar of a building (ex. start the
    /// contractor off). Optional argument index to get a specific Feature.
    virtual Feature getStartAxiom( int index = -1 ) const;

    virtual void addFeature( Feature f , double probability = 1 );

private:
    vector< FeatureWithProbability > m_footprints;
};

#endif // AXIOM_H
