#pragma once
#include "../../DPLMPCommon/PLMath.h"
// Angular velocity is still a mystery to me
class PHBaseObj {
public:
	mat<float, 4, 4>* GetMatrix();
	vec<float, 3> GetPosition();
	void SetPosition(vec<float, 3> position);
	vec<float, 3> GetVelocity();
	void SetVelocity(vec<float, 3> velocity);
	quat<float> GetRotation();
	void SetRotation(quat<float> rotation);
	void WakeUp(bool wakeUp);
};