#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "PHBaseObj.h"

class CHandling;

class CVehicle {
public:
	void SetColor(vec<float,3> rgb);
	void GetHandling(CHandling** handling);
	PHBaseObj* GetPhysicsObject();
};