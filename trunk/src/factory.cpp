#include "factory.h"

#include <iostream>

using namespace std;

Factory::Factory()
{
}

Factory::~Factory()
{
    for(map<string, QImage*>::iterator ii = m_texMap.begin(); ii != m_texMap.end(); ++ ii)
    {
        delete ii->second;
    }

    for(map<string, Mesh*>::iterator ii = m_meshMap.begin(); ii != m_meshMap.end(); ++ ii)
    {
        delete ii->second;
    }
}

bool Factory::addFeatureType(string id, bool isTerminal, string geom, string dataPath)
{
    // If it's already in the map, return false
    if(m_featureListing.find(id) != m_featureListing.end()) { return false; }

    // Add a new listing
    m_featureListing[id] = FeatureProperties(id, isTerminal, geom, dataPath);

    // Look for a datapath on terminal symbols only
    if(!isTerminal   ) { return true; }
    if(geom     == "") { cerr << "ERROR: terminal symbol used without a specified geometry" << endl; return false; }
    if(dataPath == "") { cerr << "ERROR: terminal symbol used without a specified datapath" << endl; return false; }

    // Planes have textures (2D)
    if(geom == "plane" && m_texMap.find(dataPath) == m_texMap.end())
    {
        m_texMap[dataPath] = new QImage(QString(dataPath.c_str()));
    }

    // Meshes have... meshes. (3D)
    else if(geom == "mesh" && m_meshMap.find(dataPath) == m_meshMap.end())
    {
        m_meshMap[dataPath] = new Mesh(dataPath);
    }

    else { cerr << "ERROR: Unknown geometry type: " << geom << endl; }

    return true;
}

Feature Factory::instanceOf(string symbol)
{
    Feature toReturn;
    if(m_featureListing.find(symbol) == m_featureListing.end())
    {
        toReturn = Feature();
    }
    else
    {
        FeatureProperties& f = m_featureListing[symbol];
        toReturn = Feature(f.id, f.terminal, true);

        //toReturn.setMedia
    }

    return toReturn;
}
