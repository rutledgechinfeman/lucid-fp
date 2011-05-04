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
    FeatureProperties(string identity = "", bool isTerminal = false, string geometryType = "", string tileOrStretch = "")
    {
        id = identity;
        terminal = isTerminal;
        geomType = geometryType;
        mesh = NULL;
        texId = -1;
        if (tileOrStretch == "tile")         { tile = true;  }
        else if (tileOrStretch == "stretch") { tile = false; }
        else if (tileOrStretch == "")        { tile = false; }
        else { tile = false; cerr << "ERROR: Symbol definition expecting 'stretch' or 'tile', but received: " << tileOrStretch << endl; }
    }

    string id;
    bool terminal;
    string geomType;
    Mesh* mesh;
    GLuint texId;
    bool tile;
};

class Factory
{
public:
    /// CTOR
    Factory();

    /// DTOR
    virtual ~Factory();

    /// Add listings to the factory
    bool addFeatureType(string symbol, bool isTerminal, string geom = "", string dataPath = "", string tileOrStretch = "");

    /// Get a new instance from the factory (user is responsible for deletion)
    Feature* instanceOf(string symbol, Scope scope = Scope());

private:
    /// Master list of features
    map<string, FeatureProperties> m_featureListing;

    /// Master list of textures to avoid unnecessary loading
    map<string, GLuint> m_texMap;

    /// Master list of meshes to avoid unnecessary loading
    map<string, Mesh*> m_meshMap;
};

#endif // FACTORY_H
