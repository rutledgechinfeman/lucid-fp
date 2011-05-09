#ifndef FLOORPLANNER_H
#define FLOORPLANNER_H

#include "feature.h"
#include <vector>
#include "common.h"
#include "scope.h"


struct rectangle
{
    rectangle(double2 av, double2 bv) { a = av; b = bv; }

    double2 a;
    double2 b;
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
    vector<double2> getIntersectors(unsigned int i);
    bool isOnLine(double m, double y, double x, double2 pt);
    int doTheyIntersect(double2 l1p1, double2 l1p2, double2 l2p1, double2 l2p2);
    void printIntersections();

    bool onTheSameLine(double2 v1, double2 v2);

    void initializeIntersectionsToZero();

    void setIntersected(unsigned int i, unsigned int j);
    vector<Scope> m_scopeList;
    vector<Vector4> m_windows;
    vector<Vector4> m_doors;
    vector<double2> m_vertices;

    vector<rectangle> m_2DscopeList;
    vector<double2> m_2Dwindows;
    vector<double2> m_2Ddoors;

    vector<vector<bool> > intersections;

    double2 m_mins;
    double2 m_maxs;

    void drawLine(double2 a, double2 b, float width = 2.0, double3 color = double3(0.0, 0.0, 1.0));
    void drawQuad(rectangle, double3 color = double3(1.0, 0.0, 0.0));


    void buildFirstRepresentation(Feature* root);
    bool bigScope(Scope scope);
    bool hasScope(Scope scope);
    void normalizeTo2D();

    void drawDot(double2 a, double size = 1.0, double3 color = double3(0.0, 1.0, 0.0));


    double2* getScopeCorners(Scope s);

};

#endif // FLOORPLANNER_H
