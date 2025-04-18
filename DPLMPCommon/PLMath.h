#pragma once
#include "../thirdparty/qvm/qvm.hpp"

using namespace boost::qvm;

constexpr double pi = 3.14159265358979323846;

void SetPosition(mat<float, 4, 4>* matrix, vec<float, 3>position);

vec<float, 3> GetPosition(mat<float, 4, 4>* matrix);

void SetEulerRotation(mat<float, 4, 4>* matrix, vec<float, 3>rotation);

void SetQuaternionRotation(mat<float, 4, 4>* matrix, quat<float> rotation);

quat<float> GetQuaternionRotation(mat<float, 4, 4>* matrix);

mat<float, 4, 4> VisualMatrixFromPhysicsTransform(vec<float, 3>& position, quat<float>& rotation);

vec<float, 3> Lerp(vec<float, 3> start, vec<float, 3> end, float percent);

quat<float> SLerp180Clamped(quat<float> start, quat<float> end, float percent);