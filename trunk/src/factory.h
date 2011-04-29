#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include <QImage>
#include "mesh.h"
#include "feature.h"
#include "scope.h"
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
    /// CTOR
    Factory();

    /// DTOR
    virtual ~Factory();

    /// Add listings to the factory
    bool addFeatureType(string symbol, bool isTerminal, string geom = "", string dataPath = "");

    /// Get a new instance from the factory (user is responsible for deletion)
    Feature* instanceOf(string symbol, Scope scope = Scope());

private:
    /// Master list of features
    map<string, FeatureProperties> m_featureListing;

    /// Master list of textures to avoid unnecessary loading
    map<string, QImage*> m_texMap;

    /// Master list of meshes to avoid unnecessary loading
    map<string, Mesh*> m_meshMap;
};

#endif // FACTORY_H
