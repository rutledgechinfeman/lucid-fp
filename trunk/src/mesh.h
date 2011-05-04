#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QSet>
#include "common.h"
#include <string>
#include "CS123Algebra.h"
using std::vector;
using namespace std;

struct MeshVertex {
   double2 t; //texture coordinate
   double3 n;
   double3 p; //vertex location and
   int idx; //vertex index
};

struct MeshTriangle {
    MeshVertex *v0, *v1, *v2; //triangle vertices in ccw order
};

class Mesh
{
public:
    Mesh(string s);
    //trilist length must be a multiple of 3.  Thus numtriangles = length of trilist / 3.
    //Mesh(MeshVertex *vertexlist, int numvertices, int *trilist, int numtriangles);
    ~Mesh();

    void drawGL();
    //void drawTriangleIDs();
    //MeshTriangle *triangles() { return m_triangles; }
    //MeshVertex *vertices() { return m_vertices; }
    //int numTriangles() { return m_numtriangles; }
    //int numVertices() { return m_numvertices; }




protected:
    vector<MeshVertex*> vertices;
    vector<Vector4> triangles;
    vector<double3> normals;
    vector<double2> texcoords;
    //MeshVertex *m_vertices;
    //MeshTriangle *m_triangles;
    //double *m_texcoords;
    //int m_numvertices, m_numtriangles;
};

#endif // MESH_H
