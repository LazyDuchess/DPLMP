#include "CVehicle.h"

void CVehicle::SetColor(float R, float G, float B)
{
	*(float*)(this + 140) = R;
	*(float*)(this + 144) = G;
	*(float*)(this + 148) = B;
}

CHandling* CVehicle::GetHandling() {
	return *(CHandling**)((int)this + 0x2C);
}