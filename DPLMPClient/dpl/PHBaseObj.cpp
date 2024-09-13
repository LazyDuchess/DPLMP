#include "PHBaseObj.h"

void PHBaseObj::SetPosition(vec<float, 3> position) {
	*(float*)(this + 0x80) = position.a[0];
	*(float*)(this + 0x84) = position.a[0];
	*(float*)(this + 0x88) = position.a[0];
}

vec<float, 3> PHBaseObj::GetPosition() {
	return vec<float, 3>{ *(float*)(this + 0x80), * (float*)(this + 0x84), * (float*)(this + 0x88) };
}