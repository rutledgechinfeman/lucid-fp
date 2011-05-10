#ifndef FLOORPLANNER_H
#define FLOORPLANNER_H

#include <vector>
#include "feature.h"
#include "common.h"
#include "scope.h"


#define RESOLUTION_CONSTANT 50.0
#define WINDOW_LENGTH 1.0
#define DOOR_LENGTH 1.0


#define INSIDE      -1
#define WINDOW      -2
#define DOOR        -3
#define OUTSIDE     -4

struct rectangle
{
    rectangle(int2 av, int2 bv) { a = av; b = bv; }
    rectangle(double2 av, double2 bv) { a.x = (int)(av.x+0.5); a.y = (int)(av.y+0.5); b.x = (int)(bv.x+0.5); b.y = (int)(bv.y+0.5); }
    int2 topLeft()
    {
        return int2(min(a.x, b.x), min(a.y, b.y));
    }
    int2 bottomRight()
    {
        return int2(max(a.x, b.x), max(a.y, b.y));
    }

    int2 a;
    int2 b;
};

class FloorPlanner
{
public:
    FloorPlanner();
    virtual ~FloorPlanner();

    void plan(Feature* root);

    void drawSelf();


private:
    void drawLine(double2 a, double2 b, float width = 2.0, double3 color = double3(0.0, 0.0, 1.0));
    void drawQuad(double2 a, double2 b, double3 color = double3(1.0, 0.0, 0.0));
    void drawQuad(rectangle, double3 color = double3(1.0, 0.0, 0.0));
    void drawDot(double2 a, double size = 1.0, double3 color = double3(0.0, 1.0, 0.0));

    void buildFirstRepresentation(Feature* root);
    void getScopeCorners(Scope s, int2* array);
    void normalizeTo2D();
    void buildPlanGrid();
    void deleteGrid();
    void newGrid();
    void putWindowsAndDoorsOnGrid();
    bool inBounds(int2 p);
    void printGrid();

    /// Input sanitization
    vector<Scope> m_scopeList;
    vector<Vector4> m_windows;
    vector<Vector4> m_doors;

    vector<rectangle> m_2DscopeList;
    vector<double2> m_2Dwindows;
    vector<double2> m_2Ddoors;

    /// For scaling
    int2 m_mins;
    int2 m_maxs;

    int** m_planGrid;
};

#endif // FLOORPLANNER_H
