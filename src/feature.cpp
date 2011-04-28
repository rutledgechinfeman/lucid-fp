#include "feature.h"

Feature::Feature()
{
}

Feature::Feature(string symbol, bool isTerminal, bool isActive)
{
    //I think this is for identification. We may add id later
    m_symbol = symbol;

    /// Whether this feature has children or is in active use
    m_active = isActive;

    /// Whether this symbol is a terminal or a nonterminal
    m_terminal = isTerminal;


    //Set mesh and texture to null. SET THESE LATER IF YOU WANT TO USE THE MESH OR TEXTURE
    m_mesh = NULL;
    m_texture = NULL;

}


//This is here so that we don't have to include mesh and image in the constructor.
//Convenient because feature may not have a mesh or a texture
bool Feature::setMesh(Mesh* mesh){
    m_mesh = mesh;
}

bool Feature::setTexture(QImage* image){
    m_texture = image;
}


//Adds a feature child to the list of children
//This is no longer a terminal if it ever was in the past
bool Feature::addChild(Feature f){
    m_children.push_back(f);

    m_terminal = false;

}


Feature::~Feature()
{

}

void Feature::draw()
{

}
