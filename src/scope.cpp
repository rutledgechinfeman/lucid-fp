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


Scope Scope::translate(Vector4 v){
    return Scope(m_point+v, m_scale, m_basis);
}

Scope Scope::setSize(Vector4 v){
    return Scope(m_point, v, m_basis);
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
