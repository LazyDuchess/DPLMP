#pragma once
#include "PHBaseObj.h"

class CHandling {
public:
	PHBaseObj* GetPhysicsObject();
	void GetOrientation(quat<float>* out);
};