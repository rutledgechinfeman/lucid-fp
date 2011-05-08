#include "scope.h"


Scope::Scope(Vector4 point, Vector4 scale, Matrix4x4 basis){
    m_scale = scale;
    m_basis = basis;
    m_point = point;
    m_xbasis = Vector4(basis.a, basis.e, basis.i, basis.m);
    m_ybasis = Vector4(basis.b, basis.f, basis.j, basis.n);
    m_zbasis = Vector4(basis.c, basis.g, basis.k, basis.o);

    this->findCorners(*this, m_corners);
}

Scope::Scope(Vector4 point, Vector4 scale, Vector4 xbasis, Vector4 ybasis, Vector4 zbasis){
    m_scale = scale;
    m_basis = Matrix4x4(xbasis.x, ybasis.x, zbasis.x, 0.0,
                        xbasis.y, ybasis.y, zbasis.y, 0.0,
                        xbasis.z, ybasis.z, zbasis.z, 0.0,
                        xbasis.w, ybasis.w, zbasis.w, 1.0);
    m_xbasis = xbasis;
    m_ybasis = ybasis;
    m_zbasis = zbasis;
    m_point = point;

    this->findCorners(*this, m_corners);
}

Vector4 Scope::getPoint(){
    return m_point;
}

Vector4 Scope::getScale(){
    return m_scale;
}

Matrix4x4 Scope::getBasis(){
    return m_basis;
}

Vector4 Scope::getXBasis(){
    return m_xbasis;
}
Vector4 Scope::getYBasis(){
    return m_ybasis;
}
Vector4 Scope::getZBasis(){
    return m_zbasis;
}
Vector4 Scope::getBasisComponent(int i){
    switch (i) {
        case 0:
            return getXBasis();
        case 1:
            return getYBasis();
        case 2:
            return getZBasis();
        default:
            return Vector4::zero();
    }
}

Scope Scope::setBasisComponent(int i, Vector4 vec) {

    switch (i) {
    case 0:
        return Scope(m_point, m_scale, vec, m_ybasis, m_zbasis);
    case 1:
        return Scope(m_point, m_scale, m_xbasis, vec, m_zbasis);
    case 2:
        return Scope(m_point, m_scale, m_xbasis, m_ybasis, vec);
    default:
        return this->copy();
    }
}


Scope Scope::translate(Vector4 v){
    return Scope(m_point+v, m_scale, m_basis);
}

Scope Scope::setScale(Vector4 v){
    return Scope(m_point, v, m_basis);
}

Scope Scope::setScaleComponent(REAL v, int i) {

    Vector4 newScale = Vector4(m_scale);
    newScale.data[i] = v;

    return Scope(m_point, newScale, m_basis);
}


Scope Scope::rotateX(REAL angle){
    angle *= M_PI / 180.0;
    Matrix4x4 rotMatP = getRotMat(m_point + .5*(m_basis*m_scale), m_xbasis, angle);
    Matrix4x4 rotMatB = getRotMat(Vector4::zero(), m_xbasis, angle);
    return Scope(rotMatP * m_point, m_scale, rotMatB *m_basis);
}

Scope Scope::rotateY(REAL angle){
    angle *= M_PI / 180.0;
    Matrix4x4 rotMatP = getRotMat(m_point + .5*(m_basis*m_scale), m_ybasis, angle);
    Matrix4x4 rotMatB = getRotMat(Vector4::zero(), m_ybasis, angle);
    return Scope(rotMatP * m_point, m_scale, rotMatB * m_basis);
}

Scope Scope::rotateZ(REAL angle){
    angle *= M_PI / 180.0;
    Matrix4x4 rotMatP = getRotMat(m_point + .5*(m_basis*m_scale), m_zbasis, angle);
    Matrix4x4 rotMatB = getRotMat(Vector4::zero(), m_zbasis, angle);
    return Scope(rotMatP * m_point, m_scale, rotMatB * m_basis);
}

Scope Scope::copy(){
    return Scope(m_point, m_scale, m_basis);

}

int Scope::occludes(Scope other) {

    Vector4 corners[8];
    this->findCorners(other, corners);

    int cornerContainCount = 0;
    for (int i = 0; i < 8; i ++) {
        if (contains(corners[i])) cornerContainCount ++;
    }

    if (cornerContainCount == 8) { return 2; }
    else if (cornerContainCount > 0) { return 1; }
    else { //must check edges

        Vector4 a = corners[0];
        Vector4 b = corners[1];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[2];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[4];
        if (intersectedBy(a, b) > 0) return 1;


        a = corners[7];
        b = corners[6];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[3];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[5];
        if (intersectedBy(a, b) > 0) return 1;

        a = corners[1];
        b = corners[5];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[3];
        if (intersectedBy(a, b) > 0) return 1;

        a = corners[2];
        b = corners[3];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[6];
        if (intersectedBy(a, b) > 0) return 1;

        a = corners[4];
        b = corners[5];
        if (intersectedBy(a, b) > 0) return 1;
        b = corners[6];
        if (intersectedBy(a, b) > 0) return 1;
    }


    return 0;
}

void Scope::findCorners(Scope scope, Vector4* corners) {

    Vector4 p = scope.getPoint();
    Vector4 xB = scope.getXBasis();
    Vector4 yB = scope.getYBasis();
    Vector4 zB = scope.getZBasis();
    Vector4 s = scope.getScale();

    corners[0] = p;
    corners[1] = p + xB * s.x;
    corners[2] = p + yB * s.y;
    corners[3] = p + xB * s.x + yB * s.y;

    for (int i = 0 ; i < 4; i ++) {
        corners[i+4] = corners[i] + zB * s.z;
    }

}

bool Scope::contains(Vector4 point) {

    int num = intersectedBy(point, Vector4(100000, 100000, 100000, 1.0));

    if (num % 2 == 1) return true;
    return false;
}

//dont ask
int Scope::intersectedBy(Vector4 a, Vector4 b) {
    int result = 0;
    Vector4 p1 = m_corners[0];
    Vector4 p2 = m_corners[1];
    Vector4 p3 = m_corners[2];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;
    p2 = m_corners[4];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;
    p3 = m_corners[1];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;

    p1 = m_corners[7];
    p2 = m_corners[6];
    p3 = m_corners[5];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;
    p2 = m_corners[3];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;
    p3 = m_corners[6];
    if (intersectsPlane(a, b, p1, p2, p3)) result ++;

    return result;
}

bool Scope::intersectsPlane(Vector4 a, Vector4 b, Vector4 p1, Vector4 p2, Vector4 p3) {

    Vector4 d = b-a; //find intersection point with entire plane
    Vector4 n = (p2-p1).cross(p3 - p1);


    //line and plane are||
    if (fabs(d.dot(n)) < .00001) return false;

    double t = (p1 - a).dot(n) / (d.dot(n));


    //if intersection doesn't occur on this line segment
    if (t < 0 || t > 1) {
        return false;
    }

    Vector4 p = a + d*t;


    Vector4 p4 = p3 + (p2 - p1); //get fourth corner of the plane

    Vector4 c1 = (p1 - p).cross(p2 - p);
    Vector4 c = (p2 - p).cross(p3 - p); //make sure that p lies INSIDE the plane bounds
    if (c1.dot(c) < 0) return false;
    c = (p3 - p).cross(p4 - p);
    if (c1.dot(c) < 0) return false;
    c = (p4 - p).cross(p1 - p);
    if (c1.dot(c) < 0) return false;


    return true;
}

void Scope::printSelf()
{
    cout << "============" << endl;
    cout << "Scope:" << endl;
    cout << "Basis:" << endl << m_basis << endl;
    cout << "Scale:" << endl << m_scale << endl;
    cout << "Point:" << endl << m_point << endl;
    cout << "============" << endl;
}


