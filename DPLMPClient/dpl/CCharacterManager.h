#pragma once
#include "../../DPLMPCommon/PLMath.h"

class CCharacterManager {
public:
	static CCharacterManager* GetInstance();
	void CreateCharacter(int model, vec<float, 4>* position, float health);
};