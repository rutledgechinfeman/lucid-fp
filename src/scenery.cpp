#include "scenery.h"
#include <iostream>
using namespace std;

Scenery::Scenery()
{
}


void Scenery::draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH); //Enable smoothing

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();
    glPopMatrix();
}
