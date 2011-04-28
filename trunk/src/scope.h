#ifndef SCOPE_H
#define SCOPE_H
#include "CS123Algebra.h"

class Scope
{
public:
    Scope(Vector4 scale = Vector4(1,1,1,1), Matrix4x4 basis = Matrix4x4::identity(), Vector4 point = Vector4(0,0,0,0));
    Vector4 getPoint();
    Matrix4x4 getBasis();
    Vector4 getScale();


private:
    Vector4 m_point;
    Matrix4x4 m_basis;
    Vector4 m_scale;
};

#endif // SCOPE_H
