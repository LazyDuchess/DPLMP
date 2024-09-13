#include "CHandling.h"

PHBaseObj* CHandling::GetPhysicsObject() {
	int vTableAddr = *(int*)this;
	return ((PHBaseObj*(__thiscall*)(CHandling*))*(int*)(vTableAddr + 0xEC))(this);
}