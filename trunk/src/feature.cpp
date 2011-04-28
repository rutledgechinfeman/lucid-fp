#include "feature.h"

Feature::Feature()
{
}

Feature::Feature(string symbol, string geom, bool isActive, Feature* parent)
{
    /// I think this is for identification. We may add id later
    m_symbol = symbol;

    /// Whether this feature has children or is in active use
    m_active = isActive;


    /// If this is the root then parent is NULL
    m_parent = parent;


    //Set mesh and texture to null. SET THESE LATER IF YOU WANT TO USE THE MESH OR TEXTURE
    m_mesh = NULL;
    m_texture = NULL;


    setType(geom);


}


void Feature::setType(string geom){
    if(geom == "plane") m_type = PLANE;
    if(geom == "mesh") m_type = MESH;
}


//This is here so that we don't have to include mesh and image in the constructor.
//Convenient because feature may not have a mesh or a texture
void Feature::setMesh(Mesh* mesh){
    m_mesh = mesh;
}

void Feature::setTexture(QImage* image){
    m_texture = image;
}

void Feature::setActive(bool b){
    m_active = b;
}


//Adds a feature child to the list of children
//This is no longer a terminal if it ever was in the past
void Feature::addChild(Feature* f){
    m_children.push_back(f);

}


Feature::~Feature()
{

}

void Feature::draw()
{

}
