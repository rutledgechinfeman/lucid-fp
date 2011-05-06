#include "scope.h"


Scope::Scope(Vector4 point, Vector4 scale, Matrix4x4 basis){
    m_scale = scale;
    m_basis = basis;
    m_point = point;
    m_xbasis = Vector4(basis.a, basis.e, basis.i, basis.m);
    m_ybasis = Vector4(basis.b, basis.f, basis.j, basis.n);
    m_zbasis = Vector4(basis.c, basis.g, basis.k, basis.o);
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
    return Scope(m_point, m_scale, getRotMat(m_point, m_xbasis, angle) * m_basis);
}

Scope Scope::rotateY(REAL angle){
    return Scope(m_point, m_scale, getRotMat(m_point, m_ybasis, angle) * m_basis);
}

Scope Scope::rotateZ(REAL angle){
    return Scope(m_point, m_scale, getRotMat(m_point, m_zbasis, angle) * m_basis);
}

Scope Scope::copy(){
    return Scope(m_point, m_scale, m_basis);
}

bool Scope::occludes(Scope other) {

    Vector4 corners[8];
    this->findCorners(other, corners);




    return false;
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

void Scope::printSelf()
{
    cout << "============" << endl;
    cout << "Scope:" << endl;
    cout << "Basis:" << endl << m_basis << endl;
    cout << "Scale:" << endl << m_scale << endl;
    cout << "Point:" << endl << m_point << endl;
    cout << "============" << endl;
}


