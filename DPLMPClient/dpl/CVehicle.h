#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "CHandling.h"

class CVehicle {
public:
	void SetColor(float R, float G, float B);
	CHandling* GetHandling();
};