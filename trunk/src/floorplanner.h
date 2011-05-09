#ifndef FLOORPLANNER_H
#define FLOORPLANNER_H

#include "feature.h"
#include <vector>
#include "common.h"


class FloorPlanner
{
public:
    FloorPlanner();

    void plan(Feature* root);

    void drawSelf();


private:

    vector<Scope> m_scopeList;
    vector<Vector4> m_windows;
    vector<Vector4> m_doors;

    void drawLine(double2 a, double2 b, float width = 2.0, double3 color = double3(0.0, 0.0, 1.0));
    void drawQuad(double2 a, double2 b, double3 color = double3(1.0, 0.0, 0.0));

};

#endif // FLOORPLANNER_H
