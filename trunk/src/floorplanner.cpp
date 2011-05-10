#include "floorplanner.h"
#include <queue>
#include "CS123Algebra.h"
#include <string>

FloorPlanner::FloorPlanner()
{
    m_mins = int2(1000000, 1000000);
    m_maxs = int2(-1000000, -1000000);

    m_planGrid = NULL;

}

FloorPlanner::~FloorPlanner()
{
    deleteGrid();
}

void FloorPlanner::deleteGrid() {

    if (m_planGrid)
    {
        for (int i = 0; i < m_maxs.x - m_mins.x; i ++)
        {
            delete [] m_planGrid[i];
            m_planGrid[i] = NULL;
        }
        delete [] m_planGrid;
        m_planGrid = NULL;
    }
}

void FloorPlanner::newGrid() {

    int width = m_maxs.x - m_mins.x;
    int height = m_maxs.y - m_mins.y;

    m_planGrid = new int*[width];

    for (int i = 0; i < width; i ++)
    {
        m_planGrid[i] = new int[height];
        for (int j = 0; j < height; ++j)
        {
            m_planGrid[i][j] = OUTSIDE;
        }
    }
}

void FloorPlanner::plan(Feature* root)
{
    deleteGrid();

    m_scopeList.clear();
    m_windows.clear();
    m_doors.clear();

    m_2DscopeList.clear();
    m_2Dwindows.clear();
    m_2Ddoors.clear();

    m_mins = int2(1000000, 1000000);
    m_maxs = int2(-1000000, -1000000);

    buildFirstRepresentation(root);

    normalizeTo2D();

    buildPlanGrid();

}

void FloorPlanner::buildPlanGrid()
{
    newGrid();

    int2 min, max;
    for (unsigned int i = 0; i < m_2DscopeList.size(); i ++)
    {
        min = m_2DscopeList[i].topLeft() - m_mins;
        max = m_2DscopeList[i].bottomRight() - m_mins;

        for (int x = min.x; x < max.x; ++ x)
        {
            for (int y = min.y; y < max.y; ++y)
            {
                m_planGrid[x][y] = INSIDE;
            }
        }
    }
}


void FloorPlanner::drawSelf()
{
    glDisable(GL_LIGHTING);
    //draw background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (unsigned int i = 0; i < m_2DscopeList.size(); i ++) {

        drawQuad(m_2DscopeList[i]);
    }

    for (unsigned int i = 0; i < m_2Dwindows.size(); i ++) {
        drawDot(m_2Dwindows[i], 0.5, double3(0, 1, 0));
    }

    for (unsigned int i = 0; i < m_2Ddoors.size(); i ++) {
        drawDot(m_2Ddoors[i], 1.0, double3(0, 0, 1));
    }

    glEnable(GL_LIGHTING);
}

void FloorPlanner::drawDot(double2 a, double size, double3 color) {

    size *= (double) RESOLUTION_CONSTANT / 2.0;
    drawQuad(rectangle(a + double2(size, size), a - double2(size, size)), color);
}

void FloorPlanner::drawLine(double2 a, double2 b, float width, double3 color)
{

    double scale = max(m_maxs.x - m_mins.x, m_maxs.y - m_mins.y);

    a = (a - m_mins) / scale;
    b = (b - m_mins) / scale;


    glLineWidth(width);
    glColor3f(color.x, color.y, color.z);
    glBegin(GL_LINE_STRIP);
    glVertex2f(a.x, 1.0-a.y);
    glVertex2f(b.x, 1.0-b.y);

    glColor3f(1.0, 1.0, 1.0);
    glEnd();

}

void FloorPlanner::drawQuad(rectangle r, double3 color)
{

    double2 l_max(m_maxs);
    double2 l_min(m_mins);

    double big = l_max.x - l_min.x;
    double small = l_max.y - l_min.y;
    int bigger = 0;
    if (l_max.x - l_min.x < l_max.y - l_min.y) {
        big = l_max.y - l_min.y;
        small = l_max.x - l_min.x;
        bigger = 1;
    }

    double2 a, b;

    a = (double2(r.a) - l_min) / big;
    b = (double2(r.b) - l_min) / big;

    a.data[1-bigger] += (1.0 - small/big) * .5;
    b.data[1-bigger] += (1.0 - small/big) * .5;

    glBegin(GL_QUADS);
    glColor3f(color.x, color.y, color.z);
    glVertex2f(1.0-a.x,1.0-a.y);
    glVertex2f(1.0-b.x,1.0-a.y);
    glVertex2f(1.0-b.x,1.0-b.y);
    glVertex2f(1.0-a.x,1.0-b.y);

    glVertex2f(1.0-a.x,1.0-a.y);
    glVertex2f(1.0-a.x,1.0-b.y);
    glVertex2f(1.0-b.x,1.0-b.y);
    glVertex2f(1.0-b.x,1.0-a.y);

    glColor3f(1.0, 1.0, 1.0);
    glEnd();
}

void FloorPlanner::buildFirstRepresentation(Feature* root)
{
    queue<Feature*> q;
    q.push(root);
    Feature* curr;
    while (!q.empty())
    {
        curr = q.front();
        q.pop();

        if (curr->getMassModel() != NULL)
        {
            m_scopeList.push_back(curr->getScope());
        }

        if (curr->getSymbol().find("door") != string::npos)
        {
            if (curr->getScope().getPoint().y < 1.5)
                m_doors.push_back(curr->getScope().getPoint() + .5*curr->getScope().getBasis()*curr->getScope().getScale());
        }

        if (curr->getSymbol().find("window") != string::npos)
        {
            if (curr->getScope().getPoint().y < 1.5)
                m_windows.push_back(curr->getScope().getPoint() + .5*curr->getScope().getBasis()*curr->getScope().getScale());
        }

        //add children
        for (int i = 0; i < curr->getNumChildren(); i ++)
        {
            q.push(curr->getChild(i));
        }
    }
}

void FloorPlanner::normalizeTo2D()
{
    for (vector<Scope>::iterator it = m_scopeList.begin(); it != m_scopeList.end(); it ++)
    {
        int2 corners[4];
        getScopeCorners(*it, corners);

        for (int i = 0; i < 4; i ++)
        {

            corners[i] = corners[i] * RESOLUTION_CONSTANT;

        }

        m_2DscopeList.push_back(rectangle(corners[0], corners[2]));


        for (int i = 0; i < 4; i ++)
        {
            m_mins.x = min(m_mins.x, corners[i].x);
            m_mins.y = min(m_mins.y, corners[i].y);

            m_maxs.x = max(m_maxs.x, corners[i].x);
            m_maxs.y = max(m_maxs.y, corners[i].y);
        }


        m_mins = m_mins - int2((double)RESOLUTION_CONSTANT / 5.0, (double)RESOLUTION_CONSTANT / 5.0);
        m_maxs = m_maxs + int2((double)RESOLUTION_CONSTANT / 5.0, (double)RESOLUTION_CONSTANT / 5.0);




    }

    for (vector<Vector4>::iterator it = m_doors.begin(); it != m_doors.end(); it ++)
    {
        m_2Ddoors.push_back(double2(it->x, it->z) * RESOLUTION_CONSTANT);
    }

    for (vector<Vector4>::iterator it = m_windows.begin(); it != m_windows.end(); it ++)
    {
        m_2Dwindows.push_back(double2(it->x, it->z) * RESOLUTION_CONSTANT);
    }
}

void FloorPlanner::getScopeCorners(Scope s, int2* array)
{
    Vector4 D3corners[4];
    D3corners[0] = s.getPoint();
    D3corners[1] = s.getPoint() + s.getXBasis() * s.getScale().x;
    D3corners[2] = s.getPoint() + s.getXBasis() * s.getScale().x + s.getZBasis() * s.getScale().z;
    D3corners[3] = s.getPoint() + s.getZBasis() + s.getScale().z;

    for (int i = 0; i < 4; i ++)
    {
        array[i] = int2(D3corners[i].x, D3corners[i].z);
    }
}
