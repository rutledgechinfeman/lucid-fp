#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include <QImage>
#include "mesh.h"
using std::string;
using std::map;

struct FeatureProperties
{
    FeatureProperties(string identity = "", bool isTerminal = false, string geometryType = "", string pathToDataFile = "")
    {
        id = identity;
        terminal = isTerminal;
        geomType = geometryType;
        dataPath = pathToDataFile;
    }

    string id;
    bool terminal;
    string geomType;
    string dataPath;
};

class Factory
{
public:
    Factory();
    virtual ~Factory();

    bool addFeatureType(string id, bool isTerminal, string geom = "", string dataPath = "");

private:
    map<string, FeatureProperties> m_featureListing;
    map<string, QImage*> m_texMap;
    map<string, Mesh*> m_meshMap;
};

#endif // FACTORY_H
