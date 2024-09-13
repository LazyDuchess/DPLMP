#pragma once
#include "../../DPLMPCommon/PLMath.h"
class PHBaseObj {
public:
	vec<float, 3> GetPosition();
	void SetPosition(vec<float, 3> position);
};