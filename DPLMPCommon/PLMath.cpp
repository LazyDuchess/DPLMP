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

quat<float> GetQuaternionRotation(mat<float, 4, 4>* matrix) {
	mat<float, 3, 3> extracted_rotation;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			extracted_rotation.a[i][j] = matrix->a[i][j];
		}
	}
	return convert_to<quat<float>>(extracted_rotation);
}

//Decompilation of obUpdateTransMatrix
mat<float, 4, 4> VisualMatrixFromPhysicsTransform(vec<float, 3> &position, quat<float> &rotation) {
	float px = position.a[0];
	float py = position.a[1];
	float pz = position.a[2];

	float rx = rotation.a[0];
	float ry = rotation.a[1];
	float rz = rotation.a[2];
	float rw = rotation.a[3];

	mat<float, 4, 4> matrix;
	float fVar1 = rx * 2.0;
	float fVar2 = fVar1 * ry;
	float fVar9 = fVar1 * rz;
	float fVar3 = ry * 2.0;
	float fVar4 = fVar3 * ry;
	fVar3 = fVar3 * rz;
	float fVar8 = rz * 2.0 * rz;
	float fVar6 = rw * 2.0;
	float fVar5 = rx * fVar6;
	float fVar7 = fVar6 * ry;
	fVar6 = fVar6 * rz;
	matrix.a[0][0] = (1.0 - fVar4) - fVar8;
	matrix.a[1][0] = fVar2 - fVar6;
	matrix.a[2][0] = fVar7 + fVar9;
	matrix.a[0][1] = fVar6 + fVar2;
	fVar1 = 1.0 - rx * fVar1;
	matrix.a[1][1] = fVar1 - fVar8;
	matrix.a[2][1] = fVar3 - fVar5;
	matrix.a[0][2] = fVar9 - fVar7;
	matrix.a[1][2] = fVar5 + fVar3;
	matrix.a[2][2] = fVar1 - fVar4;
	matrix.a[3][0] = px;
	matrix.a[3][1] = py;
	matrix.a[3][2] = pz;
	matrix.a[3][3] = 0.0;
	matrix.a[0][3] = 0.0;
	matrix.a[1][3] = 0.0;
	matrix.a[2][3] = 0.0;
	matrix.a[3][3] = 1.0;

	return matrix;
}

vec<float, 3> Lerp(vec<float, 3> start, vec<float, 3> end, float percent) {
	if (percent > 1.0)
		percent = 1.0;
	if (percent < 0.0)
		percent = 0.0;
	return start + ((end - start) * percent);
}

quat<float> SLerp180Clamped(quat<float> start, quat<float> end, float percent) {
	if (percent > 1.0)
		percent = 1.0;
	if (percent < 0.0)
		percent = 0.0;
	return slerp180(start, end, percent);
}