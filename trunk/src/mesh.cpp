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

    vector<MeshVertex*> vertices;
    vector<int> triangles;

    // Iterate over each line in the file
    string line;
    vector<string> tokens;
    int currVertex = 0;
    int currFace = 0;
    while (myfile.good())
    {
        getline(myfile, line);
        if (line.size() == 0) continue;

        // Tokenize line
        tokens.clear();
        StringUtil::split(line, " ", tokens);

        // Skip over commented lines
        if(tokens.at(0) == "#") { continue; }

        // Parse a vertex line
        if(tokens.at(0) == "v")
        {
            if(tokens.size() != 4) { cerr << "WARNING: Skipping malformed vertex line in mesh file: " << line << endl; }

            // Make a new vertex for this line
            MeshVertex* vert = new MeshVertex();
            vert->idx = currVertex; // set ID
            vert->p = double3(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str())); // set coordinates
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
                StringUtil::split(tokens.at(i), "/", face);
                triangles.push_back(atoi(face.at(0).c_str())); // Currently, we're only supporting the vertex numbers, not normals or textures

                // TODO: if we ever implement this feature
                if(face.size() > 1) { cerr << "WARNING: Currently, normals/textures are not supported for triangles. Skipping non-vertex data of: " << line << endl; }
            }
            currFace++;
        }
        // Parse a vertex normal line
        else if(tokens.at(0) == "n") { cerr << "WARNING: Currently, normals are not supported for mesh files. Skipping: " << line << endl; } // TODO
        // Parse a vertex texture line
        else if(tokens.at(0) == "t") { cerr << "WARNING: Currently, normals are not supported for mesh files. Skipping: " << line << endl; } // TODO
    }

    // Wrap up
    myfile.close();

    // Populate the master vertex list
    m_vertices = new MeshVertex[vertices.size()];
    for(unsigned int i=0; i<vertices.size(); i++)
    {
        m_vertices[i].idx = vertices.at(i)->idx;
        m_vertices[i].p = vertices.at(i)->p;
    }

    // Populate the master triangle list
    m_triangles = new MeshTriangle[triangles.size()/3];
    for(unsigned int i=0; i<triangles.size(); i++)
    {
        m_triangles[i].v0 = &m_vertices[triangles[i*3]];
        m_triangles[i].v1 = &m_vertices[triangles[i*3+1]];
        m_triangles[i].v2 = &m_vertices[triangles[i*3+2]];
    }

    m_numtriangles = currFace;
    m_numvertices = vertices.size();

}

// TODO: Would this constructor ever be used?
Mesh::Mesh(MeshVertex *vertexlist, int numvertices, int *trilist, int numtriangles)
{
    m_vertices = new MeshVertex[numvertices];
    memcpy(m_vertices, vertexlist, sizeof(MeshVertex) * numvertices);
    m_triangles = new MeshTriangle[numtriangles];

    for(int i=0; i<numtriangles; i++)
    {
        m_triangles[i].v0 = &m_vertices[trilist[i*3]];
        m_triangles[i].v1 = &m_vertices[trilist[i*3+1]];
        m_triangles[i].v2 = &m_vertices[trilist[i*3+2]];
    }

    m_numtriangles = numtriangles;
    m_numvertices = numvertices;
}


Mesh::~Mesh()
{
    delete[] m_triangles;
    delete[] m_vertices;
}

void Mesh::drawGL()
{
    glBegin(GL_TRIANGLES);
    for(int i=0; i < m_numtriangles; i++)
    {
        glTexCoord2f(m_triangles[i].v0->t.x, m_triangles[i].v0->t.y);
        glVertex3f(m_triangles[i].v0->p.x, m_triangles[i].v0->p.y, m_triangles[i].v0->p.z);

        glTexCoord2f(m_triangles[i].v1->t.x, m_triangles[i].v1->t.y);
        glVertex3f(m_triangles[i].v1->p.x, m_triangles[i].v1->p.y, m_triangles[i].v1->p.z);

        glTexCoord2f(m_triangles[i].v2->t.x, m_triangles[i].v2->t.y);
        glVertex3f(m_triangles[i].v2->p.x, m_triangles[i].v2->p.y, m_triangles[i].v2->p.z);
    }
    glEnd();
}

/*void Mesh::drawTriangleIDs()
{
    glDisable(GL_TEXTURE_2D);

    for(int i=0; i < m_numtriangles; i++)
    {
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
}*/
