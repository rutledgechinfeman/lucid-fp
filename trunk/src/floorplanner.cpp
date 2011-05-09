#include "floorplanner.h"
#include <queue>
#include "CS123Algebra.h"
#include <string>

FloorPlanner::FloorPlanner()
{
    m_mins = double2(1000000, 1000000);
    m_maxs = double2(-1000000, -1000000);

}


void FloorPlanner::plan(Feature* root)
{


    m_scopeList.clear();
    m_windows.clear();
    m_doors.clear();

    m_2DscopeList.clear();
    m_2Dwindows.clear();
    m_2Ddoors.clear();

    m_mins = double2(1000000, 1000000);
    m_maxs = double2(-1000000, -1000000);

    buildFirstRepresentation(root);

//    for (int i = 0; i < m_scopeList.size() ; ++ i)
//    {m_scopeList[i].printSelf();}
//    cout << "********" << endl;


    normalizeTo2D();

//    for (int i = 0; i < m_2DscopeList.size() ; ++ i)
//    {cout << m_2DscopeList[i].a << " :: " << m_2DscopeList[i].b << endl;}

}


void FloorPlanner::drawSelf()
{
    glDisable(GL_LIGHTING);
    //draw background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for (int i = 0; i < m_2DscopeList.size(); i ++) {
        drawQuad(m_2DscopeList[i]);
    }

    for (int i = 0; i < m_2Dwindows.size(); i ++) {
        drawDot(m_2Dwindows[i], 0.5, double3(0, 1, 0));
    }

    for (int i = 0; i < m_2Ddoors.size(); i ++) {
        drawDot(m_2Ddoors[i], 1.0, double3(0, 0, 1));
    }

    glEnable(GL_LIGHTING);
}


void FloorPlanner::drawDot(double2 a, double size, double3 color) {

    size = size/2.0;
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
    double big = m_maxs.x - m_mins.x;
    double small = m_maxs.y - m_mins.y;
    int bigger = 0;
    if (m_maxs.x - m_mins.x < m_maxs.y - m_mins.y) {
        big = m_maxs.y - m_mins.y;
        small = m_maxs.x - m_mins.x;
        bigger = 1;
    }

    r.a = (r.a - m_mins) / big;
    r.b = (r.b - m_mins) / big;

    r.a.data[1-bigger] += (1.0 - small/big) * .5;
    r.b.data[1-bigger] += (1.0 - small/big) * .5;

    glBegin(GL_QUADS);
    glColor3f(color.x, color.y, color.z);
    glVertex2f(1.0-r.a.x,1.0-r.a.y);
    glVertex2f(1.0-r.b.x,1.0-r.a.y);
    glVertex2f(1.0-r.b.x,1.0-r.b.y);
    glVertex2f(1.0-r.a.x,1.0-r.b.y);

    glVertex2f(1.0-r.a.x,1.0-r.a.y);
    glVertex2f(1.0-r.a.x,1.0-r.b.y);
    glVertex2f(1.0-r.b.x,1.0-r.b.y);
    glVertex2f(1.0-r.b.x,1.0-r.a.y);

    glColor3f(1.0, 1.0, 1.0);
    glEnd();
}

void FloorPlanner::buildFirstRepresentation(Feature* root) {

    queue<Feature*> q;
    q.push(root);
    Feature* curr;
    while (!q.empty()) {

        curr = q.front();
        q.pop();

        if (bigScope(curr->getScope())) {

            if (!hasScope(curr->getScope())) {
                m_scopeList.push_back(curr->getScope());
            }

        }

        if (curr->getSymbol().find("door") != string::npos) {
            if (curr->getScope().getPoint().y < 1.5)
                m_doors.push_back(curr->getScope().getPoint() + .5*curr->getScope().getBasis()*curr->getScope().getScale());
        }

        if (curr->getSymbol().find("window") != string::npos) {
            if (curr->getScope().getPoint().y < 1.5)
                m_windows.push_back(curr->getScope().getPoint() + .5*curr->getScope().getBasis()*curr->getScope().getScale());
        }

        //add children
        for (int i = 0; i < curr->getNumChildren(); i ++) {
            q.push(curr->getChild(i));
        }

    }
}


void FloorPlanner::normalizeTo2D(){


    for (vector<Scope>::iterator it = m_scopeList.begin(); it != m_scopeList.end(); it ++) {
        double2* corners = getScopeCorners(*it);

        m_2DscopeList.push_back(rectangle(corners[0], corners[2]));


        for (int i = 0; i < 4; i ++) {
            m_mins.x = min(m_mins.x, corners[i].x);
            m_mins.y = min(m_mins.y, corners[i].y);

            m_maxs.x = max(m_maxs.x, corners[i].x);
            m_maxs.y = max(m_maxs.y, corners[i].y);
        }
    }


    for (vector<Vector4>::iterator it = m_doors.begin(); it != m_doors.end(); it ++) {
        m_2Ddoors.push_back(double2(it->x, it->z));
    }

    for (vector<Vector4>::iterator it = m_windows.begin(); it != m_windows.end(); it ++) {
        m_2Dwindows.push_back(double2(it->x, it->z));
    }

}


double2* FloorPlanner::getScopeCorners(Scope s) {

    double2 toReturn[4];

    Vector4 D3corners[4];



    D3corners[0] = s.getPoint();
    D3corners[1] = s.getPoint() + s.getXBasis() * s.getScale().x;
    D3corners[2] = s.getPoint() + s.getXBasis() * s.getScale().x + s.getZBasis() * s.getScale().z;
    D3corners[3] = s.getPoint() + s.getZBasis() + s.getScale().z;

    s.printSelf();
    cout << "*" << endl;
    cout << s.getScale().x << endl;
    cout << "*" << endl;

    for (int i = 0; i < 4; i ++) {
        toReturn[i] = double2(D3corners[i].x, D3corners[i].z);
        cout << D3corners[i] << endl;

    }
    cout << "***" << endl;

    return toReturn;

}


bool FloorPlanner::bigScope(Scope scope) {
    Vector4 scale = scope.getScale();
    return (scale.x > 1 && scale.y > 1 && scale.z > 1 && fabs(scope.getPoint().y) < .01);
}

bool FloorPlanner::hasScope(Scope scope) {

    Scope comp;
    for (int i = 0; i < m_scopeList.size(); i ++) {
        comp = m_scopeList[i];

        if (comp.getScale() == scope.getScale() && comp.getPoint() == scope.getPoint()) return true;

    }

    return false;
}
