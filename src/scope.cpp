#include "scope.h"


Scope::Scope()
{
}

Scope::Scope(Vector4 scale, Matrix4x4 basis, Vector4 point){
    m_scale = scale;
    m_basis = basis;
    m_point = point;
}
