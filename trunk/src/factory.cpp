#include "factory.h"

#include <iostream>

using namespace std;

Factory::Factory() {
    m_texMap = map<string, QImage*>();
    m_meshMap = map<string, Mesh*>();
}

Factory::~Factory() {
//TODO: delete stuff
}

bool Factory::addFeatureType(string id, bool isTerminal, string geom, string dataPath)
{
    // If it's already in the map, return false
    if(m_featureListing.find(id) != m_featureListing.end())
    {
        return false;
    }

    // Add a new listing
    m_featureListing[id] = FeatureProperties(id, isTerminal, geom, dataPath);


    //if it is not a terminal character then it doesn't have a texture or object attached
    if(isTerminal){

        //if it is a plane then it has a texture
        if(geom == "plane"){
            if(dataPath != ""){

                if(m_texMap.find(dataPath) == m_texMap.end()){
                    m_texMap[dataPath] = new QImage(QString(dataPath.c_str()));
                }

            }
        }

        //if it is a mesh then it has a .obj file
        else if(geom == "mesh"){
            if(dataPath != ""){

                if(m_meshMap.find(dataPath) == m_meshMap.end()){
                    m_meshMap[dataPath] = new Mesh(dataPath);
                }

            }

        }

        //add other stuff later
        else{}
    }



    return true;
}
