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

    putWindowsAndDoorsOnGrid();

    doTheRobot();
}


void FloorPlanner::doTheRobot()
{
    m_currRobotID = 0;
    queue<int2> poseQueue;

    do
    {
        vector<int2> poses;
        findRobotPos(poses, m_currRobotID);
        for (unsigned int i = 0; i < poses.size(); ++i)
        {
            poseQueue.push(poses[i]);
        }

        int2 startPos = poseQueue.front();
        poseQueue.pop(); // who knows why this isn't built in


        if (robot(startPos, startPos)) // Don't increment on a bust...
        {
            m_currRobotID ++;
        }
    } while (!poseQueue.empty());
}

bool FloorPlanner::robot(int2 start, int2 end)
{
    // Base case: if our start position is invalid (someone already built into it, or it's somehow otherwise erroneous)
    if (m_planGrid[start.x][start.y] >= 0 || m_planGrid[start.x][start.y] == OUTSIDE) return false;

    /*
     * Pick one of several options: for X and Y, max += 1 or min -= 1, or both
     */

    int xmulti = 1;
    int ymulti = 1;
    // Step 1: Figure out which directions you can expand in
    if (start.x == end.x)
    {
        if (m_planGrid[start.x - 1][start.y] < 0 && m_planGrid[start.x - 1][start.y] != OUTSIDE) xmulti = -1;
    }
    else
    {
        if (start.x > end.x) xmulti = -1;
    }

    if (start.y == end.y)
    {
        if (m_planGrid[start.x][start.y - 1] < 0 && m_planGrid[start.x][start.y - 1] != OUTSIDE) ymulti = -1;
    }
    else
    {
        if (start.y > end.y) ymulti = -1;
    }

    // Step 2: Decide whether to keep expanding based on some randomized factor. If not, claim your explored area and finish.
    if (happy(start, end))
    {
        claim(start, end);
        return true;
    }

    // Step 3: Figure out which of those directions you want to expand in
    int2 dir;
    dir = int2(xmulti, ymulti);
    if (canExpand(start, end, dir))
    {
        robot(start, end + dir);
        return true;
    }
    dir = int2(0, ymulti);
    if (canExpand(start, end, dir))
    {
        robot(start, end + dir);
        return true;
    }
    dir = int2(xmulti, 0);
    if (canExpand(start, end, dir))
    {
        robot(start, end + dir);
        return true;
    }

    // Step 4: You can no longer expand, claim your explored area
    claim(start, end);

    return true;
}

bool FloorPlanner::happy(const int2& start, const int2& end)
{
    // TODO: Don't allow stopping on windows and/or doors

    // TODO: Area based heuristic
//    int area = abs((start.x - end.x) * (start.y - end.y));
//    area /= (RESOLUTION_CONSTANT * RESOLUTION_CONSTANT);

//    double pval = ((double)area - 20.0) / 50.0;
//    pval = max(min(pval, 1.0), 0.0);

//    double sample = (double) rand() / (double) RAND_MAX; // random number between 0 and 1 (found this online)
//    if (sample < pval)
//    {
//        return true;
//    }

    return false;
}

void FloorPlanner::claim(const int2& start, const int2& end)
{
    for (int x = min(start.x, end.x); x < max(start.x, end.x); ++ x)
    {
        for (int y = min(start.y, end.y); y < max(start.y, end.y); ++ y)
        {
            m_planGrid[x][y] = m_currRobotID;
        }
    }
}

bool FloorPlanner::canExpand(const int2& start, int2 end, const int2& dir)
{
    end += dir;
    for (int x = min(start.x, end.x); x < max(start.x, end.x); ++ x)
    {
        for (int y = min(start.y, end.y); y < max(start.y, end.y); ++ y)
        {
            if (m_planGrid[x][y] >= 0 || m_planGrid[x][y] == OUTSIDE)
            {
                return false;
            }
        }
    }

    return true;
}

void FloorPlanner::findRobotPos(vector<int2> &validList, int i) {

    if (i == 0)
    {
        int inNeighbors, outNeighbors;
        for (int x = 1; x < m_maxs.x - m_mins.x - 1; ++ x)
        {
            for (int y = 1; y < m_maxs.y - m_mins.y - 1; ++ y)
            {
                inNeighbors = 0;
                outNeighbors = 0;
                if (m_planGrid[x][y] == OUTSIDE) continue;

                if (m_planGrid[x+1][y] != OUTSIDE) inNeighbors ++;
                else outNeighbors ++;

                if (m_planGrid[x-1][y] != OUTSIDE) inNeighbors ++;
                else outNeighbors ++;

                if (m_planGrid[x][y+1] != OUTSIDE) inNeighbors ++;
                else outNeighbors ++;

                if (m_planGrid[x][y-1] != OUTSIDE) inNeighbors ++;
                else outNeighbors ++;

                if (outNeighbors == 2 && inNeighbors == 2) validList.push_back(int2(x, y));
            }
        }

        return;
    }

    i -= 1; // We already incremented the robot number, so look for the PREVIOUS one

    int inNeighbors, iNeighbors;
    for (int x = 1; x < m_maxs.x - m_mins.x - 1; ++ x)
    {
        for (int y = 1; y < m_maxs.y - m_mins.y - 1; ++ y)
        {
            if (m_planGrid[x][y] >= 0 || m_planGrid[x][y] == OUTSIDE) continue;

            int2 p;
            inNeighbors = 0;
            iNeighbors = 0;

            p = int2(x + 1, y);
            if (m_planGrid[p.x][p.y] < 0 && m_planGrid[p.x][p.y] != OUTSIDE) inNeighbors ++;
            else if(m_planGrid[p.x][p.y] == i) iNeighbors ++;

            p = int2(x - 1, y);
            if (m_planGrid[p.x][p.y] < 0 && m_planGrid[p.x][p.y] != OUTSIDE) inNeighbors ++;
            else if(m_planGrid[p.x][p.y] == i) iNeighbors ++;

            p = int2(x, y + 1);
            if (m_planGrid[p.x][p.y] < 0 && m_planGrid[p.x][p.y] != OUTSIDE) inNeighbors ++;
            else if(m_planGrid[p.x][p.y] == i) iNeighbors ++;

            p = int2(x, y - 1);
            if (m_planGrid[p.x][p.y] < 0 && m_planGrid[p.x][p.y] != OUTSIDE) inNeighbors ++;
            else if(m_planGrid[p.x][p.y] == i) iNeighbors ++;

            if (inNeighbors == 2 && iNeighbors == 1) validList.push_back(int2(x, y));
        }
    }
}

void FloorPlanner::printGrid()
{
    for (int x = 0; x < m_maxs.x - m_mins.x; ++ x)
    {
        for (int y = 0; y < m_maxs.y - m_mins.y; ++ y)
        {
            if (m_planGrid[x][y] == OUTSIDE) cout << "   ";
            else cout << m_planGrid[x][y] << " ";
        }
        cout << endl;
    }
}

void FloorPlanner::putWindowsAndDoorsOnGrid()
{
    for (unsigned int i = 0; i < m_2Dwindows.size(); ++i)
    {
        double2 temp = m_2Dwindows[i] - m_mins + 0.5;
        int2 loc = int2(temp.x, temp.y);
        int r = RESOLUTION_CONSTANT * WINDOW_LENGTH / 2.0;
        for (int x = loc.x - r; x < loc.x + r; ++ x)
        {
            for (int y = loc.y - r; y < loc.y + r; ++ y)
            {
                if (!inBounds(int2(x, y))) continue;
                if (m_planGrid[x][y] == OUTSIDE) continue;
                m_planGrid[x][y] = WINDOW;
            }
        }
    }

    for (unsigned int i = 0; i < m_2Ddoors.size(); ++i)
    {
        double2 temp = m_2Ddoors[i] - m_mins + 0.5;
        int2 loc = int2(temp.x, temp.y);
        int r = RESOLUTION_CONSTANT * DOOR_LENGTH / 2.0;
        for (int x = loc.x - r; x < loc.x + r; ++ x)
        {
            for (int y = loc.y - r; y < loc.y + r; ++ y)
            {
                if (!inBounds(int2(x, y))) continue;
                if (m_planGrid[x][y] == OUTSIDE) continue;
                m_planGrid[x][y] = DOOR;
            }
        }
    }
}

bool FloorPlanner::inBounds(int2 p)
{
    return p.x >= 0 && p.x < (m_maxs.x - m_mins.x) && p.y >= 0 && p.y < (m_maxs.y - m_mins.y);
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

    for (int x = m_mins.x; x < m_maxs.x; ++ x)
    {
        for (int y = m_mins.y; y < m_maxs.y; ++ y)
        {
            int data = m_planGrid[x - m_mins.x][y - m_mins.y];
            double3 color;
            switch (data)
            {
                case INSIDE:
                    color = double3(1.0, 0.0, 0.0);
                    break;
                case WINDOW:
                    color = double3(0.0, 0.0, 1.0);
                    break;
                case DOOR:
                    color = double3(0.0, 1.0, 0.0);
                    break;
                case OUTSIDE:
                    color = double3(1.0, 1.0, 1.0);
                    break;
                default:
                    double intensity = (double)data/(double)m_currRobotID;
                    color = double3(intensity, intensity, intensity);
            }

            drawQuad(rectangle(int2(x, y), int2(x+1, y+1)), color);
        }
    }

    glEnable(GL_LIGHTING);
}

void FloorPlanner::drawDot(double2 a, double size, double3 color)
{
    size *= (double) RESOLUTION_CONSTANT / 2.0;
    drawQuad(a + double2(size, size), a - double2(size, size), color);
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

void FloorPlanner::drawQuad(double2 a, double2 b, double3 color)
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

    a = (a - l_min) / big;
    b = (b - l_min) / big;

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

void FloorPlanner::drawQuad(rectangle r, double3 color)
{
    drawQuad(double2(r.a), double2(r.b), color);
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
