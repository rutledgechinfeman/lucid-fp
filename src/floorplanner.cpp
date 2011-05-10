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

    normalizeTo2D();

    // Make a graph out of the rectangles we have

    // For each edge, check interstections, add intersections to the graph, remove redundant points

    // Create another graph. Walk around the outside of the original, add only good points/edges to the new one.

    // Add windows and doors: for each edge, find all the windows on it. subdivide that edge based on those.

    // Pass this crap to rutledge

}


void FloorPlanner::drawSelf()
{
    glDisable(GL_LIGHTING);
    //draw background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

        if (curr->getMassModel() != NULL)
        {
            m_scopeList.push_back(curr->getScope());
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

//void FloorPlanner::initializeIntersectionsToZero(){
//    for(unsigned int i=0; i<m_scopeList.size() * 4; i++){
//        vector<bool> v = vector<bool>(m_scopeList.size() * 4);
//        intersections.push_back(v);
//    }
//}

void FloorPlanner::normalizeTo2D(){

//    initializeIntersectionsToZero();

    for (vector<Scope>::iterator it = m_scopeList.begin(); it != m_scopeList.end(); it ++) {
        double2 corners[4];
        getScopeCorners(*it, corners);

        m_2DscopeList.push_back(rectangle(corners[0], corners[2]));


        for (int i = 0; i < 4; i ++) {
            m_mins.x = min(m_mins.x, corners[i].x);
            m_mins.y = min(m_mins.y, corners[i].y);

            m_maxs.x = max(m_maxs.x, corners[i].x);
            m_maxs.y = max(m_maxs.y, corners[i].y);
        }

//        for(int i=0; i<4; i++){
//            for(int j=i; j<4; j++){
//                if(onTheSameLine(corners[i], corners[j])){
//                    intersections[m_vertices.size()-4 + i][ m_vertices.size()-4 + j] = true;
//                    intersections[ m_vertices.size()-4 + j][m_vertices.size()-4 + i] = true;
//                    setIntersected(m_vertices.size()-4 + i, m_vertices.size()-4 + j);
//                }
//            }
//        }





    }


    for (vector<Vector4>::iterator it = m_doors.begin(); it != m_doors.end(); it ++) {
        m_2Ddoors.push_back(double2(it->x, it->z));
    }

    for (vector<Vector4>::iterator it = m_windows.begin(); it != m_windows.end(); it ++) {
        m_2Dwindows.push_back(double2(it->x, it->z));
    }




}

//void FloorPlanner::printIntersections(){
//    for(unsigned int i=0; i<intersections.size(); i++){
//        for(unsigned int j=0; j<intersections.size(); j++){
//            cout<< intersections[i][j]<<" ";
//        }
//        cout<< endl;
//    }
//}


//bool FloorPlanner::onTheSameLine(double2 v1, double2 v2){
//    if(EQ(v1.x, v2.x) || EQ(v1.y, v2.y)){
//        return true;
//    }
//    return false;
//}

//void FloorPlanner::setIntersected(unsigned int a, unsigned int b){
//    //given that two indices (of vertices in m_vertices) are Intersected (by a wall, probably)
//    double2 l1p1 = m_vertices.at(a);
//    double2 l1p2 = m_vertices.at(b);
//    //for every vertex in m_vertices
//    for(unsigned int i=0; i<m_vertices.size(); i++){
//        double2 l2p1 = m_vertices[i];
//        //for every vertex that it is intersected with
//        vector<double2> inters = getIntersectors(i);
//        for(unsigned int j=0; j<inters.size(); j++){

//            double2 l2p2 = inters.at(j);

//            int val = doTheyIntersect(l1p1, l1p2, l2p1, l2p2);
//            //if val is 0 then they are parallel (and not the same line)
//            //if val is 1 they intersected at one point
//            //if val is 2 they are the same line

//        }

//    }

//    //TODO: SET INTERSECTION STUFF!!
//}

//int FloorPlanner::doTheyIntersect(double2 l1p1, double2 l1p2, double2 l2p1, double2 l2p2){

//    double m = (l1p1.y - l1p2.y) / (l1p1.x - l1p2.y);

//    if(isOnLine(m, l1p1.y, l1p1.x, l2p1) && isOnLine(m, l1p1.y, l1p1.x, l2p2)){
//        return 2;
//    }

//    else{
//        double A1 = l1p2.y - l1p1.y;
//        double B1 = l1p1.x - l1p2.x;

//        double A2 = l2p2.y - l2p1.y;
//        double B2 = l2p1.x - l2p2.x;

//        double det = A1*B2 - A2*B1;
//        if(EQ(det, 0)){
//            return 0;
//        }
//    }
//    return 1;
//}

//bool FloorPlanner::isOnLine(double m, double y, double x, double2 pt){
//    if(EQ(y-pt.y, m*(x-pt.x))){
//        return true;
//    }
//    return false;
//}

//vector<double2> FloorPlanner::getIntersectors(unsigned int i){
//    vector<double2> toRet;
//    vector<bool> thisRow = intersections.at(i);

//    for(unsigned int j=0; j<thisRow.size(); j++){
//        if(thisRow.at(j)){
//            toRet.push_back(m_vertices.at(j));
//        }
//    }
//    return toRet;
//}


void FloorPlanner::getScopeCorners(Scope s, double2* array)
{
    Vector4 D3corners[4];
    D3corners[0] = s.getPoint();
    D3corners[1] = s.getPoint() + s.getXBasis() * s.getScale().x;
    D3corners[2] = s.getPoint() + s.getXBasis() * s.getScale().x + s.getZBasis() * s.getScale().z;
    D3corners[3] = s.getPoint() + s.getZBasis() + s.getScale().z;

    for (int i = 0; i < 4; i ++)
    {
        array[i] = double2(D3corners[i].x, D3corners[i].z);
    }
}


bool FloorPlanner::bigScope(Scope scope) {
    Vector4 scale = scope.getScale();
    return (scale.x > 1 && scale.y > 1 && scale.z > 1 && fabs(scope.getPoint().y) < .01);
}

bool FloorPlanner::hasScope(Scope scope) {

    Scope comp;
    for (unsigned int i = 0; i < m_scopeList.size(); i ++) {
        comp = m_scopeList[i];

        if (comp.getScale() == scope.getScale() && comp.getPoint() == scope.getPoint()) return true;

    }

    return false;
}
