#include "factory.h"

#include <iostream>

using namespace std;

Factory::Factory()
{
}

Factory::~Factory()
{
    for(map<string, Mesh*>::iterator ii = m_meshMap.begin(); ii != m_meshMap.end(); ++ ii)
    {
        delete ii->second;
    }
}

bool Factory::addFeatureType(string id, bool isTerminal, string geom, string dataPath, string tileOrStretch)
{
    // If it's already in the map, return false
    if(m_featureListing.find(id) != m_featureListing.end()) { return false; }

    // Add a new listing
    m_featureListing[id] = FeatureProperties(id, isTerminal, geom, tileOrStretch);

    // Look for a datapath on terminal symbols only
    if(!isTerminal   ) { return true; }
    if(geom     == "") { cerr << "ERROR: terminal symbol used without a specified geometry" << endl; return false; }
    if(dataPath == "") { cerr << "ERROR: terminal symbol used without a specified datapath" << endl; return false; }

    dataPath = "../data/" + dataPath;

    // Planes have textures (2D)
    if(geom == "plane")
    {
        if (m_texMap.find(dataPath) == m_texMap.end())
        {
            QImage* img = new QImage(QImage(QString(dataPath.c_str())).mirrored());
            GLuint temp = 0;
            m_texMap[dataPath] = temp;
            glGenTextures(1, &m_texMap[dataPath]);

            glBindTexture(GL_TEXTURE_2D, m_texMap[dataPath]);
            gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img->width(), img->height(), GL_BGRA, GL_UNSIGNED_BYTE, img->bits());
            delete img;
        }
        m_featureListing[id].texId = m_texMap[dataPath];
    }

    // Meshes have... meshes. (3D)
    else if(geom == "mesh")
    {
        if (m_meshMap.find(dataPath) == m_meshMap.end()) { m_meshMap[dataPath] = new Mesh(dataPath); }
        m_featureListing[id].mesh = m_meshMap[dataPath];
    }

    else { cerr << "ERROR: Unknown geometry type: " << geom << endl; }

    return true;
}

Feature* Factory::instanceOf(string symbol, Scope scope)
{
    Feature* toReturn;
    if(m_featureListing.find(symbol) == m_featureListing.end())
    {
        cerr << "WARNING: Requested feature of type [" << symbol << "], which has not be created yet. Returning an empty Feature." << endl;
        toReturn = new Feature();
    }
    else
    {
        FeatureProperties& f = m_featureListing[symbol];
        toReturn = new Feature(f.id, f.geomType, !f.terminal);
        toReturn->setTileTex(f.tile);
        if (m_featureListing[symbol].terminal)
        {
            if (m_featureListing[symbol].geomType == "plane")
            {
                toReturn->setTextureID(m_featureListing[symbol].texId);
            }
            else
            {
                toReturn->setMesh(m_featureListing[symbol].mesh);
            }
        }
    }

    toReturn->setScope(scope);

    return toReturn;
}
