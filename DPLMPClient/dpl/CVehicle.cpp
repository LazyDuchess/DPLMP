#include "CVehicle.h"

void CVehicle::SetColor(float R, float G, float B)
{
	*(float*)(this + 140) = R;
	*(float*)(this + 144) = G;
	*(float*)(this + 148) = B;
}

PHBaseObj* CVehicle::GetPhysicsObject() {
	return *(PHBaseObj**)((int)this + 0xef80);
}