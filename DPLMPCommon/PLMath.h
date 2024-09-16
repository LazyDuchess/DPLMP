#pragma once
#include "../thirdparty/qvm/qvm.hpp"

using namespace boost::qvm;

constexpr double pi = 3.14159265358979323846;

void SetPosition(mat<float, 4, 4>* matrix, vec<float, 3>position);

vec<float, 3> GetPosition(mat<float, 4, 4>* matrix);

void SetEulerRotation(mat<float, 4, 4>* matrix, vec<float, 3>rotation);

void SetQuaternionRotation(mat<float, 4, 4>* matrix, quat<float> rotation);

quat<float> GetQuaternionRotation(mat<float, 4, 4>* matrix);

mat<float, 4, 4> MatrixFromPhysicsTransform(vec<float, 3>& position, quat<float>& rotation);