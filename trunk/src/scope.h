#ifndef SCOPE_H
#define SCOPE_H
#include "CS123Algebra.h"

class Scope
{
public:
    Scope(Vector4 point = Vector4(0,0,0,0), Vector4 scale = Vector4(1,1,1,1), Matrix4x4 basis = Matrix4x4::identity());
    Scope(Vector4 point, Vector4 scale, Vector4 xbasis, Vector4 ybasis, Vector4 zbasis);

    Vector4 getPoint();
    Matrix4x4 getBasis();
    Vector4 getScale();

    Scope translate(Vector4 v);
    Scope setSize(Vector4 v);
    Scope rotateX(REAL angle);
    Scope rotateY(REAL angle);
    Scope rotateZ(REAL angle);
    Vector4 getXBasis();
    Vector4 getYBasis();
    Vector4 getZBasis();

private:
    Vector4 m_point;
    Matrix4x4 m_basis;
    Vector4 m_scale;
    Vector4 m_xbasis;
    Vector4 m_ybasis;
    Vector4 m_zbasis;
};

#endif // SCOPE_H