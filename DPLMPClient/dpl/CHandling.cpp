#include "CHandling.h"

PHBaseObj* CHandling::GetPhysicsObject() {
	int vTableAddr = *(int*)this;
	return ((PHBaseObj*(__thiscall*)(CHandling*))*(int*)(vTableAddr + 0xEC))(this);
}

void CHandling::GetOrientation(quat<float>* out) {
	int vTableAddr = *(int*)this;
	return ((void(__thiscall*)(CHandling*, quat<float>*)) * (int*)(vTableAddr + 0x10))(this, out);
}