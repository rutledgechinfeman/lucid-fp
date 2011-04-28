#include "scope.h"


Scope::Scope(Vector4 scale, Matrix4x4 basis, Vector4 point){
    m_scale = scale;
    m_basis = basis;
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
