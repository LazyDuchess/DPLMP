#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "CHandling.h"

class CVehicle {
public:
	void SetColor(vec<float,3> rgb);
	void GetHandling(CHandling** handling);
};