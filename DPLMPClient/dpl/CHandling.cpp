#include "CHandling.h"

PHBaseObj* CHandling::GetPhysicsObject() {
	int vTableAddr = *(int*)this;
	return ((PHBaseObj*(__thiscall*)(CHandling*))*(int*)(vTableAddr + 0xEC))(this);
}

void CHandling::GetOrientation(quat<float>* out) {
	int vTableAddr = *(int*)this;
	return ((void(__thiscall*)(CHandling*, quat<float>*)) * (int*)(vTableAddr + 0x10))(this, out);
}

CVehicle* CHandling::GetVehicle() {
	return *(CVehicle**)((int)this + 0x244);
}

float CHandling::GetSteering() {
	return *(float*)((int)this + 0xc4);
}

void CHandling::SetSteering(float steering) {
	*(float*)((int)this + 0xc4) = steering;
}

bool CHandling::GetHandbraking() {
	return *(bool*)((int)this + 0xcc);
}

void CHandling::SetHandbraking(bool handbraking) {
	*(bool*)((int)this + 0xcc) = handbraking;
}

float CHandling::GetPower() {
	return *(float*)((int)this + 0xc0);
}

void CHandling::SetPower(float power) {
	*(float*)((int)this + 0xc0) = power;
}

float CHandling::GetThrottle() {
	return *(float*)((int)this + 0x32c);
}

void CHandling::SetThrottle(float throttle) {
	*(float*)((int)this + 0x32c) = throttle;
}

float CHandling::GetRPM() {
	return *(float*)((int)this + 0x5c);
}

void CHandling::SetRPM(float rpm) {
	*(float*)((int)this + 0x5c) = rpm;
}