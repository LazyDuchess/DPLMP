#include "PLMath.h"

void SetPosition(mat<float, 4, 4>* matrix, vec<float, 3>position) {
	matrix->a[3][0] = position.a[0];
	matrix->a[3][1] = position.a[1];
	matrix->a[3][2] = position.a[2];
	matrix->a[3][3] = 1.0;
}

vec<float, 3> GetPosition(mat<float, 4, 4>* matrix) {
	return { matrix->a[3][0], matrix->a[3][1], matrix->a[3][2] };
};

void SetEulerRotation(mat<float, 4, 4>* matrix, vec<float, 3>rotation) {
	vec<float, 3> oldPos = GetPosition(matrix);
	SetPosition(matrix, { 0.0, 0.0, 0.0 });
	mat<float, 4, 4> rot = rot_mat_xyz<4>(rotation.a[0] * (pi / 180.0f), rotation.a[1] * (pi / 180.0f), rotation.a[2] * (pi / 180.0f));
	*matrix *= rot;
	SetPosition(matrix, oldPos);
}