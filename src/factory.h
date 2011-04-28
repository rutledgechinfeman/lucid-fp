#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include <QImage>
#include "mesh.h"
#include "feature.h"
using std::string;
using std::map;

struct FeatureProperties
{
    FeatureProperties(string identity = "", bool isTerminal = false, string geometryType = "")
    {
        id = identity;
        terminal = isTerminal;
        geomType = geometryType;
        dataPtr = NULL;
    }

    string id;
    bool terminal;
    string geomType;
    void* dataPtr;
};

class Factory
{
public:
    Factory();
    virtual ~Factory();

    bool addFeatureType(string symbol, bool isTerminal, string geom = "", string dataPath = "");
    Feature* instanceOf(string symbol);

private:
    map<string, FeatureProperties> m_featureListing;
    map<string, QImage*> m_texMap;
    map<string, Mesh*> m_meshMap;
};

#endif // FACTORY_H
