#include "factory.h"

#include <iostream>

using namespace std;

Factory::Factory() { }

Factory::~Factory() { }

bool Factory::addFeatureType(string id, bool isTerminal, string geom, string dataPath)
{
    // If it's already in the map, return false
    if(m_featureListing.find(id) != m_featureListing.end())
    {
        return false;
    }

    // Add a new listing
    m_featureListing[id] = FeatureProperties(id, isTerminal, geom, dataPath);
    return true;
}
