#include "floorplanner.h"

FloorPlanner::FloorPlanner()
{
}


void FloorPlanner::plan(Feature* root)
{

}


void FloorPlanner::drawSelf()
{
    glDisable(GL_LIGHTING);
    //draw background
    drawQuad(double2(0.0,0.0), double2(1.0, 1.0), double3(1.0, 1.0, 1.0));


    drawLine(double2(.25, .25), double2(.5, 1.0));


    drawQuad(double2(.5, .6), double2(.75, .75));


    glEnable(GL_LIGHTING);
}


void FloorPlanner::drawLine(double2 a, double2 b, float width, double3 color)
{

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

    glBegin(GL_QUADS);
    glColor3f(color.x, color.y, color.z);
    glVertex2f(a.x,1.0-a.y);
    glVertex2f(b.x,1.0-a.y);
    glVertex2f(b.x,1.0-b.y);
    glVertex2f(a.x,1.0-b.y);

    glVertex2f(a.x,1.0-a.y);
    glVertex2f(a.x,1.0-b.y);
    glVertex2f(b.x,1.0-b.y);
    glVertex2f(b.x,1.0-a.y);

    glColor3f(1.0, 1.0, 1.0);
    glEnd();
}
