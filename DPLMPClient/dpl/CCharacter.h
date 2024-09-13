#pragma once
#include "../../DPLMPCommon/PLMath.h"

class CCharacter {
public:
	void SetModel(int model);
	void SetAnimType(int type);
	void SwapSkin(int animations, int skin);
	mat<float, 4, 4>* GetMatrix();
};