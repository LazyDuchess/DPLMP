#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "PHBaseObj.h"

class CVehicle {
public:
	void SetColor(float R, float G, float B);
	PHBaseObj* GetPhysicsObject();
};