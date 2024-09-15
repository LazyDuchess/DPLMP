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
	set_identity(*matrix);
	mat<float, 4, 4> rot = rot_mat_xyz<4>(rotation.a[0] * (pi / 180.0f), rotation.a[1] * (pi / 180.0f), rotation.a[2] * (pi / 180.0f));
	*matrix *= rot;
	SetPosition(matrix, oldPos);
}

void SetQuaternionRotation(mat<float, 4, 4>* matrix, quat<float> rotation) {
	vec<float, 3> oldPos = GetPosition(matrix);
	set_identity(*matrix);
	*matrix *= convert_to<mat<float, 4, 4>>(rotation);
	SetPosition(matrix, oldPos);
}

quat<float> ToDriverOrientation(quat<float> q) {
	return { q.a[1], q.a[2], q.a[3], -q.a[0] };
}

quat<float> GetQuaternionRotation(mat<float, 4, 4>* matrix) {
	mat<float, 3, 3> extracted_rotation;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			extracted_rotation.a[i][j] = matrix->a[i][j];
		}
	}
	return ToDriverOrientation(convert_to<quat<float>>(extracted_rotation));
}