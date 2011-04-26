#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>

using std::string;
using std::map;

struct FeatureProperties
{
    FeatureProperties()
    {
        id = "";
        terminal = false;
        geomType = "";
        dataPath = "";
    }

    FeatureProperties(string identity, bool isTerminal, string geometryType, string pathToDataFile)
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
};

#endif // FACTORY_H
