#include "mesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <qgl.h>
#include <stringutil.h>
using std::vector;
using namespace std;


Mesh::Mesh(string filename)
{
    ifstream myfile(filename.c_str());
    if(!myfile.is_open() || !myfile.good())
    {
        cerr << "ERROR: Could not open mesh file: " << filename << endl;
    }



    // Iterate over each line in the file
    string line;
    vector<string> tokens;
    int currVertex = 0;
    int currFace = 0;
    while (myfile.good())
    {
        getline(myfile, line);


        // Tokenize line
        tokens.clear();
        StringUtil::split(line, " ", tokens);
        if(tokens.size() == 0) continue;

        // Skip over commented lines
        if(tokens.at(0) == "#") { continue; }

        // Parse a vertex line
        if(tokens.at(0) == "v")
        {
            if(tokens.size() != 4) { cerr << "WARNING: Skipping malformed vertex line in mesh file: " << line << endl; }

            // Make a new vertex for this line
            MeshVertex* vert = new MeshVertex();
            vert->idx = currVertex; // set ID
            vert->p = double3(strtod(tokens.at(1).c_str(), NULL), strtod(tokens.at(2).c_str(), NULL), strtod(tokens.at(3).c_str(), NULL)); // set coordinates
            vertices.push_back(vert); // add to the list
            currVertex++;
        }

        // Parse a face line
        else if(tokens.at(0) == "f")
        {
            if(tokens.size() != 4) { cerr << "WARNING: Skipping malformed vertex line in mesh file: " << line << endl; }

            // Parse the vertices, of the form "vertex/normal/texture"
            vector<string> face;
            for(unsigned int i=1; i<tokens.size(); i++)
            {
                face.clear();
                StringUtil::split(tokens.at(i), "/", face, true);
                //normal of the ith vertex on this face:
                if(atoi(face.at(2).c_str())-1 >= (int)normals.size()){
                    cout << atoi(face.at(2).c_str())-1 << endl;
                    cout << normals.size() << endl;
                }
                Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), strtod(face.at(1).c_str(), NULL), strtod(face.at(2).c_str(), NULL), 0);
                triangles.push_back(v); // Currently, we're only supporting the vertex numbers, not normals or texture
                // TODO: if we ever implement this feature

            }
            currFace++;
        }
        // Parse a vertex normal line

        else if(tokens.at(0) == "vn") {
            if(tokens.size() != 4) {
                cerr << "WARNING: Skipping malformed normal line in mesh file: " << line << endl;
                continue;
            }
            double3 d = double3(strtod(tokens.at(1).c_str(), NULL), strtod(tokens.at(2).c_str(), NULL), strtod(tokens.at(3).c_str(), NULL));
            normals.push_back(d); // set coordinates
        }
        // Parse a vertex texture line
        else if(tokens.at(0) == "vt") {
            if(tokens.size() != 3){
                cerr << "WARNING: Skipping malformed texture line in mesh file: " << line << endl;
                continue;
            }
            double2 d = double2(strtod(tokens.at(1).c_str(), NULL), strtod(tokens.at(2).c_str(), NULL));
            texcoords.push_back(d); // set coordinates
        }


    }
    // Wrap up
    myfile.close();

}


Mesh::~Mesh()
{
}

void Mesh::drawGL()
{

    glFrontFace(GL_CCW);
    glEnable(GL_LIGHTING);
    //cout << "drawing" << endl;
    glBegin(GL_TRIANGLES);
    for(unsigned int i=0; i < triangles.size(); i++)
    {
        //for each element on a face we index into normals, vertices, and maybe texture coords
        if(normals.size() > 0){
            double3 thisNorm = normals.at(triangles.at(i).z - 1 );
            glNormal3f(thisNorm.x, thisNorm.y, thisNorm.z);
        }
/*
        if(texcoords.size() > 0){
            double2 thisTexCoord = texcoords.at(triangles.at(i).y -1);
            glTexCoord2d(thisTexCoord.x, thisTexCoord.y);
        }*/

        if(vertices.size()>0){
            MeshVertex* thisVert = vertices.at(triangles.at(i).x -1);
            glVertex3f(thisVert->p.x, thisVert->p.y, thisVert->p.z);
        }
    }
    glEnd();
    glDisable(GL_LIGHTING);
    glFrontFace(GL_CW);
}
