#include "mesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <qgl.h>
#include <stringutil.h>
#include <QGLShaderProgram>
using std::vector;
using namespace std;
extern "C"{

    extern void APIENTRY glActiveTexture(GLenum);

    extern void APIENTRY glMultiTexCoord3f(GLenum, GLfloat, GLfloat, GLfloat);
    extern void APIENTRY glMultiTexCoord3i(GLenum, GLint, GLint, GLint);

    extern void APIENTRY glMultiTexCoord4f(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);

}
extern "C" void glMultiTexCoord3f (GLenum, GLfloat, GLfloat, GLfloat);

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
            double x, y, z;
            x = strtod(tokens.at(1).c_str(), NULL);
            y = strtod(tokens.at(2).c_str(), NULL);
            z = strtod(tokens.at(3).c_str(), NULL);
            if(x>currMaxX){
                currMaxX = x;
            }
            if(x<currMinX){
                currMinX = x;
            }
            if(y>currMaxY){
                currMaxY = y;
            }
            if(y<currMinY){
                currMinY = y;
            }
            if(z>currMaxZ){
                currMaxZ = z;
            }
            if(z<currMinZ){
                currMinZ = z;
            }
            vert->p = double3(x,y,z); // set coordinates
            vertices.push_back(vert); // add to the list
            currVertex++;
        }

        // Parse a face line
        else if(tokens.at(0) == "f")
        {
            //quads
            if(tokens.size() == 5) {

                // Parse the vertices, of the form "vertex/normal/texture"
                vector<string> face;
                for(unsigned int i=1; i<tokens.size(); i++)
                {
                    face.clear();
                    StringUtil::split(tokens.at(i), "/", face, true);
                    if(face.size() == 3){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), strtod(face.at(1).c_str(), NULL), strtod(face.at(2).c_str(), NULL), 0);
                        quads.push_back(v);
                    }
                    if(face.size() == 1){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), 0, 0, 0);
                        quads.push_back(v);
                    }
                    else if(face.size() == 2){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), strtod(face.at(1).c_str(), NULL), 0, 0);
                        quads.push_back(v);
                    }

                }
                currFace++;
            }

            else{
                    // Parse the vertices, of the form "vertex/normal/texture"
                vector<string> face;
                for(unsigned int i=1; i<tokens.size(); i++)
                {
                    face.clear();
                    StringUtil::split(tokens.at(i), "/", face, true);
                    if(face.size() == 3){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), strtod(face.at(1).c_str(), NULL), strtod(face.at(2).c_str(), NULL), 0);
                        triangles.push_back(v);
                    }
                    if(face.size() == 1){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), 0, 0, 0);
                        triangles.push_back(v);
                    }
                    else if(face.size() == 2){
                        Vector4 v = Vector4(strtod(face.at(0).c_str(), NULL), strtod(face.at(1).c_str(), NULL), 0, 0);
                        triangles.push_back(v);
                    }

                }
                currFace++;
            }
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
        else{

        }

    }

    Vector3 scaleFac = Vector3(currMaxX - currMinX, currMaxY - currMinY, currMaxZ - currMinZ);
    for(unsigned int i=0; i<vertices.size(); i++){
        MeshVertex* m = vertices.at(i);
        m->p.x = (m->p.x - currMinX)/scaleFac.x;
        m->p.y = (m->p.y - currMinY)/scaleFac.y;
        m->p.z = (m->p.z - currMinZ)/scaleFac.z;
    }
    // Wrap up
    myfile.close();

}


Mesh::~Mesh()
{
    for(unsigned int i=0; i<vertices.size(); i++){
        delete vertices.at(i);
    }
}

void Mesh::drawGL()
{

    //glPushMatrix();
    glFrontFace(GL_CCW);
    if(normals.size() > 0){
        glBegin(GL_TRIANGLES);
        for(unsigned int i=0; i < triangles.size(); i++)
        {
            //for each element on a face we index into normals, vertices, and maybe texture coords
            double3 thisNorm = normals.at(triangles.at(i).z - 1 );
            glNormal3f(thisNorm.x, thisNorm.y, thisNorm.z);
            glMultiTexCoord3f(GL_TEXTURE0, 1.0, 1.0, 1.0);


            if(vertices.size()>0){
                MeshVertex* thisVert = vertices.at(triangles.at(i).x -1);
                glVertex3f(thisVert->p.x, thisVert->p.y, thisVert->p.z);
            }
        }
        glEnd();
        glBegin(GL_QUADS);
        for(unsigned int i=0; i < quads.size(); i++)
        {
            //for each element on a face we index into normals, vertices, and maybe texture coords
            double3 thisNorm = normals.at(quads.at(i).z - 1 );
            glNormal3f(thisNorm.x, thisNorm.y, thisNorm.z);
            glMultiTexCoord3f(GL_TEXTURE0, 1.0, 1.0, 1.0);

            if(vertices.size()>0){
                MeshVertex* thisVert = vertices.at(quads.at(i).x -1);
                glVertex3f(thisVert->p.x, thisVert->p.y, thisVert->p.z);
            }
        }
        glEnd();
    }
    else{


        glBegin(GL_TRIANGLES);
        for(unsigned int i=0; i < triangles.size()/3; i++)
        {
            MeshVertex *vert1, *vert2, *vert3;
            vert1 = vertices.at(triangles.at(i*3).x -1);
            vert2 = vertices.at(triangles.at(i*3 + 1).x -1);
            vert3 = vertices.at(triangles.at(i*3 + 2).x -1);
            double3 v1 = vert2->p - vert1->p;
            double3 v2 = vert3->p - vert1->p;
            Vector3 vec1 = Vector3(v1.data);
            Vector3 vec2 = Vector3(v2.data);
            Vector3 normal = vec1.cross(vec2);
            normal.normalize();
            glMultiTexCoord3f(GL_TEXTURE0, 1.0, 1.0, 1.0);

            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vert1->p.x, vert1->p.y, vert1->p.z);
            glVertex3f(vert2->p.x, vert2->p.y, vert2->p.z);
            glVertex3f(vert3->p.x, vert3->p.y, vert3->p.z);
        }
        glEnd();
        glBegin(GL_QUADS);
        for(unsigned int i=0; i<quads.size() / 4; i++){
            MeshVertex *vert1, *vert2, *vert3, *vert4;
            vert1 = vertices.at(quads.at(i*4).x -1);
            vert2 = vertices.at(quads.at(i*4 + 1).x -1);
            vert3 = vertices.at(quads.at(i*4 + 2).x -1);
            vert4 = vertices.at(quads.at(i*4 + 3).x -1);
            double3 v1 = vert2->p - vert1->p;
            double3 v2 = vert3->p - vert1->p;
            Vector3 vec1 = Vector3(v1.data);
            Vector3 vec2 = Vector3(v2.data);
            Vector3 normal = vec1.cross(vec2);
            normal.normalize();
            glMultiTexCoord3f(GL_TEXTURE0, 1.0, 1.0, 1.0);

            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vert1->p.x, vert1->p.y, vert1->p.z);
            glVertex3f(vert2->p.x, vert2->p.y, vert2->p.z);
            glVertex3f(vert3->p.x, vert3->p.y, vert3->p.z);
            glVertex3f(vert4->p.x, vert4->p.y, vert4->p.z);
        }
        glEnd();
    }

    glColor3f(1.0, 1.0, 1.0);
    glFrontFace(GL_CW);
    //glPopMatrix();
}
