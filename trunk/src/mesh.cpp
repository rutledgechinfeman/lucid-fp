#include "mesh.h"
#include <boost/algorithm/string.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <qgl.h>
#include <stringutil.h>
using std::vector;
using namespace std;


Mesh::Mesh(string filename){
    loadMesh(filename);


    /*
    string line;
    ifstream myfile(filename.c_str());

    if (!myfile.is_open() || !myfile.good()) {
        cout << "grammar file fail" << endl;
        return false;
    }

    string dir = filename.substr(0, filename.find_last_of("/") + 1);

    getline(myfile, line);
    parseSymbolFile(dir + line, fac);
    getline(myfile, line);
    parseRuleFile(dir + line, grammar);


    myfile.close();

    return true;*/
}

bool Mesh::loadMesh(string filename){
    ifstream myfile(filename.c_str());
    if(!myfile.is_open() || !myfile.good()){
        cout << "obj file fail" << endl;
        cout << "hi" << endl;
        return false;
    }
    vector<MeshVertex*> vertices;
    int* triangles;

    string line;
    vector<string> tokens;
    //for each line in the file
    int currVertex = 0;
    int currFace = 0;
    while(myfile.good()){
        tokens.clear();
        getline(myfile, line);

        if (line.size() == 0) continue;

        StringUtil::split(line, " ", tokens);
        string func = tokens[0];
        //comment
        if(tokens.at(0) == "#"){
            continue;
        }
        else if(tokens.at(0) == "v"){
            //vertex, v
            if(func.size() == 1){
                MeshVertex* vert;
                vert->idx = currVertex;
                if(tokens.size() < 4){
                    cout << "not enough coordinates in vertex number " << currVertex << endl;
                }
                vert->p = double3(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()));
                vertices.push_back(vert);
                currVertex++;
            }
        }
        //vertex normal, vn
        else if(tokens.at(0) == "n"){
            //TODO: NORMAL STUFF?
        }
        //vertex texture, vt
        else if(tokens.at(0) == "t"){
            //TODO: TEXTURE STUFF?
        }

        //face stuff
        else if(tokens.at(0) == "f"){
            //FACE STUFF
            if(tokens.size() != 4){
                cout << "This may be a quad mesh" << endl;
            }
            vector<string> face;
            for(int i=1; i<tokens.size(); i++){
                face.clear();
                StringUtil::split(tokens[i], "/", face);
                triangles[currFace*3 + (i-1)] = atoi(face.at(0).c_str());
            }
            currFace++;
        }
    }


}

Mesh::Mesh(MeshVertex *vertexlist, int numvertices, int *trilist, int numtriangles) {
    m_vertices = new MeshVertex[numvertices];
    memcpy(m_vertices, vertexlist, sizeof(MeshVertex) * numvertices);
    m_triangles = new MeshTriangle[numtriangles];

    for(int i=0; i<numtriangles; i++) {
        m_triangles[i].v0 = &m_vertices[trilist[i*3]];
        m_triangles[i].v1 = &m_vertices[trilist[i*3+1]];
        m_triangles[i].v2 = &m_vertices[trilist[i*3+2]];
    }

    m_numtriangles = numtriangles;
    m_numvertices = numvertices;
}


Mesh::~Mesh() {
    delete[] m_triangles;
    delete[] m_vertices;
}

void Mesh::drawGL() {
    glBegin(GL_TRIANGLES);
    for(int i=0; i < m_numtriangles; i++) {
        glTexCoord2f(m_triangles[i].v0->t.x, m_triangles[i].v0->t.y);
        glVertex3f(m_triangles[i].v0->p.x, m_triangles[i].v0->p.y, m_triangles[i].v0->p.z);
        glTexCoord2f(m_triangles[i].v1->t.x, m_triangles[i].v1->t.y);
        glVertex3f(m_triangles[i].v1->p.x, m_triangles[i].v1->p.y, m_triangles[i].v1->p.z);
        glTexCoord2f(m_triangles[i].v2->t.x, m_triangles[i].v2->t.y);
        glVertex3f(m_triangles[i].v2->p.x, m_triangles[i].v2->p.y, m_triangles[i].v2->p.z);
    }
    glEnd();
}
/*
void Mesh::drawTriangleIDs() {
    glDisable(GL_TEXTURE_2D);

    for(int i=0; i < m_numtriangles; i++) {
        //convert i to rgb
        int k1 = i / (255 * 255);
        int k2 = (i - k1 * (255 * 255)) / (255);
        int k3 = (i - k2 * 255 - k1 * (255 * 255));
        glColor3f(k1 / 255.0f, k2 / 255.0f, k3 / 255.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(m_triangles[i].v0->p.x, m_triangles[i].v0->p.y);
        glVertex2f(m_triangles[i].v1->p.x, m_triangles[i].v1->p.y);
        glVertex2f(m_triangles[i].v2->p.x, m_triangles[i].v2->p.y);
        glEnd();
    }

    glEnable(GL_TEXTURE_2D);
}
*/
