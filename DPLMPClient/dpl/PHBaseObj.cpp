#include "PHBaseObj.h"

void PHBaseObj::SetPosition(vec<float, 3> position) {
	*(float*)(this + 0x80) = position.a[0];
	*(float*)(this + 0x84) = position.a[1];
	*(float*)(this + 0x88) = position.a[2];
}

mat<float, 4, 4>* PHBaseObj::GetMatrix() {
	return (mat<float, 4, 4>*)((int)this + 16);
}

mat<float, 4, 4>* PHBaseObj::GetObjectToWorldMatrix() {
	return (mat<float, 4, 4>*)((int)this + 0xf0);
}

vec<float, 3> PHBaseObj::GetPosition() {
	return vec<float, 3>{ *(float*)(this + 0x80), * (float*)(this + 0x84), * (float*)(this + 0x88) };
}

void PHBaseObj::SetVelocity(vec<float, 3> velocity) {
	*(float*)(this + 0x90) = velocity.a[0];
	*(float*)(this + 0x94) = velocity.a[1];
	*(float*)(this + 0x98) = velocity.a[2];
}

vec<float, 3> PHBaseObj::GetVelocity() {
	return vec<float, 3>{ *(float*)(this + 0x90), * (float*)(this + 0x94), * (float*)(this + 0x98) };
}

void PHBaseObj::SetRotation(quat<float> rotation) {
	*(float*)(this + 0xA0) = rotation.a[0];
	*(float*)(this + 0xA4) = rotation.a[1];
	*(float*)(this + 0xA8) = rotation.a[2];
	*(float*)(this + 0xAC) = rotation.a[3];
}

void PHBaseObj::WakeUp() {
	((void(__thiscall*)(PHBaseObj*))0x005c56dc)(this);
}

quat<float> PHBaseObj::GetRotation() {
	return quat<float>{ *(float*)(this + 0xA0), * (float*)(this + 0xA4), * (float*)(this + 0xA8), * (float*)(this + 0xAC) };
}

void PHBaseObj::SetPositionAndOrientation(vec<float, 3> position, quat<float> rotation, bool wakeUp) {
	SetPosition(position);
	if (wakeUp)
		WakeUp();
	UpdateTransMatrix();
	SetRotation(rotation);
	//UpdateState();
	if (wakeUp)
		WakeUp();
	UpdateTransMatrix();
}

void PHBaseObj::UpdateTransMatrix() {
	((void(__thiscall*)(PHBaseObj*))0x005cf4e0)(this);
}

void PHBaseObj::UpdateState() {
	((void(__thiscall*)(PHBaseObj*))0x005cdb3a)(this);
}