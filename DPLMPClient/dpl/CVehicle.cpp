#include "CVehicle.h"

void CVehicle::SetColor(vec<float,3> rgb)
{
	*(float*)(this + 140) = rgb.a[0];
	*(float*)(this + 144) = rgb.a[1];
	*(float*)(this + 148) = rgb.a[2];
}

void CVehicle::GetHandling(CHandling** handling) {
	int vTableAddr = *(int*)this;
	((void(__thiscall*)(CVehicle*, CHandling**)) * (int*)(vTableAddr + 0x64))(this, handling);
}

PHBaseObj* CVehicle::GetPhysicsObject() {
	CHandling* carHandling = nullptr;
	GetHandling(&carHandling);
	return carHandling->GetPhysicsObject();
}