#ifndef FLOORPLANNER_H
#define FLOORPLANNER_H

#include <vector>
#include "feature.h"
#include "common.h"
#include "scope.h"

struct edge;

struct vertex
{
    vertex(float xpos, float ypos, int idnum = -1)
    {
        x = xpos;
        y = ypos;
        id = idnum;
    }

    vector<edge*> edges;
    int id;
    float x, y;
};

struct edge
{
    edge(vertex* first, vertex* second)
    {
        a = first;
        b = second;
    }

    vertex* a;
    vertex* b;
};

struct rectangle
{
    rectangle(double2 av, double2 bv) { a = av; b = bv; }

    double2 a;
    double2 b;
};

struct graph
{
    vector<vertex*> vertices;
    vector<edge*> edges;
};

class FloorPlanner
{
public:
    FloorPlanner();

    void plan(Feature* root);

    void drawSelf();


private:

    bool equal(double d1, double d2){
        if(abs(d1 - d2) < .01){
            return true;
        }
        return false;
    }
//    vector<double2> getIntersectors(unsigned int i);
//    bool isOnLine(double m, double y, double x, double2 pt);
//    int doTheyIntersect(double2 l1p1, double2 l1p2, double2 l2p1, double2 l2p2);
//    void printIntersections();

//    bool onTheSameLine(double2 v1, double2 v2);

//    void initializeIntersectionsToZero();

//    void setIntersected(unsigned int i, unsigned int j);

    void drawLine(double2 a, double2 b, float width = 2.0, double3 color = double3(0.0, 0.0, 1.0));
    void drawQuad(rectangle, double3 color = double3(1.0, 0.0, 0.0));
    void drawDot(double2 a, double size = 1.0, double3 color = double3(0.0, 1.0, 0.0));

    void buildFirstRepresentation(Feature* root);
    void getScopeCorners(Scope s, double2* array);
    bool bigScope(Scope scope);
    bool hasScope(Scope scope);
    void normalizeTo2D();

    /// Input sanitization
    vector<Scope> m_scopeList;
    vector<Vector4> m_windows;
    vector<Vector4> m_doors;

    vector<rectangle> m_2DscopeList;
    vector<double2> m_2Dwindows;
    vector<double2> m_2Ddoors;
    vector<vector<bool> > intersections;

    /// For scaling
    double2 m_mins;
    double2 m_maxs;

    graph m_inputGraph;
    graph m_finalGraph;
};

#endif // FLOORPLANNER_H
